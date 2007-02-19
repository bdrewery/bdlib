/* String.h
 *
 * $Id$
 */
#ifndef _mSTRING_H
#define _mSTRING_H 1

#include "bdlib.h"

#include <iostream>
#include <sys/types.h>
#include <algorithm> // min() / max()

#ifdef DEBUG
#include <stdio.h>
#endif

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
        size_t len; //Length of string
        size_t size; //Capacity of buffer
        char *buf;
        mutable uint8_t n; //References
        char sbuf[16];
    
        StringBuf() : len(0), size(0), buf(NULL), n(1) {};
        void Reserve(const size_t);
        /**
         * @brief Allocates a buffer and returns it's address.
         * @param bytes The number of bytes to allocate.
         * @post A new block of memory is allocated.
         * @todo Implement mempool here.
         */
        char *AllocBuf(const size_t bytes) {
          if (bytes <= sizeof(sbuf)) return sbuf;
          else return new char[bytes];
        }

        /**
         * @brief Free's up the allocated buffer.
         * @param p The buffer to be free'd
         * @post The buffer is deleted.
         * @todo Implement mempool here.
         */
        void FreeBuf(const char* p) {
          if (p != sbuf) delete[] p;
        }

        /**
          * @brief Is this string shared?
          */
        bool isShared() const { return n > 1; };
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
        }

        /**
          * @brief Set the lengths to the specified length
          * @param newLen the new length to set to
          */
        void setLength(size_t newLen) { Ref->len = sublen = newLen; };

        /**
          * @sa setLength()
          */
        void addLength(size_t diff) { Ref->len += diff; sublen += diff; };

        /**
          * @sa setLength()
          */
        void subLength(size_t diff) { Ref->len -= diff; sublen -= diff; };

        /**
          * @brief Mutable Ref->buf+offset reference for use internally
          */
        char* mdata() const { return Buf() + offset; };

        /**
          * @brief Mutable Ref->buf reference for use internally
          */
        char* Buf(int i = 0) const { return Ref->buf + i; };

        /**
          * @brief Ref->buf reference for use internally
          */
        const char* constBuf(int i = 0) const { return Buf(i); };

        /**
          * @brief Increment our reference counter.
          */
        uint8_t incRef() const { return ++Ref->n; };

        /**
          * @brief Decrement our reference counter.
          */
        uint8_t decRef() const { return --Ref->n; };

        /**
          * @return True if this string is shared; false if not.
          */
         bool isShared() const { return Ref->isShared(); };         
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
         * This is only called in ~String() and operator=(String &).
         * It checks whether of not this String was the last reference to the buffer, and if it was, it removes it.
         */
        void CheckDeallocRef() {
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
          } else
            setLength(0);
          offset = 0;
        }

        void AboutToModify(size_t);
  public:
        int rcount() const { return Ref->n; };

        /* Constructors */
        String() : Ref(new StringBuf()), offset(0), sublen(0) {};
	String(const String &string) : Ref(string.Ref), offset(string.offset), sublen(string.sublen) { incRef(); };
	/**
	 * @brief Create a String from a given cstring.
	 * @param cstring The null-terminated character array to create the object from.
	 * @post A StringBuf has been initialized.
	 * @post The buffer has been filled with the string.
	 * @test String test("Some string");
 	*/
	String(const char *cstring) : Ref(new StringBuf()), offset(0), sublen(0) { if (cstring) append(cstring); };

	/**
	 * @brief Create a String from a given cstring with the given strlen.
	 * @param cstring The null-terminated character array to create the object from.
	 * @param slen The length of the given string to use.
	 * @pre len > 0
	 * @post A StringBuf has been initialized.
	 * @post The buffer has been filled with the string (up to len characters).
	 * @test String test("Some string");
         */
        String(const char *cstring, size_t slen) : Ref(new StringBuf()), offset(0), sublen(0) { append(cstring, slen); };

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
        String(const int newSize) : Ref(new StringBuf()), offset(0), sublen(0) {
          if (newSize <= 0) return;
          Reserve(newSize);
        };

	/**
	 * @brief String Destructor
	 * @post If the String's Reference is not shared, it is free'd.
	 * @post If the String's Reference IS shared, it is decremented and detached.
	 */
        virtual ~String() { CheckDeallocRef(); };
        
        const char* begin() const { return data(); };
        const char* end() const { return begin() + length(); };


        /* Accessors */
        /**
         * @brief Returns length of the string.
         * @return Length of the string.
         */
        const size_t length() const { return sublen; };

        /**
         * @brief Returns capacity of the String object.
         * @return Capacity of the String object.
         */
        const size_t capacity() const { return Ref->size; };

        /**
          * @brief Check whether the string is 'empty'
          * @return True if empty, false if non-empty
          */
        bool isEmpty() const { return length() == 0; };
        /**
          * @sa isEmpty()
          * This is for: if (!string)
          * Having if(string) conflicts with another operator
          */
        bool operator ! () const { return isEmpty(); };

	/**
	 * @brief Cstring accessor
	 * @return A null-terminated character array (cstring).
	 * @post The buffer size is (possibly) incremented by 1 for the '\0' character.
	 * @post There is a '\0' at the end of the buffer.
	 * @post The actual String size is unchanged.
         * @todo Create ConstAboutToModify() const, this function really should be const.
	 */
        const char *c_str() {
          AboutToModify(length() + 1);
          Ref->buf[length()] = '\0';
          return data();
        }

        /**
         * @sa c_str()
         * @brief This is a cast operator to const char*
         * @todo this should be const too
         * This would be used in this situation: 
         * String string("blah");
         * const char *cstring = (const char *) string;
         */
        const char* operator * () { return c_str(); };


	/**
	 * @brief Data accessor
	 * @return Pointer to array of characters (not necesarily null-terminated).
	 */
        const char *data() const {
          return Ref->buf;
        }

        /**
         * @brief Checks if the buffer has the given index or not.
         * @return Boolean true/false as to whether or not index exists.
         * @param i Index to check.
        */
        bool hasIndex(int i) const { 
#ifdef DEBUG
        if (i < 0 || i > (int) length()) std::printf("ATTEMPT TO ACCESS INDEX %d/%d\n", i, length());
#endif
          return (i < (int) length()); 
        };

        /**
         * @sa charAt()
         * Unlinke charAt() this is unchecked.
         */
        const char operator[] (int i) const { 
#ifdef DEBUG
        if (i < 0 || i > (int) length()) std::printf("ATTEMPT TO ACCESS INDEX %d/%d\n", i, length());
#endif
          return Ref->buf[i]; 
        };

        /**
         * @brief Returns the character at the given index.
         * @return The character at the given index.
         * @param i Index to return.
         * @pre The index must exist.
         * @sa operator[]()
         * @todo Perhaps this should throw an exception if out of range?
         */
        const char charAt(int i) const { return hasIndex(i) ? (*this)[i] : 0; };

        String substring(int, size_t) const;

	/**
	 * @brief Compare our String object with another String object
	 * @param string The String object to compare to
	 * @return an integer less than, equal to, or greater than zero if our buffer is found, respectively, to be less than, to match, or be greater than str.
	 */
	int compare(const String &string) const { return compare(string, string.length()); };
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
        void append(const char ch) { insert(length(), ch); };

        /**
         * @brief Appends given cstring to end of buffer.
         * @param string The cstring to be appended.
         * @param n How many characters to copy from the string.
         * @post The buffer is allocated.
         * This is the same as inserting the string at the end of the buffer.
         */
        void append(const char *string, int n = -1) { insert(length(), string, n); };

        /**
         * @brief Appends given string to the end of buffer
         * @param string The string to be appended.
         * @param n How many characters to copy from the String object.
         * @post The buffer is allocated.
         * This is the same as inserting the string at the end of the buffer.
         */
        void append(const String &string, int n = -1) { insert(length(), string, n); };

        void insert(int, const char);
        void insert(int, const char *, int = -1);
        void insert(int, const String&, int = -1);

        void replace(int, const char);
        void replace(int, const char *, int = -1);
        void replace(int, const String&, int = -1);

        /**
         * @sa StringBuf::Reserve()
         * @post The String will also never shrink after this.
        */
        virtual void Reserve(const size_t newSize) {
          if (newSize > 0)
            Ref->Reserve(newSize);
        };

	virtual void printf(const char*, ...);
