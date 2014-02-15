/*
 * Copyright (c) 2006-2014, Bryan Drewery <bryan@shatow.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* String.cpp
 */

#include "String.h"
#include "Array.h"
#include "HashTable.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include "base64.h"
//#include <memory>
//#include <iostream>

BDLIB_NS_BEGIN

unsigned char String::cleanse_ctr = 0;

/* Accessors */
int String::compare(const String& str, size_t n, size_t start) const
{
  const size_t my_len = length();
  /* Same string? */
  if (data() == str.data() && my_len == str.length())
    return 0;

  const size_t slen = n ? std::min(str.length(), n) : str.length();
  const size_t len = std::min(my_len - start, slen);
  const int diff = std::memcmp(begin() + start, str.begin(), len);
  if (diff)
    return diff;
  else if (n)
    return std::min(my_len - start, n) - slen;
  else
    return (my_len - start) - slen;
}

/* Setters */

/**
 * @brief Insert a cstring at the given index.
 * @param string The cstring to be inserted.
 * @param pos The index to insert at.
 * @param n Up to this many characters will be used from the string.
 * @post A buffer is allocated.
 * @post If the old buffer was too small, it is enlarged.
 * @post The string is inserted at the given index.
 */
void String::insert(size_t pos, const char *string, size_t n)
{
  if (n == 0) return;
  if (pos && !hasIndex(pos-1)) return;
  
  size_t slen = (n == npos) ? std::strlen(string) : n;

  AboutToModify(length() + slen);
  std::memmove(Buf() + pos + slen, Buf() + pos, length() - pos);
  std::copy(string, string + slen, Buf() + pos);
  addLength(slen);
}

/**
 * @brief Replaces the given index with the given cstring up to the given amount of characters.
 * @param pos The index to replace at.
 * @param string The cstring to replace with.
 * @param n How many characters to use from string.
 */
void String::replace(size_t pos, const char *string, size_t n)
{
  if (n == 0) return;
  if (pos && !hasIndex(pos-1)) return;

  size_t slen = (n == npos) ? std::strlen(string) : n;
  size_t newlen = pos + slen;
  
  if (newlen >= length()) {
    AboutToModify(newlen);
  } else {
    newlen = length();
    getOwnCopy();
  }
  std::copy(string, string + slen, Buf() + pos);
  setLength(newlen);
}

std::istream& operator>>(std::istream& is, String& string) {
  char ch;
  string = "";    // empty string, will build one char at-a-time
  is >> ch;    // whitespace skipped, first non-white char in ch

  if (!is.fail())
  {
    do
    {
      string += ch;
      is.get(ch);
    } while (!is.fail() && !isspace(ch));

    if (isspace(ch))    // put whitespace back on the stream
    {
      is.putback(ch);
    }
  }
  return is;
}

std::istream& getline(std::istream& is, String& string) {
 
  char ch;
  string = "";     // empty string, will build one char at-a-time
 
  while (is.get(ch) && ch != '\n')
    string += ch;
  
  return is;
}

/*
 * @brief Split a string by a delimiter
 * @param delim The delimiter to use
 * @param limit Optional param which specified the max number of elements to return
 * @note Excess whitespace is trimmed similar to Ruby's split.
 */
Array<String> String::split(const String& delim, size_t limit) const {
  if (!length()) return Array<String>();
  if (limit == 1) {
    String list[] = {*this};
    return Array<String>(list, 1);
  }

  const String space(' ');
  Array<String> array;
  // Use a temporary - is a fast reference and never modified, so no COW ever used.
  String str(*this);
  size_t pos = 0;

  while (array.size() < limit - 1) {
    // Trim out left whitespace
    if (delim == space) while (str.length() && str[0] == ' ') ++str;

    // All done when there's nothing left
    if (!str) break;

    if ((pos = str.find(delim)) == npos)
      pos = str.length();
    array << str(0, pos);
    str += pos + delim.length();
  }

  // Add on extra
  if (limit != npos) {
    // Trim out left whitespace
    if (delim == space) while (str.length() && str[0] == ' ') ++str;
    array << str;
  }

  return array;
}

