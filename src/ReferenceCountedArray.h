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

/* ReferenceCountedArray.h
 */

#ifndef _BD_REFERENCE_COUNTED_ARRAY_H
#define _BD_REFERENCE_COUNTED_ARRAY_H 1

#include "bdlib.h"
#include <algorithm>
#include <atomic>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <sys/types.h>
#include <cstring>

BDLIB_NS_BEGIN
template <class T, class Allocator = std::allocator<T> >
/**
 * @class ArrayRef
 * @brief Helps the String and Array classes with reference counting
 * @todo look into something like a string object made of string pointers, and << displays all of the pointed to objects. (this would save copying to construct immuatable concatenated strings, but would only work with the << stream
 */
class ArrayRef {
  public:
    typedef typename Allocator::pointer iterator;
    mutable Allocator alloc;
    mutable size_t size; //Capacity of buffer
    union {
      mutable iterator buf;
      mutable T buf_local[16];
    };
    mutable std::atomic<int> refs; //References

    ArrayRef(const Allocator& allocator = Allocator()) :
      alloc(allocator), size(0), buf(nullptr), refs(1) {};
    ~ArrayRef() {
      FreeBuf(buf);
    };
    /**
     * @brief Ensure that the buffer capacity() is >= newSize; else grow/copy into larger buffer.
     * @param newSize A size that we need to Allocate the buffer to.
     * @param scaling_factor How much to multiply the size by to help avoid later resizing
     * @param offset The offset of the old buffer so we know where to start
     * @param sublen The length of the subarray in use
     * @pre newSize is > 0 (assumed as size_t is unsigned)
     * @post The buffer is at least nsize bytes long.
     * @post If the buffer had to grow, the old data was deep copied into the new buffer and the old deleted.
     */
    void Reserve(size_t newSize, double scaling_factor, size_t& offset,
        size_t sublen) const
    {
      /* Don't new if we already have enough room! */
      if (size < newSize) {
        newSize = std::max(size_t(size * scaling_factor), newSize);

        iterator newbuf = alloc.allocate(newSize, buf);

        if (newbuf != buf) {
          // Initialize new memory
          std::uninitialized_fill(newbuf, newbuf + newSize, T());
          if (buf) {
            /* Copy old buffer into new - only copy the subarray */
            std::copy(buf + offset, buf + offset + sublen, newbuf);
            FreeBuf(buf);
          }
          buf = newbuf;
          size = newSize;
          offset = 0;
        }
      } else if ((size - offset) < newSize) {
        // There's enough room in the current buffer, but we're offsetted/shifted to a point where there's no room left
        // Shift everything to the beginning and reset the offset
        /* Only copy the subarray */
        std::memmove(static_cast<void*>(buf), static_cast<void*>(buf + offset),
            sublen);
        offset = 0;
      }
    }

    /**
     * @brief Free's up the allocated buffer.
     * @param p The buffer to be free'd
     * @post The buffer is deleted.
     * @todo Implement mempool here.
     */
    inline void FreeBuf(iterator p) const {
      for (iterator i = p; i != p + size; ++i) {
        alloc.destroy(i);
      }
      alloc.deallocate(p, size);
    }

    /**
     * @brief Is this ReferenceCountedArray shared?
     */
    inline bool isShared() const __attribute__((pure)) {
      return refs > 1;
    }
  private:
    // No copying allowed
    ArrayRef(const ArrayRef&) = delete;
    ArrayRef& operator=(const ArrayRef&) = delete;
};

template <class T>
/**
 * @class Slice
 * @brief Safe subarray reading and writing.
 * This class should be optimized away and fully inlined such that:
 * ReferenceCountedArray s("look over there");
 * s(0, 4) = "blah"';
 * Should be rewritten as:
 * s.replace(0, "blah", 4);
 */
class Slice {
  private:
    T* rca;
    ssize_t start;
    ssize_t len;