#ifdef DISABLED
	const String encrypt(String);
	const String decrypt(String);
#endif /* DISABLED */
        /**
          * @brief Encode the string into base64
          */
        const String base64Encode(void);
        /**
          * @brief Decode the string from base64
          */
        const String base64Decode(void);

        /* Operators */

        String &operator += (const char);
        String &operator += (const char *);
        String &operator += (const String&);
        String &operator += (int);
        String &operator -= (int);

        const String &operator ++ (); //Prefix
        const String operator ++ (int); //Postfix
        const String &operator -- (); //Prefix 
        const String operator -- (int); //Postfix
        //bool operator == (const String&) const;
        //bool operator != (const String&) const;
        //bool operator < (const String&) const;
        //bool operator <= (const String&) const;
        //bool operator > (const String&) const;
        //bool operator >= (const String&) const;
        //operator bool ();


        virtual const String &operator = (const char);
	virtual const String &operator = (const char *);
	const String &operator = (const String&);
        
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
          ::CPPUNIT_NS::Asserter::failNotEqual(expected.c_str(), actual.c_str(), sourceLine);
        }
#endif /* CPPUNIT_VERSION */
        
};

/**
 * @relates String
 * @brief Concatenates two string objects together.
 * @param string1 The LHS string.
 * @param string2 The RHS string.
 * @post A new string is allocated, reference copied and returned.
 * @return Returns a new string that can be reference copied by the lvalue.
 */