String String::operator*(int times) const {
  String newString((this->length() * times));
  for (int i = 0; i < times; ++i)
    newString += *this;
  return newString;
}

String String::printf(const char* format, ...) {
  char va_out[4096] = "";
  va_list va;

  va_start(va, format);
  size_t len = vsnprintf(va_out, sizeof(va_out), format, va);
  va_end(va);

  return String(va_out, len);
}

String& String::chomp() {
  if (length() && (*this)[length() - 1] == '\n') --(*this);
  if (length() && (*this)[length() - 1] == '\r') --(*this);
  return *this;
}

String& String::trim() {
  // ltrim
  while (length() && isspace((*this)[0])) { ++offset; subLength(1); }
  // rtrim
  while (length() && isspace((*this)[length() - 1])) subLength(1);
  return *this;
}

size_t String::find(const String& str) const {
  if (length() >= str.length()) {
    const size_t last_pos = length() - str.length();
    for (size_t pos = 0; pos <= last_pos; ++pos)
      if (str[0] == (*this)[pos] && !std::memcmp(begin() + pos, str.begin(), std::min(str.length(), length() - pos)))
        return pos;
  }
  return npos;
}

size_t String::ifind(const String& str) const {
  String str_upper(str), this_upper(*this);
  std::transform(str.begin(), str.end(), str_upper.begin(), ::toupper);
  std::transform(this->begin(), this->end(), this_upper.begin(), ::toupper);
  return this_upper.find(str_upper);
}

size_t String::rifind(const String& str, const size_t lpos) const {
  String str_upper(str), this_upper(*this);
  std::transform(str.begin(), str.end(), str_upper.begin(), ::toupper);
  std::transform(this->begin(), this->end(), this_upper.begin(), ::toupper);
  return this_upper.rfind(str_upper);
}

size_t String::rfind(const String& str, const size_t lpos) const {
  if (length() >= str.length()) {
    const size_t last_pos = length() - str.length();
    for (size_t pos = last_pos; pos + 1 > lpos; --pos)
      if (str[0] == (*this)[pos] && !std::memcmp(begin() + pos, str.begin(), std::min(str.length(), length() - pos)))
        return pos;
  }
  return npos;
}

String String::sub(const String& search, const String& replacement, int limit) const {
  String newStr(length()), search_str(*this);
  size_t pos = 0;
  int cnt = 0;
  // Search in our const string so that stuff like \->\\ doesnt become \\\, \\\\ etc.
  while ((pos = search_str.find(search)) != npos) {
    newStr += search_str(0, pos);
    newStr += replacement;
    search_str += pos + search.length();
    if (limit != -1 && ++cnt == limit) break;
  }
  // Left over
  if (search_str.length())
    newStr += search_str;
  return newStr;
}

String String::subst(HashTable<String, String> hashes) const {
  String newStr(*this);
  Array<String> keys(hashes.keys());
  Array<String> values(hashes.values());
  for (size_t i = 0; i < keys.length(); ++i) {
    const String key(keys[i]), value(values[i]);
    //::printf("%s -> %s\n", key.c_str(), value.c_str());
    newStr = newStr.sub(key, value);
  }
  return newStr;
}

/**
 * @brief Splits off tokens
 * @param str The string to split a token off of
 * @param delim The delimiter to tokenize with
 * This will modify the given string to remove the first token, which is returned.
 * If no more tokens exist, the remaining string is returned and the string becomes "". Future calls will return a blank String("")
 */
String newsplit(String& str, char delim)
{
  if (!str.length()) return "";
  size_t pos = str.find(delim);
  if (pos == String::npos)
    pos = str.length();

  String first(str(0, pos));

  /* Trim out runs of whitespaces */
  if (delim == ' ') {
    while ((str.begin() + pos + 1) < str.end() && str[pos + 1] == ' ')
      ++pos;
  }

//  str = str(pos + 1, str.length() - pos + 1);
  str += pos + 1;
  return first;
}
BDLIB_NS_END
