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

#include <stdint.h>
#include <iostream>
#include <sys/types.h>
#include <algorithm> // min() / max()

#include <stdio.h>

#ifdef CPPUNIT_VERSION
#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>
#define CPPUNIT_ASSERT_STRING_EQUAL(expected, actual) BDLIB_NS::String::checkStringEqual(expected, actual, CPPUNIT_SOURCELINE())
#endif /* CPPUNIT_VERSION */


BDLIB_NS_BEGIN

class String;
class StringBuf;

/**
  * @class StringBuf
  * @brief Helps the String class with reference counting
  * @todo look into something like a string object made of string pointers, and << displays all of the pointed to objects. (this would save copying to construct immuatable concatenated strings, but would only work with the << stream
  */

class StringBuf {
  public:
        ~StringBuf() { FreeBuf(buf); };
        mutable size_t len; //Length of string
        mutable size_t size; //Capacity of buffer
        mutable char* buf;
        mutable uint8_t n; //References
        mutable char sbuf[16];
    
        StringBuf() : len(0), size(0), buf(NULL), n(1) {};
        void Reserve(const size_t) const;
        /**
         * @brief Allocates a buffer and returns it's address.
         * @param bytes The number of bytes to allocate.
         * @post A new block of memory is allocated.
         * @todo Implement mempool here.
         */
        inline char* AllocBuf(const size_t bytes) const {
          if (bytes <= sizeof(sbuf)) return sbuf;
          else return new char[bytes];
        }

        /**
         * @brief Free's up the allocated buffer.
         * @param p The buffer to be free'd
         * @post The buffer is deleted.
         * @todo Implement mempool here.
         */
        inline void FreeBuf(const char* p) const {
          if (p != sbuf) delete[] p;
        }

        /**
          * @brief Is this string shared?
          */
        inline bool isShared() const { return n > 1; };
private:
        // No copying allowed
        StringBuf(const StringBuf&); ///<Block implicit copy constructor
        StringBuf& operator=(const StringBuf&); ///<Block implicit copy constructor
};

/**
 * @class String
 * @brief Provides custom string class for easy and optimized string manipulation.
 * @todo compute hash on insert, then use to compare instead of strcmp
 * @todo an updating hash as the copy is done.
 */
class String {
  private:
        /**
         * @class Cref
         * @brief Safe element reading and writing.
         * @todo This should not provide copy constructors for Cref, they shouldn't be needed because of const char String::operator[]
         * This class should be optimized away and fully inlined such that:
         * String s;
         * s[0] = 'a';
         * Should be rewritten as:
         * s.write(0, 'a');
         */
        class Cref {
            friend class String;
            String& s;
            int k;

            /**
              * @brief Used by String::Cref operator[]
              */
            Cref(String& string, int i) : s(string), k(i) {};
            Cref(); //Not defined - never used

          public:            
            Cref(const Cref& cref) : s(cref.s), k(cref.k) {};
            inline Cref& operator=(const Cref& cref) {
              (*this) = (char) cref;
              return (*this);
            }

          public:
            /**
             * @sa char String::operator[]
             */
            inline operator char() const { return s.read(k); };

            /**
             * Stroustrup shows using this as void with no return value, but that breaks chaining a[n] = b[n] = 'b';
             */
            inline Cref& operator=(char c) { 
              s.write(k, c); 
              return (*this);
            };
        };

        /**
         * @class SubString
         * @brief Safe substring reading and writing.
         * @todo This should not provide copy constructors for Cref, they shouldn't be needed because of const char String::operator[]
         * This class should be optimized away and fully inlined such that:
         * String s("look over there");
         * s(0, 4) = "blah"';
         * Should be rewritten as:
         * s.replace(0, "blah", 4);
         */
         class SubString {
            friend class String;
            String &s;
            int start;
            int len;

            SubString(String& string, int _start, int _len) : s(string), start(_start), len(_len) {};
            SubString();

          public:
            SubString(const SubString& substring) : s(substring.s), start(substring.start), len(substring.len) {};

