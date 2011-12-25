/* String.h
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
#ifndef _BD_STRING_H
#define _BD_STRING_H 1

#include "bdlib.h"
#include "HashTable.h"
#include "ReferenceCountedArray.h"
//#include "Array.h"

#include <stdint.h>
#include <iostream>
#include <sys/types.h>
#include <algorithm> // min() / max()
#include <cstring>

#include <stdio.h>

#ifdef CPPUNIT_VERSION
#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>
#define CPPUNIT_ASSERT_STRING_EQUAL(expected, actual) BDLIB_NS::String::checkStringEqual(expected, actual, CPPUNIT_SOURCELINE())
#endif /* CPPUNIT_VERSION */


BDLIB_NS_BEGIN
template <class T>
class Array;

typedef char String_Array_Type;

/**
 * @class String
 * @brief Provides custom string class for easy and optimized string manipulation.
 * @todo compute hash on insert, then use to compare instead of strcmp
 * @todo an updating hash as the copy is done.
 */
class String : public ReferenceCountedArray<String_Array_Type> {
  private:
        static unsigned char cleanse_ctr;

        /* Cleanse our buffer using OPENSSL_cleanse() */
        void cleanse() {
          const size_t len = capacity();
          char* ptr = const_cast<char*>(real_begin()); //real_begin() will ignore offset.

          unsigned char *p = reinterpret_cast<unsigned char *>(ptr);
          size_t loop = len;
          while(loop--)
          {
            *(p++) = static_cast<unsigned char>(cleanse_ctr);
            cleanse_ctr += (17 + (reinterpret_cast<size_t>(p) & 0xF));
          }
          if(memchr(ptr, cleanse_ctr, len))
            cleanse_ctr += (63 + reinterpret_cast<size_t>(p));
        }
  public:

        /* Constructors */
        String() : ReferenceCountedArray<String_Array_Type>() {};
	String(const String& string) : ReferenceCountedArray<String_Array_Type>(string) {};
#ifdef __GXX_EXPERIMENTAL_CXX0X__
	String(String&& string) : ReferenceCountedArray<String_Array_Type>(std::move(string)) {};
#endif
	/**
	 * @brief Create a String from a given cstring.
	 * @param cstring The null-terminated character array to create the object from.
	 * @post The buffer has been filled with the string.
	 * @test String test("Some string");
 	*/
	String(const char* cstring) : ReferenceCountedArray<String_Array_Type>() { if (cstring) append(cstring); };

	/**
	 * @brief Create a String from a given cstring with the given strlen.
	 * @param cstring The null-terminated character array to create the object from.
	 * @param slen The length of the given string to use.
	 * @pre len > 0
	 * @post The buffer has been filled with the string (up to len characters).
	 * @test String test("Some string");
         */
        String(const char* cstring, size_t slen) : ReferenceCountedArray<String_Array_Type>() { append(cstring, slen); };

	/**
	 * @brief Create a String from a given character.
	 * @param ch The character to create the string from.
	 * @post A stringBuf has been initialized.
	 * @post The buffer has been filled with the caracter.
	 * @test String test('a');
	 */
        String(const char ch) : ReferenceCountedArray<String_Array_Type>() { append(ch); };

	/**
	 * @brief Create an empty String container with at least the specified bytes in size.
	 * @param newSize Reserve at least this many bytes for this String.
	 * @post This string's memory will also never be shrunk.
	 * @post A buffer has been created.
	 * 
	 * The idea behind this is that if a specific size was asked for, the buffer is like
	 * a char buf[N];
         */
        explicit String(const size_t newSize) : ReferenceCountedArray<String_Array_Type>(newSize) {};
        String(const size_t newSize, const char value) : ReferenceCountedArray<String_Array_Type>(newSize, value) {};


        virtual ~String() {
          /* If deallocating the last reference, cleanse the string buffer with OPENSSL_cleanse() */
          if (rcount() == 1) {
            cleanse();
          }
        }

        /**
         * @brief Find a string in the string
         * @return The position of the string if found, or String::npos if not found
         **/
        size_t find(const String&) const;
        using ReferenceCountedArray<String_Array_Type>::find;

        /**
         * @brief Find a string, starting from the end of the string
         * @param str The string to look for
         * @param lpos The last position to consider when searching
         * @return The position of the string if found, or String::npos if not found
         * @sa find
         */
        size_t rfind(const String& str, const size_t lpos = 0) const;
        using ReferenceCountedArray<String_Array_Type>::rfind;