inline String operator + (const String &string1, const String &string2) {
  String temp(string1);
  temp += string2;
  return temp;
}

inline const String &String::operator ++ () { //Prefix
  return (*this) += 1;
}

inline const String String::operator ++ (int) //Postfix
{
  String tmp(*this);
  ++(*this);
  return tmp;
}

inline const String &String::operator -- () { //Prefix
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
inline String &String::operator += (const char ch) {
  append(ch);
  return *this;
}

/**
 * \sa append(const char*)
 */
inline String &String::operator += (const char *string) {
  append(string);
  return *this;
}

/**
 * \sa append(const String&)
 */
inline String &String::operator += (const String &string) {
  append(string);
  return *this;
}

inline String &String::operator += (const int n) {
  int len = length() - n;
  replace(0, &Ref->buf[n], len);
  setLength(len);
  return *this;
}

inline String &String::operator -= (const int n) {
  AboutToModify(capacity());
  subLength(n);
  return *this;
}



// comparison operators:
inline bool operator == (const String &lhs, const String &rhs) {
  return (lhs.compare(rhs) == 0);
}

inline bool operator != (const String &lhs, const String &rhs) {
  return ! (lhs == rhs);
}

inline bool operator <  (const String &lhs, const String &rhs) {
  return (lhs.compare(rhs) < 0);
}

inline bool operator <= (const String &lhs, const String &rhs) {
  return ! (rhs < lhs);
}

inline bool operator >  (const String &lhs, const String &rhs) {
  return (rhs < lhs);
}

inline bool operator >= (const String &lhs, const String &rhs) {
  return ! (lhs < rhs);
}

#ifdef no
//inline bool String::operator == (const String &rhs) const {
//  return (compare(rhs) == 0);
//}
inline bool operator == (const String &lhs, const String &rhs) {
  return (lhs.compare(rhs) == 0);
}

inline bool String::operator != (const String &rhs) const {
  return !(*this == rhs);
}

inline bool String::operator <  (const String &rhs) const {
  return (compare(rhs) < 0);
}

inline bool String::operator <= (const String &rhs) const {
  return !(rhs < *this);
}

inline bool String::operator >  (const String &rhs) const {
  return (rhs < *this);
}

inline bool String::operator >= (const String &rhs) const {
  return !(*this < rhs);
}
#endif
//inline String::operator bool () {
//  return length() == 0;
//}

inline std::ostream& operator << (std::ostream& os, const String &string) {
  for (const char *c = string.begin(); c != string.end(); ++c)
    os << *c;
  return os;
  //return os << string.c_str();
}

std::istream& operator >> (std::istream&, String&);
std::istream& getline(std::istream&, String&);

BDLIB_NS_END
//std::ostream& operator << (std::ostream&, const std::vector<String>);
#endif /* !_mSTRING_H */