  public:
    Slice() = delete;
    Slice(T* _rca, ssize_t _start, ssize_t _len) :
      rca(_rca), start(_start), len(_len) {};
    Slice(const Slice& slice) = default;
    Slice(Slice&& slice) noexcept = default;

    /**
     * @brief return a new (const) slice
     */
    inline operator T() const {
      T newArray(*rca);
      newArray.slice(start, len);
      return newArray;
    };

    friend void swap(Slice& a, Slice& b) {
      using std::swap;

      swap(a.rca, b.rca);
      swap(a.start, b.start);
      swap(a.len, b.len);
    }

    /**
     * @brief Assign a Slice to a Slice
     */
    inline Slice& operator=(const Slice& slice) {
      (*this) = T(slice);
      return (*this);
    }

    /**
     * @brief Assign a Slice to a Slice
     */
    inline Slice& operator=(Slice&& slice) noexcept {
      (*this) = T(std::move(slice));
      return (*this);
    }

    /**
     * @brief Assign to a Slice
     * @todo This needs to account for negative start/len
     */
    inline Slice& operator=(const T& array) {
      rca->replace(start, array, len);
      return (*this);
    }

    /**
     * @brief Assign to a Slice
     * @todo This needs to account for negative start/len
     */
    inline Slice& operator=(T&& array) {
      rca->replace(start, std::move(array), len);
      return (*this);
    }
};

class ReferenceCountedArrayBase {
  public:
    static const size_t npos = static_cast<size_t>(-1);
};


static const double _rca_cow_scaling_factor = 1.5;

template <class T, class Allocator = std::allocator<T> >
/**
 * @class ReferenceCountedArray
 * @brief Common template base class for String and Array
 */
class ReferenceCountedArray : public ReferenceCountedArrayBase {
  public:
    typedef T value_type;

    typedef typename Allocator::size_type             size_type;
    typedef typename Allocator::difference_type       difference_type;
    typedef typename Allocator::pointer               pointer;
    typedef typename Allocator::const_pointer         const_pointer;
    typedef typename Allocator::reference             reference;
    typedef typename Allocator::const_reference       const_reference;
    typedef typename Allocator::pointer               iterator;
    typedef typename Allocator::const_pointer         const_iterator;
    typedef std::random_access_iterator_tag           iterator_category;
    typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;
    typedef std::reverse_iterator<iterator>           reverse_iterator;

  private:
    /**
     * @brief Detach from the shared reference.
     * This is only called when losing the old buffer or when modifying the buffer (and copy-on-write is used)
     * @note This does not free the old reference, as it is still in use
     */
    void doDetach() const {
      decRef();
      Ref = nullptr;
      sublen = 0;
      offset = 0;
      my_hash = 0;
    }
    /**
     * @brief Increment our reference counter.
     */
    inline int incRef() const { return Ref ? ++Ref->refs : 0; };

    /**
     * @brief Decrement our reference counter.
     */
    inline int decRef() const { return Ref ? --Ref->refs : 0; };

  protected:
    /**
     * @brief Set the lengths to the specified length
     * @param newLen the new length to set to
     */
    inline void setLength(size_t newLen) const { sublen = newLen; my_hash = 0; };

    /**
     * @sa setLength()
     */
    inline void addLength(size_t diff) const { sublen += diff; my_hash = 0; };

    /**
     * @sa setLength()
     */
    inline void subLength(size_t diff) const { sublen -= diff; my_hash = 0; };

    /**
     * @brief Mutable Ref->buf+offset reference for use internally
     */
    //pointer mdata() const { return Buf() + offset; };

    /**
     * @brief Mutable Ref->buf reference for use internally
     */
    inline pointer Buf(size_t pos = 0) const __attribute__((pure)) {
      return Ref ? &Ref->buf[offset + pos] : nullptr;
    }