	/**
	 * @brief Cstring accessor
	 * @return A null-terminated character array (cstring).
	 * @post The buffer size is (possibly) incremented by 1 for the '\\0' character.
	 * @post There is a '\\0' at the end of the buffer.
	 * @post The actual String size is unchanged.
	 */
        const char* c_str() const {
          AboutToModify(length() + 1);
          *(Buf(length())) = '\0';
          return data();
        }

        /**
         * @brief Duplicate the string into a c-style NULL-terminated buffer which must be deleted
         */
        inline char* dup() const {
          char *ret = new char[length() + 1];
          std::copy(begin(), end(), ret);
          ret[length()] = '\0';
          return ret;
        }

        /**
         * @sa c_str()
         */
        inline const char* operator*() const { return c_str(); };

        /**
         * @sa c_str()
         * @brief This is a cast operator to const char*
         * This would be used in this situation: 
         * String string("blah");
         * const char* cstring = (const char*) string;
         */
        //inline operator const char* () const { return c_str(); };

	/**
	 * @brief Returns a new String containing integer copies of the receiver.
	 * @return a new String
	 * from Ruby
	 */
	String operator*(int) const;

        /**
         * @brief Trim off \\n,\\r,\\r\\n from end
         * @return The string, to allow for chaining
         */
        String& chomp();

        /**
         * @brief Trim off \\n,\\r,\\r\n from end
         * @return New string
         */
        String chomp() const { return String(*this).chomp(); }

        /**
         * @brief Trim off whitespace
         * @return The string, to allow for chaining
         */
        String& trim();

        /**
         * @brief Trim off whitespace
         * @return New string
         */
        String trim() const { return String(*this).trim(); }

        /**
         * @sa at
         */
        inline char charAt(size_t pos) const { return at(pos); };

        // Substrings
        /**
         * @sa ReferenceCountedArray::slice()
         */
        String substring(int start, int len = -1) const {
          String newString(*this);
          newString.slice(start, len);
          return newString;
        };

        /**
         * @sa substring
         */
        inline String operator()(int start, int len = -1) const { return substring(start, len); };

        /**
         * @brief Returns a 'Slice' class for safe (cow) writing into the array
         * @sa Slice
         * @param start Starting position
         * @param len How many items to use
         */
        inline Slice<String> operator()(int start, int len = -1) { return Slice<String>(*this, start, len); };

        /**
         * @brief Compare our String object with another String object, but only n characters
         * @param str The String object to compare to.
         * @param n The number of characters to compare.
         * @param start The index to start looking from
         * @return an integer less than, equal to, or greater than zero if our buffer is found, respectively, to be less than, to match, or be greater than str.
         */
        int compare(const String& str, size_t n = 0, size_t start = 0) const;

        Array<String> split(const String&, size_t limit = npos) const;

        /* Setters */
        /**
         * @brief Appends given cstring to end of buffer.
         * @param string The cstring to be appended.
         * @param n How many characters to copy from the string.
         * @post The buffer is allocated.
         * This is the same as inserting the string at the end of the buffer.
         */
        inline void append(const char* string, size_t n = npos) { insert(length(), string, n); };

        using ReferenceCountedArray<String_Array_Type>::append;

        void insert(size_t, const char*, size_t = npos);
        using ReferenceCountedArray<String_Array_Type>::insert;

        void replace(size_t, const char*, size_t = npos);
        using ReferenceCountedArray<String_Array_Type>::replace;

        /**
         * @brief Replace all occurances of the given search with the given replacement.
         * @param search The string to search for.
         * @param replacement The string to replace with
         * @param limit An optional limit to how many replacements to do
         * @return A new String is returned
         */
        String sub(const String& search, const String& replacement, int limit = -1) const;

        /**
         * @brief Return a new string with the values mapped from the given hash list
         * @param hashes The mapping to use
         * @return A new String is returned
         */
        String subst(HashTable<String, String> hashes) const;

        /**
         * @brief Return a new formatted string
         * @return A new String
         * @sa printf(3)
         */
#ifdef __GNUC__
        static String printf(const char*, ...)  __attribute__((format(printf, 1, 2)));
#else
	static String printf(const char*, ...);
#endif
        /* Operators */

        String& operator+=(const char);
        String& operator+=(const char*);
        String& operator+=(const String&);
        String& operator+=(const size_t);
        String& operator-=(const size_t);

        const String& operator++();
        const String operator++(int);
        const String& operator--();
        const String operator--(int);


        //using ReferenceCountedArray<String_Array_Type>::operator=;

