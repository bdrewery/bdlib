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

/* Array.h
 */

#ifndef _BD_ARRAY_H
#define _BD_ARRAY_H 1

#include "bdlib.h"
#include "ReferenceCountedArray.h"
#include "String.h"

#include <functional>

#ifdef CPPUNIT_VERSION
#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>
#define CPPUNIT_ASSERT_ARRAY_EQUAL(expected, actual) expected.CPPUNIT_checkArrayEqual(actual, CPPUNIT_SOURCELINE())
#endif /* CPPUNIT_VERSION */

BDLIB_NS_BEGIN
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
    Array() noexcept = default;
    Array(const Array<value_type>& array) noexcept = default;
    Array(Array<value_type>&& array) noexcept = default;
    using ReferenceCountedArray<T>::ReferenceCountedArray;

    /**
     * @brief Create a Array from a given carray.
     * @param carray The null-terminated array to create the object from.
     * @post A ArrayBuf has been initialized.
     * @post The buffer has been filled with the array.
     * @test Array test("Some array");
     */
    Array(const_pointer carray) : Array() {
      if (carray == nullptr || *carray == '\0')
        return;
      while (*carray != '\0') {
        reserve(ReferenceCountedArray<T>::sublen + 1, 4);
        *(Buf(ReferenceCountedArray<T>::sublen++)) = *carray++;
      }
    };

    Array& operator=(const Array<value_type>& array)  noexcept = default;
    Array& operator=(Array<value_type>&& array) noexcept = default;
    using ReferenceCountedArray<T>::operator=;

    inline void push_front(const_reference item) { this->insert(size_t(0), item); };
    inline void push_front(value_type&& item) { this->insert(size_t(0), std::move(item)); };

    /**
     * @sa ReferenceCountedArray::push_back()
     */
    inline friend Array<value_type>& operator<<(Array<value_type>& array,
        const_reference item) {
      array.push_back(item);
      return array;
    }
    inline friend Array<value_type>& operator<<(Array<value_type>& array,
        value_type&& item) {
      array.push_back(std::move(item));
      return array;
    }

    /**
     * @brief Shift the array left, removing the first element.
     * @return The first element.
     */
    inline value_type shift() & {
      assert(!this->isEmpty());
      pointer item{this->Buf(0)};

      value_type temp(this->isShared() ? *item : std::move(*item));
      ++(this->offset);
      --(this->sublen);
      this->my_hash = 0;
      return std::move(temp);
    }

    inline value_type shift() && noexcept {
      assert(!this->isEmpty());
      assert(!this->isShared());
      return std::move(*(this->Buf(0)));
    }

    /**
     * @brief Pop a value off the end of the array
     * @return The last element.
     */
    inline value_type pop() & {
      assert(!this->isEmpty());
      value_type temp(this->isShared() ? this->back() : std::move(this->back()));
      this->pop_back();
      return std::move(temp);
    }

    inline value_type pop() && noexcept {
      assert(!this->isEmpty());
      assert(!this->isShared());
      return std::move(this->back());
    }

    /**
     * @sa pop()
     */
    inline friend Array<value_type>& operator>>(Array<value_type>& array,
        reference item) {
      item = std::move(array.pop());
      return array;
    }

    /**
     * @brief Join an array by a delimiter into a string
     */
    String join(const String& delim, bool quoted = false) const {
      if (!this->length()) return String();
      size_t str_size = 0, i;

      // Preallocate a fully sized String to avoid frequent resizing
      str_size += (this->length() * delim.length()) - 1;
      if (quoted)
	str_size += this->length() * 2;
      for (i = 0; i < this->length(); ++i)
	str_size += this->constBuf(i)->length();

      String str(str_size);

      for (i = 0; i < this->length(); ++i) {
        if (i)
          str += delim;
        if (quoted) {
          str += '"' + *(this->constBuf(i)) + '"';
        } else {
          str += *(this->constBuf(i));
        }
      }
      return str;
    }

    inline bool equals(const Array& array) const noexcept __attribute__((pure)) {
      return equals(array, array.length());
    }
    /**
     * @brief Compare our Array object with another Array object, but only n elements
     * @param array The Array object to equals to.
     * @param n The number of items to equals.
     * @return True if the number of elements are the same, and they all are equal.
     */
    bool equals(const Array& array, size_t n) const noexcept __attribute__((pure))
    {
      const size_t my_len = this->length();
      const bool same_length = (my_len == array.length());

      /* Same array? */
      if (this->constBuf() == array.constBuf() && same_length)
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

    inline friend bool operator==(const Array& lhs, const Array& rhs)
      noexcept __attribute__((pure)) {
      return lhs.size() == rhs.size() && lhs.equals(rhs);
    }
    inline friend bool operator!=(const Array& lhs, const Array& rhs)
      noexcept __attribute__((pure)) {
        return !(lhs == rhs);
    }

    // Subarrays
    /**
     * @sa ReferenceCountedArray::slice()
     */
    inline Array subarray(ssize_t start, ssize_t len = -1) const & noexcept {
      Array newArray(*this);
      newArray.slice(start, len);
      return newArray;
    };

    inline const Array operator()(ssize_t start, ssize_t len = -1) const noexcept { return subarray(start, len); };
    /**
     * @brief Returns a 'Slice' class for safe (cow) writing into the array
     * @sa Slice
     * @param start Starting position
     * @param len How many items to use
     */
    inline Slice<Array> operator()(ssize_t start, ssize_t len = -1) noexcept {
      return Slice<Array>(*this, start, len);
    }

#ifdef CPPUNIT_VERSION
    void CPPUNIT_checkArrayEqual(const Array& actual,
        CPPUNIT_NS::SourceLine sourceLine) const {
      if ((*this) == actual) return;
      ::CPPUNIT_NS::Asserter::failNotEqual(this->join("|").c_str(),
          actual.join("|").c_str(), sourceLine);
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
    inline friend Array operator+(Array array1, const Array& array2) {
      array1 += array2;
      return array1;
    }

    /**
     * @brief Prefix increment
     */
    inline const Array& operator++() & noexcept {
      return (*this) += 1;
    }

    /**
     * @brief Postfix increment
     */
    inline const Array operator++(int) & {
      Array tmp((*this)(0, 1));
      ++(this->offset);
      this->subLength(1);
      return tmp;
    }

    inline Array operator++(int) && {
      Array tmp{std::move(shift())};
      return std::move(tmp);
    }

    /**
     * @brief Prefix decrement
     */
    inline const Array& operator--() & noexcept {
      this->pop_back();
      return *this;
    }

    /**
     * @brief Postfix decrement
     */
    inline const Array operator--(int) & {
      Array tmp((*this)(this->length() - 1, 1));
      this->subLength(1);
      return tmp;
    }

    inline Array operator--(int) && {
      Array tmp{std::move(pop())};
      return std::move(tmp);
    }

    /**
     * \sa append(const char)
     */
    inline Array& operator+=(const_reference item) & {
      this->append(item);
      return *this;
    }

    /**
     * \sa append(const Array&)
     */
    inline Array& operator+=(const Array& array) & {
      this->append(array);
      return *this;
    }

    inline Array& operator+=(Array&& array) & {
      append(std::move(array));
      return *this;
    }

    inline Array& operator+=(const int n) & {
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

    inline Array& operator-=(const int n) & {
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
BDLIB_NS_END

namespace std {
  template<typename T>
  struct hash<BDLIB_NS::Array<T>> {
    inline size_t operator()(const BDLIB_NS::Array<T>& val) const noexcept {
      return val.hash();
    }
  };
}

#endif /* _BD_ARRAY_H */
/* vim: set sts=2 sw=2 ts=8 et: */