    /**
     * @brief Ref->buf reference for use internally
     */
    inline const_pointer constBuf(size_t pos = 0) const __attribute__((pure)) {
      return Buf(pos);
    }
  private:
    Allocator alloc;
    /**
     * @brief The array reference for reference counting
     * This is mutable so that Ref->refs can be modified, which really is mutable
     */
    mutable ArrayRef<value_type, Allocator> *Ref = nullptr;
  protected:
    /**
     * Return the real buffer's start point, without accounting for offset. This is used for cleaning the buffer when needed.
     */
    inline const_pointer real_begin() const __attribute__((pure)) {
      return Ref ? Ref->buf : nullptr;
    }

    /**
     * This is for subarrays: so we know where the subarray starts.
     */
    mutable size_t offset = 0;
    /**
     * This is for subarrays: so we know where the subarray ends.
     */
    mutable size_t sublen = 0;

    /**
     * Cache of current hash() result. 0 if stale
     */
    mutable size_t my_hash = 0;

  private:
    /**
     * @brief Detach from the reference
     * This is called when the old buffer is no longer needed.
     * ie, operator=() was called.
     */
    void Detach() {
      if (isShared()) {
        doDetach();
      } else {
        setLength(0);
        offset = 0;
        my_hash = 0;
      }
    }

    /**
     * @brief Free up our reference if we have the last one.
     * @post The reference counter is decremented.
     * @post If this was the last Reference, it is free'd
     * This is only called in ~Array() and operator=(Array&).
     * It checks whether of not this Array was the last reference to the buffer, and if it was, it removes it.
     */
    inline void CheckDeallocRef() {
      if (Ref && decRef() < 1) {
        delete Ref;
        Ref = nullptr;
      }
    }

    //    void COW(size_t) const;
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
     * Also take a hint size n of the new ReferenceCountedArray's size as to avoid needless copying/allocing.
     */
    void COW(size_t n) const {
      const_pointer oldBuf = constBuf();
      const size_t oldLength = length();

      doDetach(); //Detach from the shared reference
      Reserve(std::max(oldLength, n), _rca_cow_scaling_factor); //Will set capacity()/size
      std::copy(oldBuf, oldBuf + oldLength, Buf());
      setLength(oldLength);
    }

  protected:
    /**
     * @brief Force COW if needed
     * @post The array is no longer shared, if it was.
     */
    inline void getOwnCopy() const { AboutToModify(length()); };

    /**
     * @brief Warn the reference counting that it may need to COW
     * @post The buffer is detached/COW, and possibly larger
     */
    inline void AboutToModify(size_t n) const {
      my_hash = 0;
      if (isShared())
        COW(n); // Clears the offset
      else {
        Reserve(n, _rca_cow_scaling_factor);
        /* Shift the offset away */
      }
    }

    /**
     * @brief Checks if the buffer has the given index or not.
     * @param pos Index to check.
     */
    inline void validateIndex(size_t pos) const {
      if (pos >= length())
        throw std::out_of_range("ReferenceCountedArray::validateIndex");
    };

  public:
    ReferenceCountedArray(const Allocator& allocator = Allocator()) noexcept :
      ReferenceCountedArrayBase(), alloc(allocator) {
    }
    ReferenceCountedArray(const ReferenceCountedArray& rca) noexcept :
      ReferenceCountedArrayBase(), alloc(rca.alloc), Ref(rca.Ref),
      offset(rca.offset), sublen(rca.sublen), my_hash(rca.my_hash) {
      incRef();
    }
    ReferenceCountedArray(ReferenceCountedArray&& rca) noexcept :
      ReferenceCountedArrayBase(), alloc(std::move(rca.alloc)),
      Ref(rca.Ref), offset(rca.offset),
      sublen(rca.sublen), my_hash(rca.my_hash) {
      rca.Ref = nullptr;
      rca.offset = 0;
      rca.sublen = 0;
      rca.my_hash = 0;
    }