            inline SubString& operator= (const SubString& substring) {
              (*this) = (String) substring;
              return (*this);
            }


            /*
             * @brief return a new (const) substring
             */
            inline operator String() const { return s.substring(start, len); };

            /**
             * @todo This needs to account for negative start/len
             */
            inline SubString& operator= (const String& string) {
              s.replace(start, string, len);
              return (*this);
            }

            inline SubString& operator= (const char* string) {
              s.replace(start, string, len);
              return (*this);
            }
        };


        /* Most of these are helper abstractions in case I chose to change the reference implementation
         * ie, into the first element of the buffer
         */

        /**
         * @brief Detach from the shared reference.
         * This is only called when losing the old buffer or when modifying the buffer (and copy-on-write is used)
         */
	void doDetach() const {
          decRef();
          Ref = new StringBuf();
          sublen = 0;
          offset = 0;
        }

        /**
          * @brief Set the lengths to the specified length
          * @param newLen the new length to set to
          */
        inline void setLength(size_t newLen) const { Ref->len = sublen = newLen; };

        /**
          * @sa setLength()
          */
        inline void addLength(size_t diff) const { Ref->len += diff; sublen += diff; };

        /**
          * @sa setLength()
          */
        inline void subLength(size_t diff) const { Ref->len -= diff; sublen -= diff; };

        /**
          * @brief Mutable Ref->buf+offset reference for use internally
          */
//        char* mdata() const { return Buf() + offset; };

        /**
          * @brief Mutable Ref->buf reference for use internally
          */
        inline char* Buf(int i = 0) const { return Ref->buf + offset + i; };

        /**
          * @brief Ref->buf reference for use internally
          */
        inline const char* constBuf(int i = 0) const { return Buf(i); };

        /**
          * @brief Increment our reference counter.
          */
        inline uint8_t incRef() const { return ++Ref->n; };

        /**
          * @brief Decrement our reference counter.
          */
        inline uint8_t decRef() const { return --Ref->n; };

        /**
          * @return True if this string is shared; false if not.
          */
        inline bool isShared() const { return Ref->isShared(); };
  protected:
        /**
          * @brief The string reference for reference counting 
          * This is mutable so that Ref->n can be modified, which really is mutable
          */
        mutable StringBuf *Ref;
        
        /**
          * This is for substrings: so we know where the substring starts.
          */
        mutable size_t offset;
        /**
          * This is for substrings: so we know where the substring ends.
          */
        mutable size_t sublen;

        /**
         * @brief Free up our reference if we have the last one.
         * @post The reference counter is decremented.
         * @post If this was the last Reference, it is free'd
         * This is only called in ~String() and operator=(String&).
         * It checks whether of not this String was the last reference to the buffer, and if it was, it removes it.
         */
        inline void CheckDeallocRef() {
          if (decRef() < 1)
            delete Ref;
        }

        /**
          * @brief Detach from the reference
          * This is called when the old buffer is no longer needed for this String.
          * ie, operator=() was called.
          */
        void Detach() {
          if (isShared()) {
            doDetach();
          } else {
            setLength(0);
            offset = 0;
          }
        }

        void COW(size_t) const;

        inline void AboutToModify(size_t n) const {
          if (isShared())
            COW(n);
          else
            Reserve(n);
        }
        inline void getOwnCopy() const { AboutToModify(capacity()); };
  public:
        inline int rcount() const { return Ref->n; };

        /* Constructors */
        String() : Ref(new StringBuf()), offset(0), sublen(0) {};
	String(const String& string) : Ref(string.Ref), offset(string.offset), sublen(string.sublen) { incRef(); };
	/**
	 * @brief Create a String from a given cstring.
	 * @param cstring The null-terminated character array to create the object from.
	 * @post A StringBuf has been initialized.
	 * @post The buffer has been filled with the string.
	 * @test String test("Some string");
 	*/
	String(const char* cstring) : Ref(new StringBuf()), offset(0), sublen(0) { if (cstring) append(cstring); };

