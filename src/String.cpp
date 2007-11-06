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

#ifndef lint
static const char rcsid[] = "$Id$";
#endif

#include "String.h"
#include <stdarg.h>
#include "base64.h"
//#include <memory>
//#include <iostream>
//using namespace std;

BDLIB_NS_BEGIN

/**
 * @brief Ensure that the buffer capacity() is >= newSize; else grow/copy into larger buffer.
 * @param newSize A size that we need to Allocate the buffer to.
 * @pre newSize is > 0 (assumed as size_t is unsigned)
 * @post The buffer is at least nsize bytes long.
 * @post If the buffer had to grow, the old data was deep copied into the new buffer and the old deleted.
 */
void StringBuf::Reserve(const size_t newSize) const
{
  /* Don't new if we already have enough room! */
  if (size < newSize) { 
    size = std::max(size_t(size * 1.5), newSize);

    char *newbuf = AllocBuf(size);

    if (newbuf != buf) {
      std::copy(buf, buf + len, newbuf);
      FreeBuf(buf);
      buf = newbuf;
    }
  }
}

/**
 * @brief Ensure that our internal buffer is unshared.
 * @param n Create/Grow the buffer to this size.
 * @pre n is > 0; this is assumed due to size_t being unsigned though.
 * @post The internal buffer/data is unshared
 * @post The buffer is at least size n.
 * @post The buffer is deep copied to a new buffer.
 * 
 * Ensure that our internal buffer is unshared.
 * If needed, performs a deep copy into a new buffer (COW).
 * Also take a hint size n of the new string's size as to avoid needless copying/allocing.
 */
void String::COW(size_t n) const {
  const char *p = constBuf();
  size_t oldLength = length();
  size_t oldCapacity = capacity();

  doDetach();
  Reserve( std::max(oldCapacity, n) ); //Will set capacity()/size
  std::copy(p, p + oldLength, Buf());
  setLength(oldLength);
}

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
 * @brief Insert a character at the given index.
 * @param k The index to insert at.
 * @param ch The character to be inserted.
 * @post A buffer is allocated.
 * @post If the old buffer was too small, it is enlarged.
 * @post The character is inserted at the given index.
 */
void String::insert(int k, const char ch)
{
  if (k && !hasIndex(k-1)) return;

  AboutToModify(length() + 1);
  memmove(Ref->buf + k + 1, Ref->buf + k, length() - k);
  Ref->buf[k] = ch;
  addLength(1);
}

/**
 * @brief Replace the given index with the given character.
 * @param k The index to replace.
 * @param ch The character to replace with.
 * @post The given index has been replaced.
 * @post COW is done if needed.
 */
void String::replace(int k, const char ch) {
  if (k && !hasIndex(k-1)) return;

  getOwnCopy();
  Ref->buf[k] = ch;
}

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
  memmove(Ref->buf + k + slen, Ref->buf + k, length() - k);
  std::copy(string, string + slen, Ref->buf + k);
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
  std::copy(string, string + slen, Ref->buf + k);
  setLength(newlen);
}

/**
 * @brief Inserts a String object into our buffer
 * @param k The index to insert at.
 * @param string The string to insert.
 * @param n The length to insert.
 * @post The buffer contains n characters from string inserted at index k.
 */
void String::insert(int k, const String &string, int n) {
  if (n == 0) return;
  if (k && !hasIndex(k-1)) return;
  
  int slen = string.length();

  if (n == -1 || n > slen)
    n = slen;
  slen -= slen - n;
  AboutToModify(length() + slen);
  memmove(Ref->buf + k + slen, Ref->buf + k, length() - k);
  std::copy(string.begin(), string.begin() + slen, Ref->buf + k);
  addLength(slen);
}

/**
 * @brief Replaces n characters in our buffer at index k with the given String object
 * @param k The index to replace at.
 * @param string The String object to replace with.
 * @param n The number of characters to use for the replace.
 */