    /**
     * @brief Create an empty container with at least the specified bytes in size.
     * @param newSize Reserve at least this many bytes for this ReferenceCountedArray.
     * @post This ReferenceCountedArray's memory will also never be shrunk.
     * @post A buffer has been created.
     *
     * The idea behind this is that if a specific size was asked for, the buffer is like
     * a char buf[N];
     */
    explicit ReferenceCountedArray(const size_t newSize,
        const Allocator& allocator = Allocator()) :
      ReferenceCountedArray(allocator) {
      if (newSize) {
        Reserve(newSize);
      }
    }
    /**
     * @brief Create a container filled with n copies of the given value.
     * @param newSize Reserve at least this many bytes for this ReferenceCountedArray.
     * @param value The value to populate the array with
     * @post This ReferenceCountedArray's memory will also never be shrunk.
     * @post A buffer has been created.
     *
     */
    ReferenceCountedArray(const size_t newSize, const value_type value,
        const Allocator& allocator = Allocator()) :
      ReferenceCountedArray(allocator) {
      if (newSize) {
        Reserve(newSize);

        for (size_t i = 0; i < newSize; ++i) {
          *(Buf(i)) = value;
        }
        setLength(newSize);
      }
    }

    /**
     * @brief Array Destructor
     * @post If the Array's Reference is not shared, it is free'd.
     * @post If the Array's Reference IS shared, it is decremented and detached.
     */
    ~ReferenceCountedArray() { CheckDeallocRef(); };

    /**
     * @brief Swap this with another
     */
    friend void swap(ReferenceCountedArray& a, ReferenceCountedArray& b) {
      using std::swap;

      swap(a.alloc, b.alloc);
      swap(a.offset, b.offset);
      swap(a.sublen, b.sublen);
      swap(a.my_hash, b.my_hash);
      swap(a.Ref, b.Ref);
    }

    /**
     * @brief Sets our Reference to the given ReferenceCountedArray reference.
     * @param rca The ReferenceCountedArray object to reference.
     * @post The old buffer (if we had one) is free'd.
     * @post Our Reference now points to the given ReferenceCountedArray.
     * @post Our old rca object has been deleted (disconnected).
     * @return The new rca object.
     * This handles self-assignment just fine, checking for it explicitly would be ineffecient for most cases.
     */
    ReferenceCountedArray& operator=(const ReferenceCountedArray& rca) {
      rca.incRef();
      alloc = rca.alloc;
      offset = rca.offset;
      sublen = rca.sublen;
      my_hash = rca.my_hash;
      CheckDeallocRef();
      Ref = rca.Ref;
      return *this;
    }

    /**
     * @brief Moves the given ReferenceCountedArray to this
     * @param rca The ReferenceCountedArray object to take ownership of.
     */
    ReferenceCountedArray& operator=(ReferenceCountedArray&& rca) noexcept {
      if (&rca == this)
        return *this;
      CheckDeallocRef();
      alloc = std::move(rca.alloc);
      offset = rca.offset;
      sublen = rca.sublen;
      my_hash = rca.my_hash;
      Ref = rca.Ref;

      rca.offset = 0;
      rca.sublen = 0;
      rca.my_hash = 0;
      rca.Ref = nullptr;

      return *this;
    }

    /**
     * @brief How many references does this object have?
     */
    inline size_t rcount() const { return Ref ? Ref->refs.load() : 0; };
    /**
     * @return True if this object is shared; false if not.
     */
    inline bool isShared() const __attribute__((pure)) {
      return Ref && Ref->isShared();
    }

    /**
     * @sa ArrayRef::Reserve()
     * @param newSize A size that we need to Allocate the buffer to.
     * @param scaling_factor How much to multiple the size by to help avoid later resizing
     * @post The ReferenceCountedArray will also never shrink after this.
     */
    void Reserve(const size_t newSize,
        double scaling_factor = 1) const {
      if (!Ref) {
        Ref = new ArrayRef<value_type, Allocator>(alloc);
      }
      Ref->Reserve(newSize, scaling_factor, offset, sublen);
    }

    /**
     * @brief Clear contents of ReferenceCountedArray and set length to 0
     */
    inline void clear() { Detach(); };