	/**
	 * @brief Create a String from a given cstring with the given strlen.
	 * @param cstring The null-terminated character array to create the object from.
	 * @param slen The length of the given string to use.
	 * @pre len > 0
	 * @post A StringBuf has been initialized.
	 * @post The buffer has been filled with the string (up to len characters).
	 * @test String test("Some string");
         */
        String(const char* cstring, size_t slen) : Ref(new StringBuf()), offset(0), sublen(0) { append(cstring, slen); };

	/**
	 * @brief Create a String from a given character.
	 * @param ch The character to create the string from.
	 * @post A stringBuf has been initialized.
	 * @post The buffer has been filled with the caracter.
	 * @test String test('a');
	 */
        String(const char ch) : Ref(new StringBuf()), offset(0), sublen(0) { append(ch); };

	/**
	 * @brief Create an empty String container with at least the specified bytes in size.
	 * @param newSize Reserve at least this many bytes for this String.
	 * @post This string's memory will also never be shrunk.
	 * @post A buffer has been created.
	 * 
	 * The idea behind this is that if a specific size was asked for, the buffer is like
	 * a char buf[N];
         */
        explicit String(const size_t newSize) : Ref(new StringBuf()), offset(0), sublen(0) {
          if (newSize <= 0) return;
          Reserve(newSize);
        };

	/**
	 * @brief String Destructor
	 * @post If the String's Reference is not shared, it is free'd.
	 * @post If the String's Reference IS shared, it is decremented and detached.
	 */
        virtual ~String() { CheckDeallocRef(); };

        inline const char* begin() const { return data(); };
        inline const char* end() const { return begin() + length(); };

        /**
         * @brief Clear contents of String and set length to 0
         */
        inline void clear() { Detach(); };


        /* Accessors */
        /**
         * @brief Returns length of the string.
         * @return Length of the string.
         */
        inline size_t length() const { return sublen; };

        /**
         * @brief Returns capacity of the String object.
         * @return Capacity of the String object.
         */
        inline size_t capacity() const { return Ref->size; };

        /**
          * @brief Check whether the string is 'empty'
          * @return True if empty, false if non-empty
          */
        inline bool isEmpty() const { return length() == 0; };
        /**
          * @sa isEmpty()
          * This is for: if (!string)
          * Having if(string) conflicts with another operator
          */
        inline bool operator ! () const { return isEmpty(); };

	/**
	 * @brief Cstring accessor
	 * @return A null-terminated character array (cstring).
	 * @post The buffer size is (possibly) incremented by 1 for the '\0' character.
	 * @post There is a '\0' at the end of the buffer.
	 * @post The actual String size is unchanged.
	 */
        const char* c_str() const {
          AboutToModify(length() + 1);
          Ref->buf[length()] = '\0';
          return data();
        }

        /**
         * @sa c_str()
         */
        inline const char* operator * () const { return c_str(); };

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
	String operator * (int) const;

	/**
	 * @brief Data accessor
	 * @return Pointer to array of characters (not necesarily null-terminated).
	 */
        inline const char* data() const { return Ref->buf + offset; }

        /**
         * @brief Checks if the buffer has the given index or not.
         * @return Boolean true/false as to whether or not index exists.
         * @param i Index to check.
        */
        inline bool hasIndex(int i) const { 
#ifdef DEBUG
        if (i < 0 || i >= (int) (offset + length())) ::printf("ATTEMPT TO ACCESS INDEX %d/%zu\n", i, size_t(offset + length()));
#endif
          return (i < (int) length()); 
        };

        /**
         * @sa charAt()
         * Unlinke charAt() this is unchecked.
         */
        inline char read(int i) const { return *(constBuf(i)); };

        inline void write(int i, char c) {
          getOwnCopy();
          *(Buf(i)) = c;
        };

        /**
         * @brief Trim off \n,\r,\r\n from end
         * @return The string, to allow for chaining
         */
        String chomp();

        /**
         * @brief Trim off \n,\r,\r\n from end
         * @return New string
         */
        String chomp() const { return String(*this).chomp(); }

