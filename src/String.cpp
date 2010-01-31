/* String.cpp
 *
 * Copyright (C) Bryan Drewery
 *
 * This program is private and may not be distributed, modified
 * or used without express permission of Bryan Drewery.
 *
 * THIS PROGRAM IS DISTRIBUTED WITHOUT ANY WARRANTY.
 * IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 * FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 * DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
 * IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
 * NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 */


#include "String.h"
#include "Array.h"
#include <stdarg.h>
#include <cstring>
using std::strlen;
using std::memmove;
#include "base64.h"
//#include <memory>
//#include <iostream>
//using namespace std;

BDLIB_NS_BEGIN

const size_t String::npos;
unsigned char String::cleanse_ctr = 0;

/* Accessors */
/**
 * @brief Compare our String object with another String object, but only n characters
 * @param str The String object to compare to.
 * @param n The number of characters to compare.
 * @return an integer less than, equal to, or greater than zero if our buffer is found, respectively, to be less than, to match, or be greater than str.
*/
int String::compare(const String &str, size_t n) const
{
  size_t my_len = length();
  size_t slen = std::min(str.length(), n);
  size_t len = std::min(my_len, slen);

  /* Same string? */
  if (data() == str.data() && my_len == str.length())
    return 0;

  for (size_t i = 0; i < len; ++i) {
    if ((*this)[i] < str[i])
      return -1;
    else if (str[i] < (*this)[i])
      return 1;
  }
  return my_len - slen;
}

/* Setters */

/**
 * @brief Insert a cstring at the given index.
 * @param string The cstring to be inserted.
 * @param k The index to insert at.
 * @param n Up to this many characters will be used from the string.
 * @post A buffer is allocated.
 * @post If the old buffer was too small, it is enlarged.
 * @post The string is inserted at the given index.
 */
void String::insert(int k, const char *string, int n)
{
  if (n == 0) return;
  if (k && !hasIndex(k-1)) return;
  
  size_t slen = (n == -1) ? strlen(string) : (size_t) n;

  AboutToModify(length() + slen);
  memmove(Buf() + k + slen, Buf() + k, length() - k);
  std::copy(string, string + slen, Buf() + k);
  addLength(slen);
}

/**
 * @brief Replaces the given index with the given cstring up to the given amount of characters.
 * @param k The index to replace at.
 * @param string The cstring to replace with.
 * @param n How many characters to use from string.
 */
void String::replace(int k, const char *string, int n)
{
  if (n == 0) return;
  if (k && !hasIndex(k-1)) return;

  size_t slen = (n == -1) ? strlen(string) : (size_t) n;
  size_t newlen = k + slen;
  
  if (newlen >= length()) {
    AboutToModify(newlen);
  } else {
    newlen = length();
    getOwnCopy();
  }
  std::copy(string, string + slen, Buf() + k);
  setLength(newlen);
}

std::istream& operator >> (std::istream& is, String &string) {
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

std::istream& getline(std::istream& is, String &string) {   
 
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

  Array<String> array;
  // Use a temporary - is a fast reference and never modified, so no COW ever used.
  String str(*this);
  size_t pos = 0;

  while (array.size() < limit - 1) {
    // Trim out left whitespace
    if (delim == ' ') while (str.length() && str[0] == ' ') ++str;

    // All done when there's nothing left
    if (!str) break;

    if ((pos = str.find(delim)) == npos)
      pos = str.length();
    array << str(0, pos);
    str += int(pos + delim.length());
  }

  // Add on extra
  if (limit != npos) {
    // Trim out left whitespace
    if (delim == ' ') while (str.length() && str[0] == ' ') ++str;
    array << str;
  }

  return array;
}

#ifdef experimental
//LIST TEST
ostream& operator << (ostream& os, const vector<String> list) {
  for (unsigned int i=0; i < list.size(); ++i)
    os << list[i];
  return os;
}
#endif /* experimental */

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

  return (*this = String(va_out, len));
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

size_t String::find (const String& str) const {
  for (const char* c = begin(); c != end(); ++c)
    if (strncmp(c, str.c_str(), std::min(str.length(), size_t(end() - c))) == 0)
      return size_t(c - begin());
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
    search_str += int(pos) + int(search.length());
    if (limit != -1 && ++cnt == limit) break;
  }
  // Left over
  if (search_str.length())
    newStr += search_str;
  return newStr;
}

#ifdef no
String String::subst(HashTable<String, String> hashes) const {
  hashes.each(lame_string_subst, (void*)this);
}
#endif

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
  str += int(pos) + 1;
  return first;
}
BDLIB_NS_END
