/* String.h
 *
 * $Id$
 */
#ifndef _mSTRING_H
#define _mSTRING_H 1

#include <iostream>
//#include <vector>
#include <sys/types.h>
#include <cstring>
#include <algorithm> // min() / max()


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
        //static const size_t block_size() { return 16; };
        size_t len; //Length of string
        size_t size; //Capacity of buffer
        char *buf;//start
        unsigned short n; //References
        char sbuf[16];
    
        StringBuf() : len(0), size(0), buf(NULL), n(1) {};
        void Reserve(size_t);
        /**
         * @brief Allocates a buffer and returns it's address.
         * @param siz The number of bytes to allocate.
         * @post A new piece of memory is allocated.
         * @todo Implement mempool here.
        */
        char *AllocBuf(const size_t newSize) {
          if (newSize <= sizeof(sbuf)) return sbuf;
          else return new char[newSize];
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

        ///Return if the buffer is shared
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
	void doDetach() {
          --Ref->n;
          Ref = new StringBuf();
        }
  protected:
        StringBuf *Ref;

        /**
         * @brief Free up our reference if we had the last one.
         * @post The reference counter is decremented.
         * @post If this was the last Reference, it is free'd
         */
        void CheckDeallocRef() {
          /* Only deallocate the reference if we have the last pointer to it */
          if (--Ref->n < 1) {
            delete Ref;
          }
        }

        void Detach() {
          if (Ref->isShared()) {
            doDetach();
          } else
            Ref->len = 0;
        }
        void AboutToModify(size_t);
        ///This is set if the constructor was given a size.
  public:
        int rcount() const { return Ref->n; };
        /* Constructors */
        String() : Ref(new StringBuf()) {};

	String(const String& string) : Ref(string.Ref) { ++Ref->n; };
	/**
	 * @brief Create a String from a given cstring.
	 * @param cstring The null-terminated character array to create the object from.
	 * @post A StringBuf has been initialized.
	 * @post The buffer has been filled with the string.
	 * @test String test("Some string");
	*/
	String(const char *cstring) : Ref(new StringBuf()) { if (cstring) append(cstring); };

	/**
	 * @brief Create a String from a given cstring with the given strlen.
	 * @param cstring The null-terminated character array to create the object from.
	 * @param slen The length of the given string to use.
	 * @pre len > 0
	 * @post A StringBuf has been initialized.
	 * @post The buffer has been filled with the string (up to len characters).
	 * @test String test("Some string");
	*/
        String(const char *cstring, size_t slen) : Ref(new StringBuf()) { append(cstring, slen); };

	/**
	 * @brief Create a String from a given character.
	 * @param ch The character to create the string from.
	 * @post A stringBuf has been initialized.
	 * @post The buffer has been filled with the caracter.
	 * @test String test('a');
	*/
        String(const char ch) : Ref(new StringBuf()) { append(ch); };

	/**
	 * @brief Create an empty String container with at least the specified bytes in size.
	 * @param newSize Reserve at least this many bytes for this String.
	 * @post This string's memory will also never be shrunk.
	 * @post A buffer has been created.
	 * 
	 * The idea behind this is that if a specific size was asked for, the buffer is like
	 * a char buf[N];
	*/
        String(const int newSize) : Ref(new StringBuf()) {
          if (newSize <= 0) return;
          Reserve(newSize);
        };

	/**
	 * @brief String Destructor
	 * @post If the String's Reference is not shared, it is free'd.
	 * @post If the String's Reference IS shared, it is decremented and detached.
	*/
        virtual ~String() { CheckDeallocRef(); };
        
        const char* begin() const { return Ref->buf; };
        const char* end() const { return Ref->buf + length(); };


        //void assign(const String&);
        //void assign(const char *);
        //void assign(const char);
        
        /* Accessors */
        /**
         * @brief Returns length of the string.
         * @return Length of the string.
        */
        const size_t length() const { return Ref->len; };

        /**
         * @brief Returns capacity of the String object.
         * @return Capacity of the String object.
        */
        const size_t capacity() const { return Ref->size; };

	/**
	 * @brief Cstring accessor
	 * @return A null-terminated character array (cstring).
	 * @post The buffer size is (possibly) incremented by 1 for the '\0' character.
	 * @post There is a '\0' at the end of the buffer.
	 * @post The actual String size is unchanged.
	*/
        const char *c_str() {
          AboutToModify(length() + 1);
          Ref->buf[length()] = '\0';
          return Ref->buf;
        }

	/**
	 * @brief Data accessor
	 * @return Pointer to array of characters (not null-terminated).
	*/
        const char *data() const {
          return Ref->buf;
        }

        /**
         * @brief Returns the character at the given index.
         * @return The character at the given index.
         * @param i Index to return.
         * @pre The index must exist.
         * @todo Perhaps this should throw an exception if out of range?
         */
        const char charAt(unsigned int i) const { return hasIndex(i) ? Ref->buf[i] : 0; };

        /**
         * @brief Checks if the buffer has the given index or not.
         * @return Boolean true/false as to whether or not index exists.
         * @param i Index to check.
        */
        bool hasIndex(unsigned int i) const { return (i < length()); };

        String substring(int, size_t) const;

	/**
	 * @brief Compare our String object with another String object
	 * @param string The String object to compare to
	 * @return an integer less than, equal to, or greater than zero if our buffer is found, respectively, to be less than, to match, or be greater than str.
	 */
	int compare(const String& string) const { return compare(string, string.length()); };
        int compare(const String&, size_t) const;
//        const StringList split(const char);

        /* Setters */
        /**
         * @brief Appends one character to end of buffer.
         * @param ch The character to be appended.
         * @post The buffer is allocated.
         * @post The character is appended at the end of the buffer.
         */
        void append(const char ch) { insert(length(), ch); };

        /**
         * @brief Appends given cstring to end of buffer.
         * @param string The cstring to be appended.
         * @param n How many characters to copy from the string.
         * @post The buffer is allocated.
         */
        void append(const char *string, int n = -1) { insert(length(), string, n); };

        /**
         * @brief Appends given string to the end of buffer
         * @param string The string to be appended.
         * @param n How many characters to copy from the String object.
         * @post The buffer is allocated.
         */
        void append(const String& string, int n = -1) { insert(length(), string, n); };

        void insert(int, const char);
        void insert(int, const char *, int = -1);
        void insert(int, const String&, int = -1);

        void replace(int, const char);
        void replace(int, const char *, int = -1);
        void replace(int, const String&, int = -1);

        /**
         * \sa StringBuf::Reserve()
         * @post The String will also never shrink after this.
        */
        virtual void Reserve(const size_t newSize) {
          if (newSize > 0)
            Ref->Reserve(newSize);
        };

	virtual void printf(const char*, ...);
	const String encrypt(String);
	const String decrypt(String);
        const String base64Encode(void);
        const String base64Decode(void);


        /* Operators */

        /**
         * @sa c_str()
         */
        const char* operator * () { return c_str(); };

        bool operator ! () const { return length() == 0; };

        /**
         * @sa charAt()
         * Unlinke charAt() this is unchecked.
         */
        const char operator [] (int i) const { return Ref->buf[i]; };

        String& operator += (const char);
        String& operator += (const char *);
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
	virtual const String& operator = (const char *);
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
  return *this += 1;
}

inline const String String::operator ++ (int) //Postfix
{
  String tmp(*this);
  ++(*this);
  return tmp;
}

inline const String& String::operator -- () { //Prefix
  return *this -= 1;
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
inline String& String::operator += (const char *string) {
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
  int len = length() - n;
  replace(0, &Ref->buf[n], len);
  Ref->len = len;
  return *this;
}

inline String& String::operator -= (const int n) {
  AboutToModify(capacity());
  Ref->len = length() - n;
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
  for (const char *c = string.begin(); c != string.end(); ++c)
    os << *c;
  return os;
  //return os << string.c_str();
}

std::istream& operator >> (std::istream&, String&);
std::istream& getline(std::istream&, String&);

//std::ostream& operator << (std::ostream&, const std::vector<String>);
#endif /* !_mSTRING_H */