        /**
         * @brief Safe element access operator
         * @todo This is only called on a (const) String, but should for a String as well.
         */
        inline char operator [](int i) const { return read(i); };

        /**
         * @brief Returns 'Cref' class for safe (cow) writing into String.
         * @sa Cref
         */ 
        inline Cref operator [](int i) { return Cref(*this, i); };

        /**
         * @brief Returns the character at the given index.
         * @return The character at the given index.
         * @param i Index to return.
         * @pre The index must exist.
         * @sa operator[]()
         * @todo Perhaps this should throw an exception if out of range?
         */
        inline char charAt(int i) const { return hasIndex(i) ? (*this)[i] : 0; };

        String substring(int, int) const;

        /**
         * @brief Returns a 'SubString' class for safe (cow) writing into String
         * @sa SubString
         */
        inline SubString operator()(int start, int len) { return SubString(*this, start, len); };
        /**
         * @brief Returns a const substring
         * @sa SubString
         */
        inline String operator()(int start, int len) const { return substring(start, len); };

        size_t hash() const;

        /**
	 * @brief Compare our String object with another String object
	 * @param string The String object to compare to
	 * @return an integer less than, equal to, or greater than zero if our buffer is found, respectively, to be less than, to match, or be greater than str.
	 */
	inline int compare(const String& string) const { return compare(string, string.length()); };
        int compare(const String&, size_t) const;
//        const StringList split(const char);

        /* Setters */
        /**
         * @brief Appends one character to end of buffer.
         * @param ch The character to be appended.
         * @post The buffer is allocated.
         * @post The character is appended at the end of the buffer.
         * This is the same as inserting the character at the end of the buffer.
         */
        inline void append(const char ch) { insert(length(), ch); };

        /**
         * @brief Appends given cstring to end of buffer.
         * @param string The cstring to be appended.
         * @param n How many characters to copy from the string.
         * @post The buffer is allocated.
         * This is the same as inserting the string at the end of the buffer.
         */
        inline void append(const char* string, int n = -1) { insert(length(), string, n); };

        /**
         * @brief Appends given string to the end of buffer
         * @param string The string to be appended.
         * @param n How many characters to copy from the String object.
         * @post The buffer is allocated.
         * This is the same as inserting the string at the end of the buffer.
         */
        inline void append(const String& string, int n = -1) { insert(length(), string, n); };

        void insert(int, const char);
        void insert(int, const char*, int = -1);
        void insert(int, const String&, int = -1);

        void replace(int, const char);
        void replace(int, const char*, int = -1);
        void replace(int, const String&, int = -1);

        /**
         * @sa StringBuf::Reserve()
         * @post The String will also never shrink after this.
        */
        virtual void Reserve(const size_t newSize) const { Ref->Reserve(newSize); };

#ifdef __GNUC__
        /* GNU GCC DOC: 
           Since non-static C++ methods have an implicit this argument, the arguments of such methods 
           should be counted from two, not one, when giving values for string-index and first-to-check.
         */
	virtual String printf(const char*, ...)  __attribute__((format(printf, 2, 3)));
#else
	virtual String printf(const char*, ...);
#endif
        /* Operators */

        String& operator += (const char);
        String& operator += (const char*);
        String& operator += (const String&);
        String& operator += (int);
        String& operator -= (int);

        const String& operator ++ (); //Prefix
        const String operator ++ (int); //Postfix
        const String& operator -- (); //Prefix 
        const String operator -- (int); //Postfix
        //bool operator == (const String&) const;
        //bool operator != (const String&) const;
        //bool operator < (const String&) const;
        //bool operator <= (const String&) const;
        //bool operator > (const String&) const;
        //bool operator >= (const String&) const;
        //operator bool ();


        virtual const String& operator = (const char);
	virtual const String& operator = (const char*);
	const String& operator = (const String&);

