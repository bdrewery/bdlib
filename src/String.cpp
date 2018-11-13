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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTIRBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
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
#include <cstdio>
#include "base64.h"
//#include <memory>
//#include <iostream>

BDLIB_NS_BEGIN

unsigned char String::cleanse_ctr = 0;

/* Accessors */
int String::compare(const String& str) const noexcept
{
  const auto mylen = length();
  const auto slen = str.length();
  /* Same string? */
  if (cbegin() == str.cbegin() && mylen == slen)
    return 0;

  const auto len = std::min(mylen, slen);
  const auto diff = std::memcmp(cbegin(), str.cbegin(), len);
  if (diff)
    return diff;
  else
    return mylen - slen;
}

int String::compare(const String& str, size_t n) const noexcept
{
  if (n == 0)
    return 0;
  const auto mylen = length();
  /* Same string? */
  if (cbegin() == str.cbegin() && mylen == str.length())
    return 0;

  const auto slen = n ? std::min(str.length(), n) : str.length();
  const auto len = std::min(mylen, slen);
  const auto diff = std::memcmp(cbegin(), str.cbegin(), len);
  if (diff)
    return diff;
  else if (n)
    return std::min(mylen, n) - slen;
  else
    return mylen - slen;
}

/* Setters */
size_t String::copy(char *dst, size_t n) const noexcept
{
  const auto slen = std::min(n, length());

  ::memcpy(dst, cbegin(), slen);

  return slen;
}

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
  if (pos != 0)
    validateIndex(pos - 1);
  
  const auto slen = (n == npos) ? std::strlen(string) : n;

  AboutToModify(length() + slen);
  /* Shift right */
  std::move_backward(constBuf(pos), constBuf(length()), Buf(length() + slen));
  sublen += slen;
  ::memcpy(Buf(pos), string, slen);
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
  if (pos != 0)
    validateIndex(pos - 1);

  const auto slen = (n == npos) ? std::strlen(string) : n;
  auto newlen = pos + slen;

  if (newlen < length())
    newlen = length();
  AboutToModify(newlen);
  sublen = newlen;
  ::memcpy(Buf(pos), string, slen);
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
 
  char ch = 0;
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
    return Array<String>({*this});
  }

#ifndef NDEBUG
  auto ref = rcount();
#endif
  const String space(' ');
  Array<String> array;
  auto it = cbegin();

  while (array.size() < limit - 1) {
    size_type pos;

    // Trim out left whitespace
    if (delim == space) while (it < cend() && *it == ' ') ++it;

    // All done when there's nothing left
    if (it >= cend()) break;

    if ((pos = _find(delim, it - cbegin())) == npos)
      pos = cend() - it;
    array << (*this)(it - cbegin(), pos);
    assert(++ref);
    it += pos + delim.length();
    assert(it - delim.length() <= cend());
  }

  // Add on extra
  if (limit != npos) {
    // Trim out left whitespace
    if (delim == space) while (it < cend() && *it == ' ') ++it;
    array << (*this)(it - cbegin());
    assert(++ref);
  }
  /*
   * Our String should not be modified.
   * We should only be returning reference slices.
   */
  assert(ref == rcount());

  return array;
}

String String::operator*(int times) const {
  String newString((this->length() * times));
  for (auto i = 0; i < times; ++i)
    newString += *this;
  return newString;
}

String String::printf(const char* format, ...) {
  char va_out[4096] = "";
  va_list va;

  va_start(va, format);
  const auto len = vsnprintf(va_out, sizeof(va_out), format, va);
  va_end(va);

  return String(va_out, len);
}

String& String::chomp() noexcept {
  if (length() && (*this)[length() - 1] == '\n') --(*this);
  if (length() && (*this)[length() - 1] == '\r') --(*this);
  return *this;
}

String& String::trim() noexcept {
  // ltrim
  while (length() && isspace((*this)[0])) { ++offset; subLength(1); }
  // rtrim
  while (length() && isspace((*this)[length() - 1])) subLength(1);
  return *this;
}

size_t String::_find(const String& str, size_t pos) const noexcept {
  if (str.length() == 0)
    return 0;
  const auto mylen = length() - pos;
#ifdef HAVE_MEMMEM
  const void *p = ::memmem(constBuf(pos), mylen, str.cbegin(), str.length());

  if (p == NULL)
    return npos;
  return static_cast<const char*>(p) - constBuf(pos);
#else
  if (mylen >= str.length()) {
    const auto last_pos = mylen - str.length();
    for (auto i = pos; i <= last_pos; ++i)
      if (str[0] == (*this)[i] && !std::memcmp(cbegin() + i, str.cbegin(),
            std::min(str.length(), mylen - i)))
        return i;
  }
  return npos;
#endif
}

size_t String::ifind(const String& str) const {
  String str_upper(str.length()), this_upper(length());
  std::transform(str.cbegin(), str.cend(), std::back_inserter(str_upper), ::toupper);
  std::transform(this->cbegin(), this->cend(), std::back_inserter(this_upper), ::toupper);
  return this_upper.find(str_upper);
}

size_t String::rifind(const String& str, const size_t lpos) const {
  String str_upper(str.length()), this_upper(length());
  std::transform(str.cbegin(), str.cend(), std::back_inserter(str_upper), ::toupper);
  std::transform(this->cbegin(), this->cend(), std::back_inserter(this_upper), ::toupper);
  return this_upper.rfind(str_upper);
}

size_t String::rfind(const String& str, const size_t lpos) const noexcept {
  if (str.length() == 0)
    return length() - 1;
  if (length() >= str.length()) {
    const auto last_pos = length() - str.length();
    for (auto pos = last_pos; pos + 1 > lpos; --pos)
      if (str[0] == (*this)[pos] && !std::memcmp(cbegin() + pos, str.cbegin(),
            std::min(str.length(), length() - pos)))
        return pos;
  }
  return npos;
}

String String::sub(const String& search, const String& replacement, int limit) const {
  String newStr(length()), search_str(*this);
  size_type pos = 0;
  auto cnt = 0;
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

String String::subst(const HashTable<String, String>& hashes) const {
  String newStr(*this);
  for (const auto& kv : hashes) {
    newStr = newStr.sub(kv.first, kv.second);
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
  if (!str.length()) return String();
  auto pos = str.find(delim);
  if (pos == String::npos)
    pos = str.length();

  String first(str(0, pos));

  /* Trim out runs of whitespaces */
  if (delim == ' ') {
    while ((str.cbegin() + pos + 1) < str.cend() && str[pos + 1] == ' ')
      ++pos;
  }

//  str = str(pos + 1, str.length() - pos + 1);
  str += pos + 1;
  return first;
}
BDLIB_NS_END
/* vim: set sts=2 sw=2 ts=8 et: */
