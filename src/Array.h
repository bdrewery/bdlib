/* Array.h
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
#ifndef _BD_ARRAY_H
#define _BD_ARRAY_H 1

#include "bdlib.h"
#include "ReferenceCountedArray.h"
#include "String.h"
#include <stdio.h>

#ifdef CPPUNIT_VERSION
#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>
#define CPPUNIT_ASSERT_ARRAY_EQUAL(expected, actual) expected.CPPUNIT_checkArrayEqual(actual, CPPUNIT_SOURCELINE())
#endif /* CPPUNIT_VERSION */

BDLIB_NS_BEGIN
class String;
template <class T>
/**
 * @class Array
 * @brief Dynamic Array
 */
class Array : public ReferenceCountedArray<T> {
  public:
    typedef T value_type;

    typedef size_t             size_type;
    typedef std::ptrdiff_t     difference_type;
    typedef value_type*        pointer;
    typedef const value_type*  const_pointer;
    typedef value_type&        reference;
    typedef const value_type&  const_reference;

    /* Constructors */
    Array() : ReferenceCountedArray<value_type>() {};
    Array(const Array<value_type>& array) : ReferenceCountedArray<value_type>(array) {};
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    Array(Array<value_type>&& array) : ReferenceCountedArray<value_type>(array) {};
#endif
    /**
     * @brief Create a Array from a given carray.
     * @param carray The null-terminated array to create the object from.
     * @post A ArrayBuf has been initialized.
     * @post The buffer has been filled with the array.
     * @test Array test("Some array");
     */
    Array(const_pointer carray) : ReferenceCountedArray<value_type>() {
      size_t i = 0;
      while (1) {
        if (!carray[i]) break;
        push(carray[i]);
        ++i;
      }
    };

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    /**
     * @brief Create an array from an initializer list
     * @param list An initializer_list
     */
    Array(std::initializer_list<value_type> list) : ReferenceCountedArray<value_type>() {
      *this = list;
    }
#endif

    /**
     * @brief Create a Array from a given carray.
     * @param carray The null-terminated array to create the object from.
     * @param len How big is the carray?
     * @post A ArrayBuf has been initialized.
     * @post The buffer has been filled with the array.
     * @test Array test("Some array");
     */
    Array(const_pointer carray, size_t len) : ReferenceCountedArray<value_type>() {
      this->Reserve(len);
      for (size_t i = 0; i < len; ++i)
        push(carray[i]);
    };

    /**
     * @brief Create an empty Array container with at least the specified elements in size.
     * @param newSize Reserve at least this many buckets for this Array.
     * @post This array's memory will also never be shrunk.
     * @post A buffer has been created.
     */
    explicit Array(const size_type newSize) : ReferenceCountedArray<value_type>(newSize) {};
    Array(const size_type newSize, const value_type value) : ReferenceCountedArray<value_type>(newSize, value) {};

    virtual ~Array() {};

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    /**
     * @brief Create an array from an initializer list
     * @param list An initializer_list
     */
    Array& operator=(std::initializer_list<value_type> list) {
      this->clear();
      this->Reserve(list.size());
      for (value_type item : list) {
        push(std::move(item));
      }
      return *this;
    }
#endif
    using ReferenceCountedArray<value_type>::operator=;

    /**
     * @brief Add an item to the end of the array
     */
    inline void push(const value_type item) {
      this->AboutToModify(this->length() + 1);
      *(this->Buf(this->length())) = item;
      this->addLength(1);
    }

    /**
     * @sa push
     */
    inline friend Array<value_type>& operator<<(Array<value_type>& array, const_reference item) {
      array.push(item);
      return array;
    }

    /**
     * @brief Shift the array left, removing the first element.
     * @return The first element.
     */
    inline value_type shift() {
      if (this->isEmpty()) return value_type();

      value_type temp(*(this->Buf(0)));
      ++(this->offset);
      this->subLength(1);
      return temp;
    }

    /**
     * @brief Pop a value off the end of the array
     * @return The last element.
     */
    inline value_type pop() {
      if (this->isEmpty()) return value_type();

      value_type temp(*(this->Buf(this->length() - 1)));
      this->subLength(1);
      return temp;
    }

    /**
     * @sa pop
     */
    inline friend Array<value_type>& operator>>(Array<value_type>& array, reference item) {
      item = array.pop();
      return array;
    }