        friend String operator + (const String&, const String&);
        friend bool operator == (const String&, const String&);
        friend bool operator != (const String&, const String&);
        friend bool operator < (const String&, const String&);
        friend bool operator <= (const String&, const String&);
        friend bool operator > (const String&, const String&);
        friend bool operator >= (const String&, const String&);

        friend std::ostream& operator << (std::ostream&, const String&);
        friend std::ostream& operator >> (std::ostream&, const String&);

#ifdef CPPUNIT_VERSION
        static void checkStringEqual(String expected, String actual, CPPUNIT_NS::SourceLine sourceLine) {
          if (expected == actual) return;
//          std::cout << "'" << expected << "':" << expected.length() << " == " << "'" << actual << "':" << actual.length() << std::endl;
          ::CPPUNIT_NS::Asserter::failNotEqual(expected.c_str(), actual.c_str(), sourceLine);
        }
#endif /* CPPUNIT_VERSION */

};

template<typename T>
  struct hash;

template<>
  struct hash<String>
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
inline String operator + (const String& string1, const String& string2) {
  String temp(string1);
  temp += string2;
  return temp;
}

inline const String& String::operator ++ () { //Prefix
  return (*this) += 1;
}

inline const String String::operator ++ (int) //Postfix
{
  String tmp(*this);
  ++(*this);
  return tmp;
}

inline const String& String::operator -- () { //Prefix
  return (*this) -= 1;
}

inline const String String::operator -- (int) //Postfix
{
  String tmp(*this);
  --(*this);
  return tmp;
}

// Setters

/* Operators */


/**
 * \sa append(const char)
 */
inline String& String::operator += (const char ch) {
  append(ch);
  return *this;
}

/**
 * \sa append(const char*)
 */
inline String& String::operator += (const char* string) {
  append(string);
  return *this;
}

/**
 * \sa append(const String&)
 */
inline String& String::operator += (const String& string) {
  append(string);
  return *this;
}

inline String& String::operator += (const int n) {
  if (!length())
    return *this;
  int len = length() - n;
  replace(0, &Ref->buf[n], len);
  setLength(len);
  return *this;
}

inline String& String::operator -= (const int n) {
  if (!length())
    return *this;
  getOwnCopy();
  subLength(n);
  return *this;
}



// comparison operators:
inline bool operator == (const String& lhs, const String& rhs) {
  return (lhs.compare(rhs) == 0);
}

inline bool operator != (const String& lhs, const String& rhs) {
  return ! (lhs == rhs);
}

inline bool operator <  (const String& lhs, const String& rhs) {
  return (lhs.compare(rhs) < 0);
}

inline bool operator <= (const String& lhs, const String& rhs) {
  return ! (rhs < lhs);
}

inline bool operator >  (const String& lhs, const String& rhs) {
  return (rhs < lhs);
}

inline bool operator >= (const String& lhs, const String& rhs) {
  return ! (lhs < rhs);
}

#ifdef no
//inline bool String::operator == (const String& rhs) const {
//  return (compare(rhs) == 0);
//}
inline bool operator == (const String& lhs, const String& rhs) {
  return (lhs.compare(rhs) == 0);
}

inline bool String::operator != (const String& rhs) const {
  return !(*this == rhs);
}

inline bool String::operator <  (const String& rhs) const {
  return (compare(rhs) < 0);
}

inline bool String::operator <= (const String& rhs) const {
  return !(rhs < *this);
}

inline bool String::operator >  (const String& rhs) const {
  return (rhs < *this);
}

inline bool String::operator >= (const String& rhs) const {
  return !(*this < rhs);
}
#endif
//inline String::operator bool () {
//  return length() == 0;
//}

inline std::ostream& operator << (std::ostream& os, const String& string) {
  for (const char* c = string.begin(); c != string.end(); ++c)
    os << *c;
  return os;
  //return os << string.c_str();
}

std::istream& operator >> (std::istream&, String&);
std::istream& getline(std::istream&, String&);

BDLIB_NS_END
//std::ostream& operator << (std::ostream&, const std::vector<String>);
#endif /* !_BD_STRING_H */