        friend String operator+(String, const String&);
        friend bool operator==(const String&, const String&);
        friend bool operator!=(const String&, const String&);
        friend bool operator<(const String&, const String&);
        friend bool operator<=(const String&, const String&);
        friend bool operator>(const String&, const String&);
        friend bool operator>=(const String&, const String&);

        friend std::ostream& operator<<(std::ostream&, const String&);
        friend std::ostream& operator>>(std::ostream&, const String&);

#ifdef CPPUNIT_VERSION
        static void checkStringEqual(String expected, String actual, CPPUNIT_NS::SourceLine sourceLine) {
          if (expected == actual) return;
          ::CPPUNIT_NS::Asserter::failNotEqual(expected.c_str(), actual.c_str(), sourceLine);
        }
#endif /* CPPUNIT_VERSION */

};

template<typename T>
  struct Hash;

template<>
  struct Hash<String>
    {
          inline size_t operator()(const String& val) const { return val.hash(); }
    };
/**
 * @relates String
 * @brief Concatenates two string objects together.
 * @param string1 The LHS string.
 * @param string2 The RHS string.
 * @post A new string is allocated, reference copied and returned.
 * @return Returns a new string that can be reference copied by the lvalue.
 */
inline String operator+(String string1, const String& string2) {
  string1 += string2;
  return string1;
}

/**
 * @brief Prefix increment
 */
inline const String& String::operator++() {
  return (*this) += size_t(1);
}

/**
 * @brief Postfix increment
 */
inline const String String::operator++(int) {
  String tmp(*this);
  ++(*this);
  return tmp;
}

/**
 * @brief Prefix decrement
 */
inline const String& String::operator--() {
  return (*this) -= 1;
}

/**
 * @brief Postfix decrement
 */
inline const String String::operator--(int) {
  String tmp(*this);
  --(*this);
  return tmp;
}

// Setters

/* Operators */


/**
 * \sa append(const char)
 */
inline String& String::operator+=(const char ch) {
  append(ch);
  return *this;
}

/**
 * \sa append(const char*)
 */
inline String& String::operator+=(const char* string) {
  append(string);
  return *this;
}

/**
 * \sa append(const String&)
 */
inline String& String::operator+=(const String& string) {
  append(string);
  return *this;
}

inline String& String::operator+=(const size_t n) {
  if (!length())
    return *this;
  if (n > length()) {
    offset = length();
    setLength(0);
  } else {
    offset += n;
    subLength(n);
  }
  return *this;
}

inline String& String::operator-=(const size_t n) {
  if (!length())
    return *this;
  if (n > length()) {
    offset = length();
    setLength(0);
  } else
    subLength(n);
  return *this;
}



// comparison operators:
inline bool operator==(const String& lhs, const String& rhs) {
  return (lhs.compare(rhs) == 0);
}

inline bool operator!=(const String& lhs, const String& rhs) {
  return ! (lhs == rhs);
}

inline bool operator<(const String& lhs, const String& rhs) {
  return (lhs.compare(rhs) < 0);
}

inline bool operator<=(const String& lhs, const String& rhs) {
  return ! (rhs < lhs);
}

inline bool operator>(const String& lhs, const String& rhs) {
  return (rhs < lhs);
}

inline bool operator>=(const String& lhs, const String& rhs) {
  return ! (lhs < rhs);
}

#ifdef no
//inline bool String::operator==(const String& rhs) const {
//  return (compare(rhs) == 0);
//}
inline bool operator==(const String& lhs, const String& rhs) {
  return (lhs.compare(rhs) == 0);
}

inline bool String::operator!=(const String& rhs) const {
  return !(*this == rhs);
}

inline bool String::operator<(const String& rhs) const {
  return (compare(rhs) < 0);
}

inline bool String::operator<=(const String& rhs) const {
  return !(rhs < *this);
}

inline bool String::operator>(const String& rhs) const {
  return (rhs < *this);
}

inline bool String::operator>=(const String& rhs) const {
  return !(*this < rhs);
}
#endif

inline std::ostream& operator<<(std::ostream& os, const String& string) {
  for (const char* c = string.begin(); c != string.end(); ++c)
    os << *c;
  return os;
  //return os << string.c_str();
}

String newsplit(String& str, char delim = ' ');

std::istream& operator>>(std::istream&, String&);
std::istream& getline(std::istream&, String&);

BDLIB_NS_END
//std::ostream& operator << (std::ostream&, const std::vector<String>);
#endif /* _BD_STRING_H */