    /**
     * @brief Join an array by a delimiter into a string
     */
    String join(const String& delim, bool quoted = false) const {
      if (!this->length()) return String();
      String str;
      for (size_t i = 0; i < this->length(); ++i) {
        if (i)
          str += delim;
        if (quoted) {
          str += '"' + *(this->Buf(i)) + '"';
        } else {
          str += *(this->Buf(i));
        }
      }
      return str;
    }

    inline bool equals(const Array& array) const { return equals(array, array.length()); };
    /**
     * @brief Compare our Array object with another Array object, but only n elements
     * @param array The Array object to equals to.
     * @param n The number of items to equals.
     * @return True if the number of elements are the same, and they all are equal.
     */
    bool equals(const Array& array, size_t n) const
    {
      size_t my_len = this->length();
      bool same_length = (my_len == array.length());

      /* Same array? */
      if (this->data() == array.data() && same_length)
        return true;

      if (!same_length)
        return false;

      size_t slen = std::min(array.length(), n);
      size_t len = std::min(my_len, slen);

      for (size_t i = 0; i < len; ++i) {
        if ((*this)[i] != array[i])
          return false;
      }
      return true;
    }

    inline friend bool operator==(const Array& lhs, const Array& rhs) { return lhs.equals(rhs); };
    inline friend bool operator!=(const Array& lhs, const Array& rhs) { return !(lhs == rhs); };

    // Subarrays
    /**
     * @sa ReferenceCountedArray::slice()
     */
    inline Array subarray(int start, int len = -1) const {
      Array newArray(*this);
      newArray.slice(start, len);
      return newArray;
    };

    /**
     * @sa subarray
     */
    inline Array operator()(int start, int len = -1) const { return subarray(start, len); };

    /**
     * @brief Returns a 'Slice' class for safe (cow) writing into the array
     * @sa Slice
     * @param start Starting position
     * @param len How many items to use
     */
    inline Slice<Array> operator()(int start, int len = -1) { return Slice<Array>(*this, start, len); };

#ifdef CPPUNIT_VERSION
    void CPPUNIT_checkArrayEqual(Array actual, CPPUNIT_NS::SourceLine sourceLine) {
      if ((*this) == actual) return;
      ::CPPUNIT_NS::Asserter::failNotEqual(this->join("|").c_str(), actual.join("|").c_str(), sourceLine);
    }
#endif /* CPPUNIT_VERSION */

    /* Operators */
    /**
     * @relates Array
     * @brief Concatenates two array objects together.
     * @param array1 The LHS array.
     * @param array2 The RHS array.
     * @post A new array is allocated, reference copied and returned.
     * @return Returns a new array that can be reference copied by the lvalue.
     */
    inline friend Array operator+(const Array& array1, const Array& array2) {
      Array temp(array1);
      temp += array2;
      return temp;
    }

    /**
     * @brief Prefix increment
     */
    inline const Array& operator++() {
      return (*this) += 1;
    }

    /**
     * @brief Postfix increment
     */
    inline const Array operator++(int) {
      Array tmp((*this)(0, 1));
      ++(this->offset);
      this->subLength(1);
      return tmp;
    }

    /**
     * @brief Prefix decrement
     */
    inline const Array& operator--() {
      return (*this) -= 1;
    }

    /**
     * @brief Postfix decrement
     */
    inline const Array operator--(int) {
      Array tmp((*this)(this->length() - 1, 1));
      this->subLength(1);
      return tmp;
    }

    /**
     * \sa append(const char)
     */
    inline Array& operator+=(const_reference item) {
      this->append(item);
      return *this;
    }

    /**
     * \sa append(const Array&)
     */
    inline Array& operator+=(const Array& array) {
      this->append(array);
      return *this;
    }

    inline Array& operator+=(const int n) {
      if (!this->length())
        return *this;
      if (int(this->length()) - n < 0) {
        this->offset = this->length();
        this->setLength(0);
      } else {
        this->offset += n;
        this->subLength(n);
      }
      return *this;
    }

    inline Array& operator-=(const int n) {
      if (!this->length())
        return *this;
      if (int(this->length()) - n < 0) {
        this->offset = this->length();
        this->setLength(0);
      } else
        this->subLength(n);
      return *this;
    }

};

template<typename T>
  struct Hash;

template<typename T>
  struct Hash< Array<T> > {
    inline size_t operator()(const Array<T>& val) const { return val.hash(); }
  };
BDLIB_NS_END

#endif /* _BD_ARRAY_H */