void String::replace(int k, const String &string, int n) {
  if (n == 0) return;
  if (k && !hasIndex(k-1)) return;

  int slen = string.length();
  
  if (n == -1 || n > slen)
    n = slen;
  slen -= slen - n;

  size_t newlen = k + slen;
  
  if (newlen >= length()) {
    AboutToModify(newlen);
  } else {
    newlen = length();
    getOwnCopy();
  }
  std::copy(string.begin(), string.begin() + slen, Ref->buf + k);
  setLength(newlen);
}

/**
 * @brief Sets our buffer to the given character.
 * @param ch The character to set our buffer to.
 * @post The old buffer (if we had one) is free'd.
 * @post A sufficiently sized new buffer is made with the character within.
 * @return The new string object.
 */
const String &String::operator=(const char ch) {
  Detach();
  append(ch);
  return *this;
}

/**
 * @brief Sets our buffer to the given string.
 * @param string The string to set our buffer to.
 * @post The old buffer (if we had one) is free'd.
 * @post A sufficiently sized new buffer is made with the string within.
 * @return The new string object.
 */
const String &String::operator=(const char *string) {
  Detach();
  append(string);
  return *this;
}

/**
 * @brief Sets our Reference to the given String reference.
 * @param string The String object to reference.
 * @post The old buffer (if we had one) is free'd.
 * @post Our Reference now points to the given String.
 * @post Our old string object has been deleted (disconnected).
 * @return The new string object.
 * This handles self-assignment just fine, checking for it explicitly would be ineffecient for most cases.
 */
const String &String::operator=(const String &string) {
  string.incRef();
  offset = string.offset;
  sublen = string.sublen;
  CheckDeallocRef();
  Ref = string.Ref;
  return *this;
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

#ifdef experimental
String String::substring(int k, size_t siz) const
{
  if (!hasIndex(k))
    return String("");
  if (k + siz >= length())
    return String("");
  /* Intialize a temp from the given substring */
//  String tmp(Ref->buf + k, Ref->buf + k + siz);
  String tmp(Ref->buf + k, siz);
  cout << "tmp: " << tmp << endl;
  return tmp;
}

const StringList String::split(const char delim) {
  char *p = Ref->buf, *pn = NULL;
  StringList list;
  
  list.delim(delim);
  do {
    pn = strchr(p, delim);
    if (!pn)
      pn = Ref->buf + length();
    list.append(p, pn);
    p = strchr(p, delim);
    if (!p || !*p)
      break;
    p++;
  } while (1);

  return list;
}

//LIST TEST
ostream& operator << (ostream& os, const vector<String> list) {
  for (unsigned int i=0; i < list.size(); ++i)
    os << list[i];
  return os;
}
#endif /* experimental */

void String::printf(const char* format, ...) {
  char va_out[1024] = "";
  va_list va;

  va_start(va, format);
  vsnprintf(va_out, sizeof(va_out), format, va);
  va_end(va);

  *this = va_out;
}

#ifdef DISABLED
const String String::encrypt(String key) {
  String old(*this);
  if (!key)
    return old;

  size_t len = length();

  char *bdata = (char*) encrypt_binary(key.c_str(), (unsigned char*) data(), &len);
  Detach();
  append(bdata, len);
  free(bdata);
  return old;
}

const String String::decrypt(String key) {
  String old(*this);
  if (!key)
    return old;

  size_t len = length();
  char *bdata = (char*) decrypt_binary(key.c_str(), (unsigned char*) data(), &len);
  Detach();
  append(bdata, len);
  free(bdata);
  return old;
}
#endif /* DISABLED */

const String String::base64Encode(void) {
  String old(*this);
  size_t len = length();
  char *p = b64enc((unsigned char*) data(), &len);
  Detach();
  append(p, len);
  free(p);
  return old;
}

const String String::base64Decode(void) {
  String old(*this);
  size_t len = length();
  char *p = b64dec((unsigned char*) data(), &len);
  Detach();
  append(p, len);
  free(p);
  return old;
}

BDLIB_NS_END
