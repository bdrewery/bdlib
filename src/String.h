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

/* String.h
 */

#ifndef _BD_STRING_H
#define _BD_STRING_H 1

#include "bdlib.h"
#include "ReferenceCountedArray.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <algorithm> // min() / max()
#include <functional>

#ifdef CPPUNIT_VERSION
#include <string>
#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>
#define CPPUNIT_ASSERT_STRING_EQUAL(expected, actual, ...) \
  CPPUNIT_ASSERT_EQUAL(\
      static_cast<const BDLIB_NS::String>(expected), \
      static_cast<const BDLIB_NS::String>(actual), \
      ##__VA_ARGS__);
#endif /* CPPUNIT_VERSION */

BDLIB_NS_BEGIN
template <class T>
class Array;

template <typename, typename>
class HashTable;

typedef char String_Array_Type;

/**
 * @class String
 * @brief Provides custom string class for easy and optimized string manipulation.
 */
class String : public ReferenceCountedArray<String_Array_Type> {
  private:
        typedef std::allocator<String_Array_Type> Allocator;
        static unsigned char cleanse_ctr;

        /* Cleanse our buffer using OPENSSL_cleanse() */
        void cleanse() noexcept {
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
        String(const Allocator& allocator = Allocator()) noexcept :
          ReferenceCountedArray<String_Array_Type, Allocator>(allocator) {};
	String(const String& string) noexcept = default;
	String(String&& string) noexcept = default;
        explicit String(const std::string& str) : String(str.data(), str.length()) {};
        String(std::initializer_list<value_type> list) :
          ReferenceCountedArray<String_Array_Type, Allocator>(list) {};

	/**
	 * @brief Create a String from a given cstring.
	 * @param cstring The null-terminated character array to create the object from.
	 * @post The buffer has been filled with the string.
	 * @test String test("Some string");
 	*/
	String(const char* cstring, const Allocator& allocator = Allocator()) : String(allocator) {
          if (cstring == nullptr || *cstring == '\0')
            return;
          while (*cstring != '\0') {
            ReferenceCountedArray::reserve(sublen + 1, 4);
            *(Buf(sublen++)) = *cstring++;
          }
          ReferenceCountedArray::reserve(sublen + 1);
          *(Buf(sublen)) = '\0';
        }

	/**
	 * @brief Create a String from a given cstring with the given strlen.
	 * @param cstring The null-terminated character array to create the object from.
	 * @param slen The length of the given string to use.
	 * @pre len > 0
	 * @post The buffer has been filled with the string (up to len characters).
	 * @test String test("Some string");
         */
        String(const char* cstring, size_t slen) :
          ReferenceCountedArray<String_Array_Type, Allocator>(slen+1) {
          std::copy(cstring, cstring + slen, Buf());
          *(Buf(slen)) = '\0';
          sublen = slen;
        };

	/**
	 * @brief Create a String from a given character.
	 * @param ch The character to create the string from.
	 * @post A stringBuf has been initialized.
	 * @post The buffer has been filled with the caracter.
	 * @test String test('a');
	 */
        String(const char ch, const Allocator& allocator = Allocator()) : String(allocator) {
          ReferenceCountedArray::reserve(length() + 2);
          *(Buf(sublen++)) = ch;
          *(Buf(sublen)) = '\0';
        }

        void reserve(const size_t newSize,
            double scaling_factor = 1) const override {
          const size_t realSize = newSize > 0 ? newSize + 1 : newSize;
          ReferenceCountedArray::reserve(realSize, scaling_factor);
          if (constBuf() != nullptr) {
            *Buf(capacity() - offset - 1) = '\0';
          }
        }

	/**
	 * @brief Create an empty String container with at least the specified bytes in size.
	 * @param newSize reserve at least this many bytes for this String.
	 * @post This string's memory will also never be shrunk.
	 * @post A buffer has been created.
	 * 
	 * The idea behind this is that if a specific size was asked for, the buffer is like
	 * a char buf[N];
         */
        explicit String(const size_t newSize,
            const Allocator& allocator = Allocator()) :
          ReferenceCountedArray<String_Array_Type, Allocator>(newSize,
              allocator) {};
        String(const size_t newSize, const char value,
            const Allocator& allocator = Allocator()) :
          ReferenceCountedArray<String_Array_Type, Allocator>(newSize, value,
              allocator) {};

        ~String() {
          /* If deallocating the last reference, cleanse the string buffer with OPENSSL_cleanse() */
          if (rcount() == 1) {
            cleanse();
          }
        }

        String& operator=(const String& string) noexcept = default;
        String& operator=(String&& string) noexcept = default;

  private:
        size_t _find(const String& str, size_t pos) const noexcept __attribute__((pure));
  public:
        /**
         * @brief Find a string in the string
         * @param str The substring to look for
         * @return The position of the string if found, or String::npos if not found
         **/
        size_t find(const String& str) const noexcept __attribute__((pure)) {
          return _find(str, 0);
        }
        /**
         * @brief Find a string in the string
         * @param str The substring to look for
     * @param pos Where to start the search
         * @return The position of the string if found, or String::npos if not found
         **/
        size_t find(const String& str, size_t pos) const __attribute__((pure)) {
          if (pos != 0)
            validateIndex(pos - 1);
          return _find(str, pos);
        }
        using ReferenceCountedArray<String_Array_Type, Allocator>::find;

        /**
         * @brief Find a string, starting from the end of the string
         * @param str The string to look for
         * @param lpos The last position to consider when searching
         * @return The position of the string if found, or String::npos if not found
         * @sa find
         */
        size_t rfind(const String& str, const size_t lpos = 0) const
          noexcept __attribute__((pure));
        using ReferenceCountedArray<String_Array_Type, Allocator>::rfind;

        /**
         * @brief Find a string, starting from the end of the string, ignoring case
         * @param str The string to look for
         * @param lpos The last position to consider when searching
         * @return The position of the string if found, or String::npos if not found
         * @sa ifind
         */
        size_t rifind(const String& str, const size_t lpos = 0) const
          __attribute__((pure));

        /**
         * @brief Find a string in the string, ignoring case
         * @param str The substring to look for
         * @return The position of the string if found, or String::npos if not found
         **/
        size_t ifind(const String& str) const __attribute__((pure));

	/**
	 * @brief Cstring accessor
	 * @return A null-terminated character array (cstring).
	 * @post The buffer size is (possibly) incremented by 1 for the '\\0' character.
	 * @post There is a '\\0' at the end of the buffer.
	 * @post The actual String size is unchanged.
	 */
        const char* c_str() const noexcept {
          if (length() == 0)
            return "";
          assert(constBuf() != nullptr);
          assert(length() > 0);
          if (*constBuf(length()) != '\0') {
            assert(sublen > 0);
            AboutToModify(length() + 1);
            *(Buf(length())) = '\0';
          } else
            assert(*constBuf(length()) == '\0');
          return cbegin();
        }

        /**
         * @brief Duplicate the string into a c-style NULL-terminated buffer which must be deleted
         */
        inline char* dup() const {
          char *ret = new char[length() + 1];
          std::copy(cbegin(), cend(), ret);
          ret[length()] = '\0';
          return ret;
        }

        explicit operator std::string() const {
          return std::string(constBuf(), length());
        }

        /**
         * @brief Copy the contents of the string to the given cstring ptr.
         * @param dst The destination cstring ptr;
         * @param n The number of characters to copy out
         */
        size_t copy(char* dst, size_t n = npos) const noexcept;

        /**
         * @sa c_str()
         */
        inline const char* operator*() const noexcept { return c_str(); };

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
        String& chomp() noexcept;

        /**
         * @brief Trim off \\n,\\r,\\r\n from end
         * @return New string
         */
        String chomp() const { return String(*this).chomp(); }

        /**
         * @brief Trim off whitespace
         * @return The string, to allow for chaining
         */
        String& trim() noexcept;

        /**
         * @brief Trim off whitespace
         * @return New string
         */
        String trim() const { return String(*this).trim(); }

        /**
         * @sa at
         */
        inline char charAt(size_t pos) const {
          return at(pos);
        }

        // Substrings
        /**
         * @sa ReferenceCountedArray::slice()
         */
        inline String substring(ssize_t start, ssize_t len = -1) const & noexcept {
          String newString(*this);
          newString.slice(start, len);
          return newString;
        };

        inline const String operator()(ssize_t start, ssize_t len = -1) const noexcept { return substring(start, len); };
        /**
         * @brief Returns a 'Slice' class for safe (cow) writing into the array
         * @sa Slice
         * @param start Starting position
         * @param len How many items to use
         */
        inline Slice<String> operator()(ssize_t start, ssize_t len = -1) noexcept {
          return Slice<String>(*this, start, len);
        }

        /**
         * @brief Compare our String object with another String object, but only n characters
         * @param str The String object to compare to.
         * @param n The number of characters to compare.
         * @return an integer less than, equal to, or greater than zero if our buffer is found, respectively, to be less than, to match, or be greater than str.
         */
        int compare(const String& str, size_t n) const
          noexcept __attribute__((pure));

        /**
         * @brief Compare our String object with another String object
         * @param str The String object to compare to.
         * @return an integer less than, equal to, or greater than zero if our buffer is found, respectively, to be less than, to match, or be greater than str.
         */
        int compare(const String& str) const
          noexcept __attribute__((pure));

        inline int compare(const char* rhs, size_t n = npos) const
          noexcept __attribute__((pure)) {
          return strncmp(c_str(), rhs, n);
        }

        Array<String> split(const String&, size_t limit = npos) const;

        /* Setters */
        /**
         * @brief Appends given cstring to end of buffer.
         * @param string The cstring to be appended.
         * @param n How many characters to copy from the string.
         * @post The buffer is allocated.
         * This is the same as inserting the string at the end of the buffer.
         */
        inline void append(const char* string, size_t n = npos) {
          insert(length(), string, n);
        }

        using ReferenceCountedArray<String_Array_Type, Allocator>::append;

        void insert(size_t, const char*, size_t = npos);
        using ReferenceCountedArray<String_Array_Type, Allocator>::insert;

        void replace(size_t, const char*, size_t = npos);
        using ReferenceCountedArray<String_Array_Type, Allocator>::replace;

        /**
         * @brief Replace all occurances of the given search with the given replacement.
         * @param search The string to search for.
         * @param replacement The string to replace with
         * @param limit An optional limit to how many replacements to do
         * @return A new String is returned
         */
        String sub(const String& search, const String& replacement,
            int limit = -1) const;

        /**
         * @brief Return a new string with the values mapped from the given hash list
         * @param hashes The mapping to use
         * @return A new String is returned
         */
        String subst(const HashTable<String, String>& hashes) const;

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

        /**
         * \sa append(const char)
         */
        inline String& operator+=(const char ch) & {
          append(ch);
          return *this;
        }

        /**
         * \sa append(const char*)
         */
        inline String& operator+=(const char* string) & {
          append(string);
          return *this;
        }

        /**
         * \sa append(const String&)
         */
        inline String& operator+=(const String& string) & {
          append(string);
          return *this;
        }
        inline String& operator+=(String&& string) & {
          append(std::move(string));
          return *this;
        }

        inline String& operator+=(const size_t n) & {
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

        inline String& operator-=(const size_t n) & {
          if (!length())
            return *this;
          if (n > length()) {
            offset = length();
            setLength(0);
          } else
            subLength(n);
          return *this;
        }

        /**
         * @brief Prefix increment
         */
        inline const String& operator++() & noexcept {
          return (*this) += size_t(1);
        }

        /**
         * @brief Postfix increment
         */
        inline const String operator++(int) & {
          String tmp(*this);
          ++(*this);
          return tmp;
        }

        /**
         * @brief Prefix decrement
         */
        inline const String& operator--() & noexcept {
          return (*this) -= 1;
        }

        /**
         * @brief Postfix decrement
         */
        inline const String operator--(int) & {
          String tmp(*this);
          --(*this);
          return tmp;
        }

        //using ReferenceCountedArray<String_Array_Type, Allocator>::operator=;

        friend String operator+(String, const String&);
        friend bool operator==(const String&, const String&) noexcept;
        friend bool operator==(const String&, const char*) noexcept;
        friend bool operator!=(const String&, const String&) noexcept;
        friend bool operator!=(const String&, const char*) noexcept;
        friend bool operator<(const String&, const String&) noexcept;
        friend bool operator<(const String&, const char*) noexcept;
        friend bool operator<=(const String&, const String&) noexcept;
        friend bool operator<=(const String&, const char *) noexcept;
        friend bool operator>(const String&, const String&) noexcept;
        friend bool operator>(const String&, const char*) noexcept;
        friend bool operator>=(const String&, const String&) noexcept;
        friend bool operator>=(const String&, const char*) noexcept;

        friend std::ostream& operator<<(std::ostream&, const String&);
        friend std::ostream& operator<<(std::ostream&, String&&);
        friend std::ostream& operator>>(std::ostream&, const String&);
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

// comparison operators:
inline bool __attribute__((pure))
operator==(const String& lhs, const String& rhs) noexcept {
  return (lhs.length() == rhs.length() &&
      lhs.compare(rhs) == 0);
}
inline bool __attribute__((pure))
operator==(const String& lhs, const char* rhs) noexcept {
  return lhs.compare(rhs) == 0;
}

inline bool __attribute__((pure))
operator!=(const String& lhs, const String& rhs) noexcept {
  return ! (lhs == rhs);
}

inline bool __attribute__((pure))
operator!=(const String& lhs, const char* rhs) noexcept {
  return ! (lhs == rhs);
}

inline bool __attribute__((pure))
operator<(const String& lhs, const String& rhs) noexcept {
  return (lhs.compare(rhs) < 0);
}

inline bool __attribute__((pure))
operator<(const String& lhs, const char* rhs) noexcept {
  return (lhs.compare(rhs) < 0);
}

inline bool __attribute__((pure))
operator<=(const String& lhs, const String& rhs) noexcept {
  return ! (rhs < lhs);
}

inline bool __attribute__((pure))
operator<=(const String& lhs, const char * rhs) noexcept {
  return ! (rhs < lhs);
}

inline bool __attribute__((pure))
operator>(const String& lhs, const String& rhs) noexcept {
  return (rhs < lhs);
}

inline bool __attribute__((pure))
operator>(const String& lhs, const char* rhs) noexcept {
  return (lhs.compare(rhs) > 0);
}

inline bool __attribute__((pure))
operator>=(const String& lhs, const String& rhs) noexcept {
  return ! (lhs < rhs);
}

inline bool __attribute__((pure))
operator>=(const String& lhs, const char* rhs) noexcept {
  return ! (lhs < rhs);
}

inline std::ostream& operator<<(std::ostream& os, const String& string) {
  for (const char* c = string.cbegin(); c != string.cend(); ++c)
    os << *c;
  return os;
  //return os << string.c_str();
}

inline std::ostream& operator<<(std::ostream& os, String&& string) {
  assert(!string.isShared());
  for (const char* c = string.cbegin(); c != string.cend(); ++c)
    os << std::move(*c);
  return os;
}

String newsplit(String& str, char delim = ' ');

std::istream& operator>>(std::istream&, String&);
std::istream& getline(std::istream&, String&);

BDLIB_NS_END

#ifdef CPPUNIT_VERSION
CPPUNIT_NS_BEGIN
template <>
struct assertion_traits<BDLIB_NS::String>
{
    static bool equal(const BDLIB_NS::String& x, const BDLIB_NS::String& y)
    {
        return x == y;
    }

    static bool less(const BDLIB_NS::String& x, const BDLIB_NS::String& y)
    {
        return x < y;
    }

    static bool lessEqual(const BDLIB_NS::String& x, const BDLIB_NS::String& y)
    {
        return x <= y;
    }

    static std::string toString(const BDLIB_NS::String& x)
    {
      std::string ret(x);
      return ret;
    }
};
CPPUNIT_NS_END
#endif


namespace std {
  template<>
  struct hash<BDLIB_NS::String>
    {
          inline size_t operator()(const BDLIB_NS::String& val) const noexcept {
            return val.hash();
          }
    };
}
//std::ostream& operator << (std::ostream&, const std::vector<String>);
#endif /* _BD_STRING_H */
/* vim: set sts=2 sw=2 ts=8 et: */
