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
 * $Id: $
 */
#ifndef _W_ARRAY_H
#define _W_ARRAY_H 1

#include "bdlib.h"
#include "ReferenceCountedArray.h"
#include <stdio.h>

BDLIB_NS_BEGIN
template <class T>
/**
 * @class Array
 * @brief
 */
class Array : public ReferenceCountedArray<T> {
  public:
//    friend class ArrayIterator<T>;
//    friend class ArrayConstIterator<T>;

//    typedef ArrayIterator<T> iterator;
//    typedef ArrayConstIterator<T> const_iterator;
    typedef T value_type;

    typedef size_t             size_type;
    typedef ptrdiff_t          difference_type;
    typedef value_type*        pointer;
    typedef const value_type*  const_pointer;
    typedef value_type&        reference;
    typedef const value_type&  const_reference;

    /* Constructors */
    Array() : ReferenceCountedArray<value_type>() {};
    Array(const Array<value_type>& array) : ReferenceCountedArray<value_type>(array) {};
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

    /**
     * @brief Create a Array from a given carray.
     * @param carray The null-terminated array to create the object from.
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

    virtual ~Array() {};

    virtual size_t hash() const {return 0;};

    /*
     * @brief Add an item to the end of the array
     */
    void push(const value_type item) {
      AboutToModify(this->length() + 1);
      *(Buf(this->length())) = item;
      this->addLength(1);
    }

    /**
     * @sa push
     */
    inline friend Array<value_type>& operator<< (Array<value_type>& array, const_reference item) {
      array.push(item);
      return array;
    }

    /*
     * @brief Shift a value off the end of the array
     * @return the item
     */
    value_type pop() {
      if (this->isEmpty()) return value_type();

      value_type temp(*(Buf(this->length() - 1)));
      AboutToModify(this->length() - 1);
      this->subLength(1);
      return temp;
    }

    /**
     * @sa pop
     */
    inline friend Array<value_type>& operator>> (Array<value_type>& array, reference item) {
      item = array.pop();
      return array;
    }
};
BDLIB_NS_END

#endif /* !_W_ARRAY_H */