    /**
     * @brief Returns capacity of the ReferenceCountedArray object.
     * @return Capacity of the ReferenceCountedArray object.
     */
    inline size_t capacity() const __attribute__((pure)) {
      return Ref ? Ref->size : 0;
    }

    /**
     * @brief Resize the array to the given length.
     * @param len The length to resize to.
     * @param value The optional parameter to fill the space with if the array is expanded
     */
    void resize(size_t len, value_type value = value_type()) {
      if (len < this->length()) {
        this->subLength(this->length() - len);
      } else {
        this->AboutToModify(len);
        for (size_t i = 0; i < (len - this->length()); ++i)
          *(Buf(this->length() + i)) = value;
        this->addLength(len - this->length());
      }
    }

    /* Accessors */
    /**
     * @brief Returns length of the ReferenceCountedArray.
     * @return Length of the ReferenceCountedArray.
     */
    inline size_t length() const __attribute__((pure)) {
      return sublen;
    }
    /**
     * @sa length()
     */
    inline size_t size() const __attribute__((pure)) {
      return length();
    }

    /**
     * @brief Check whether the ReferenceCountedArray is 'empty'
     * @return True if empty, false if non-empty
     */
    inline bool isEmpty() const __attribute__((pure)) {
      return length() == 0;
    }
    /**
     * @sa isEmpty()
     * This is for: if (!string)
     * Having if(string) conflicts with another operator
     */
    inline bool operator!() const __attribute__((pure)) {
      return isEmpty();
    }
    inline explicit operator bool() const __attribute__((pure)) {
      return !isEmpty();
    }

    /**
     * @brief Data accessor
     * @return Pointer to array of characters (not necesarily null-terminated).
     */
    inline const_pointer data() const __attribute__((pure)) {
      return constBuf();
    }
    /**
     * @brief Give an OutputIterator for STL usage
     * @post The Array is detached.
     */
    inline pointer mdata() const { AboutToModify(length()); return Buf(); }

    /**
     * @brief Returns a read/write iterator into the Array.
     * @post The Array is detached
     */
    inline iterator begin() { return iterator(mdata()); };

    inline const_iterator cbegin() const __attribute__((pure)) {
      return const_iterator(this->data());
    };
    /**
     * @brief Returns a read-only iterator into the Array
     */
    inline const_iterator begin() const __attribute__((pure)) {
      return this->cbegin();
    }

    /**
     * @brief Returns a read/write iterator at the end the Array
     * @post The Array is detached
     */
    inline iterator end() { return iterator(begin()) + length(); };

    inline const_iterator cend() const __attribute__((pure)) {
      return const_iterator(this->cbegin()) + this->length();
    };
    /**
     * @brief Returns a read-only iterator at the end of the Array
     */
    inline const_iterator end() const __attribute__((pure)) {
      return this->cend();
    }

    /**
     * @brief Returns a read/write reverse iterator at the end of the Array. Iteration is done in reverse order.
     * @post The Array is detached
     */
    inline reverse_iterator rbegin() { return reverse_iterator(this->end()); };

    inline const_reverse_iterator crbegin() const {
      return const_reverse_iterator(this->cend());
    };
    /**
     * @brief Returns a read-only reverse iterator at the end of the Array. Iteration is done in reverse order.
     */
    inline const_reverse_iterator rbegin() const __attribute__((pure)) {
      return this->crbegin();
    }

    /**
     * @brief Returns a read/write reverse iterator at the beginning of the Array. Iteration is done in reverse order.
     * @post The Array is detached
     */
    inline reverse_iterator rend() { return reverse_iterator(this->begin()); };

    inline const_reverse_iterator crend() const __attribute__((pure)) {
      return const_reverse_iterator(this->cbegin());
    };
    /**
     * @brief Returns a read-only reverse iterator at the beginning of the Array. Iteration is done in reverse order.
     */
    inline const_reverse_iterator rend() const __attribute__((pure)) {
      return this->crend();
    }

   /**
     * @brief Return a hash of every element in the array. Cache result as well.
     * @note DJB's hash function
     */
    size_t hash() const {
      if (my_hash != 0) return my_hash;
      std::hash<value_type> hasher;
      size_t _hash = 5381;

      for(size_t i = 0; i < this->length(); ++i)
        _hash = ((_hash << 5) + _hash) + hasher(this->data()[i]);
      return (my_hash = (_hash & 0x7FFFFFFF));
    }


    /**
     * @brief Find an item in the array
     * @return The position of the item if found, or npos if not found
     **/
    size_t find(const_reference item) const __attribute__((pure)) {
      for (size_t i = 0; i < length(); ++i)
        if (*(constBuf(i)) == item)
          return i;
      return npos;
    }

    /**
     * @brief Find an item in the array, starting from the end
     * @return The position of the item if found, or npos if not found
     **/
    size_t rfind(const_reference item) const __attribute__((pure)) {
      for (size_t i = length() - 1; i + 1 > 0; --i)
        if (*(constBuf(i)) == item)
          return i;
      return npos;
    }

    // Safe index accessors

    /**
     * @sa at()
     * Unlike at() this is unchecked.
     */
    inline value_type read(size_t pos) const noexcept __attribute__((pure)) {
      return *(constBuf(pos));
    };

    /**
     * @brief Write an item to the given index
     */
    inline void write(size_t pos, value_type item) {
      getOwnCopy();
      *(Buf(pos)) = std::move(item);
    };

    /**
     * @brief Safe element access operator
     * @todo This is only called on a (const) ReferenceCountedArray, but should for a ReferenceCountedArray as well.
     */
    inline value_type operator[](size_t pos) const noexcept
      __attribute__((pure)) {
      return read(pos);
    };

    /**
     * @class Cref
     * @brief Safe element reading and writing.
     * This class should be optimized away and fully inlined such that:
     * ReferenceCountedArray s;
     * s[0] = 'a';
     * Should be rewritten as:
     * s.write(0, 'a');
     */
    class Cref {
      private:
        friend class ReferenceCountedArray;
        ReferenceCountedArray* rca;
        size_t k;

        /**
         * @brief Used by Cref operator[]
         */
        Cref(ReferenceCountedArray* _rca, size_t pos) : rca(_rca), k(pos) {};

      public:
        Cref() = delete;
        Cref(const Cref& cref) = default;
        Cref(Cref&& cref) = default;
        inline Cref& operator=(const Cref& cref) {
          (*this) = value_type(cref);
          return (*this);
        }
        inline Cref& operator=(Cref&& cref) noexcept {
          (*this) = value_type(std::move(cref));
          return (*this);
        }

        /**
         * @sa ReferenceCountedArray::operator[]
         */
        inline operator value_type() const noexcept __attribute__((pure)) {
          return rca->read(k);
        }

        /**
         * Stroustrup shows using this as void with no return value, but that breaks chaining a[n] = b[n] = 'b';
         */
        inline Cref& operator=(const_reference c) {
          rca->write(k, c);
          return (*this);
        };
        inline Cref& operator=(value_type&& c) {
          rca->write(k, std::move(c));
          return (*this);
        };
    };

    /**
     * @brief Returns 'Cref' class for safe (cow) writing.
     * @sa Cref
     */
    inline Cref operator[](size_t pos) { return Cref(this, pos); };

    /**
     * @brief Returns the character at the given index.
     * @return The character at the given index.
     * @param pos Index to return.
     * @pre The index must exist.
     * @sa operator[]()
     * @todo Perhaps this should throw an exception if out of range?
     */
    inline value_type at(size_t pos) const {
      validateIndex(pos);
      return (*this)[pos];
    };

    /**
     * @param start The offset to begin the subarray from (indexed from 0)
     * @param len The length of the subarray to return
     * The returned slice is a reference to the original array until modified.
     */
    void slice(ssize_t start, ssize_t len = -1) {
      if (len == -1) len = int(length()) - start;
      // Start is after the end, set us to an empty array
      if (start >= static_cast<signed>(length())) {
        offset = length();
        setLength(0);
        return;
      }

      //Count backwards from the end
      if (start < 0)
        start = length() + start;
      // Start was before the beginning, just reset to the beginning
      if (start < 0)
        start = 0;

      offset += start;
      //If the length of the subarray exceeds the end of the array, truncate to the end of the array
      if (start + len >= static_cast<signed>(length()))
        len = length() - start;
      // If the length is negative, stop from counting backwards from the end
      else if (len < 0)
        len = length() - start + len;
      setLength(len);
    }

    /**
     * @brief Appends one item to end of buffer.
     * @param item The item to be appended.
     * @post The buffer is allocated.
     * @post The item is appended at the end of the buffer.
     * This is the same as inserting the item at the end of the buffer.
     */
    inline void append(const_reference item) { insert(length(), item); };
    inline void append(value_type&& item) { insert(length(), std::move(item)); };
    inline void push_back(const_reference item) { append(item); };
    inline void push_back(value_type&& item) { append(std::move(item)); };
    /**
     * @brief Appends given rca to the end of buffer
     * @param rca The rca to be appended.
     * @param n How many characters to copy from the ReferenceCountedArray object.
     * @post The buffer is allocated.
     * This is the same as inserting the rca at the end of the buffer.
     */
    inline void append(const ReferenceCountedArray& rca, size_t n = npos) {
      insert(length(), rca, n);
    }
    inline void append(ReferenceCountedArray&& rca, size_t n = npos) {
      insert(length(), std::move(rca), n);
    }

    /**
     * @brief Inserts a ReferenceCountedArray object into our buffer
     * @param pos The index to insert at.
     * @param rca The rca to insert.
     * @param n The length to insert.
     * @post The buffer contains n items from rca inserted at index pos.
     */
    void insert(size_t pos, const ReferenceCountedArray& rca, size_t n = npos) {
      if (n == 0) return;
      if (pos != 0)
        validateIndex(pos - 1);

      size_t slen = rca.length();

      /* New rca is longer than ours, and inserting at 0, just replace ours with a reference of theirs */
      if (pos == 0 && slen > length() && (n == npos || n == slen)) {
        *this = rca;
        return;
      }

      if (n == npos || n > slen)
        n = slen;
      slen -= slen - n;
      AboutToModify(length() + slen);
      std::memmove(static_cast<void*>(Buf() + pos + slen),
          static_cast<void*>(Buf() + pos), length() - pos);
      std::copy(rca.begin(), rca.begin() + slen, Buf() + pos);
      addLength(slen);
    }

    /**
     * @brief Inserts a ReferenceCountedArray object into our buffer
     * @param pos The index to insert at.
     * @param rca The rca to insert.
     * @param n The length to insert.
     * @post The buffer contains n items from rca inserted at index pos.
     */
    void insert(size_t pos, ReferenceCountedArray&& rca, size_t n = npos) {
      if (n == 0) return;
      if (pos != 0)
        validateIndex(pos - 1);

      size_t slen = rca.length();

      /* New rca is longer than ours, and inserting at 0, just move theirs */
      if (pos == 0 && slen > length() && (n == npos || n == slen)) {
        *this = std::move(rca);
        return;
      }

      if (n == npos || n > slen)
        n = slen;
      slen -= slen - n;
      AboutToModify(length() + slen);
      std::memmove(static_cast<void*>(Buf() + pos + slen),
          static_cast<void*>(Buf() + pos), length() - pos);
      std::move(rca.begin(), rca.begin() + slen, Buf() + pos);
      addLength(slen);
    }

    /**
     * @brief Insert an item at the given index.
     * @param pos The index to insert at.
     * @param item The item to be inserted.
     * @post A buffer is allocated.
     * @post If the old buffer was too small, it is enlarged.
     * @post The item is inserted at the given index.
     */
    void insert(size_t pos, const_reference item)
    {
      if (pos != 0)
        validateIndex(pos - 1);

      AboutToModify(length() + 1);
      std::memmove(static_cast<void*>(Buf() + pos + 1),
          static_cast<void*>(Buf() + pos), length() - pos);
      *(Buf(pos)) = item;
      addLength(1);
    }

    /**
     * @brief Insert an item at the given index.
     * @param pos The index to insert at.
     * @param item The item to be inserted.
     * @post A buffer is allocated.
     * @post If the old buffer was too small, it is enlarged.
     * @post The item is inserted at the given index.
     */
    void insert(size_t pos, value_type&& item)
    {
      if (pos != 0)
        validateIndex(pos - 1);

      AboutToModify(length() + 1);
      std::memmove(static_cast<void*>(Buf() + pos + 1),
          static_cast<void*>(Buf() + pos), length() - pos);
      *(Buf(pos)) = std::move(item);
      addLength(1);
    }

    /**
     * @brief Replace the given index with the given item.
     * @param pos The index to replace.
     * @param item The item to replace with.
     * @post The given index has been replaced.
     * @post COW is done if needed.
     */
    void replace(size_t pos, const_reference item) {
      if (pos != 0)
        validateIndex(pos - 1);

      getOwnCopy();
      *(Buf(pos)) = item;
    }

    /**
     * @brief Replace the given index with the given item.
     * @param pos The index to replace.
     * @param item The item to replace with.
     * @post The given index has been replaced.
     * @post COW is done if needed.
     */
    void replace(size_t pos, value_type&& item) {
      if (pos != 0)
        validateIndex(pos - 1);

      getOwnCopy();
      *(Buf(pos)) = std::move(item);
    }

    /**
     * @brief Replaces n elements in our buffer at index pos with the given ReferenceCountedArray object
     * @param pos The index to replace at.
     * @param rca The ReferenceCountedArray object to replace with.
     * @param n The number of characters to use for the replace.
     */
    void replace(size_t pos, const ReferenceCountedArray& rca, size_t n = npos) {
      if (n == 0) return;
      if (pos != 0)
        validateIndex(pos - 1);

      size_t slen = rca.length();

      /* Replace rca is longer than ours, and inserting at 0, just replace ours with a reference of theirs */
      if (pos == 0 && slen > length() && (n == npos || n == slen)) {
        *this = rca;
        return;
      }

      if (n == npos || n > slen)
        n = slen;
      slen -= slen - n;

      size_t newlen = pos + slen;

      if (newlen >= length()) {
        AboutToModify(newlen);
      } else {
        newlen = length();
        getOwnCopy();
      }
      std::copy(rca.begin(), rca.begin() + slen, Buf() + pos);
      setLength(newlen);
    }

    /**
     * @brief Replaces n elements in our buffer at index pos with the given ReferenceCountedArray object
     * @param pos The index to replace at.
     * @param rca The ReferenceCountedArray object to replace with.
     * @param n The number of characters to use for the replace.
     */
    void replace(size_t pos, ReferenceCountedArray&& rca, size_t n = npos) {
      if (n == 0) return;
      if (pos != 0)
        validateIndex(pos - 1);

      size_t slen = rca.length();

      /* Replace rca is longer than ours, and inserting at 0, just move theirs */
      if (pos == 0 && slen > length() && (n == npos || n == slen)) {
        *this = std::move(rca);
        return;
      }

      if (n == npos || n > slen)
        n = slen;
      slen -= slen - n;

      size_t newlen = pos + slen;

      if (newlen >= length()) {
        AboutToModify(newlen);
      } else {
        newlen = length();
        getOwnCopy();
      }
      std::move(rca.begin(), rca.begin() + slen, Buf() + pos);
      setLength(newlen);
    }

};
BDLIB_NS_END

#endif
/* vim: set sts=2 sw=2 ts=8 et: */
