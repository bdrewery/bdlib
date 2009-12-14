/* ReferenceCountedArray.h
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
 */
#ifndef _W_REFERENCE_COUNTED_ARRAY_H
#define _W_REFERENCE_COUNTED_ARRAY_H 1

#include "bdlib.h"
#include <iterator>
#include <stdint.h>
#include <sys/types.h>

BDLIB_NS_BEGIN
template <class T>
/**
 * @class StringBuf
 * @brief Helps the String class with reference counting
 * @todo look into something like a string object made of string pointers, and << displays all of the pointed to objects. (this would save copying to construct immuatable concatenated strings, but would only work with the << stream
 */
class ArrayRef {
  public:
    ~ArrayRef() { FreeBuf(buf); };
    mutable size_t len; //Length of string
    mutable size_t size; //Capacity of buffer
    mutable T* buf;
    mutable uint8_t n; //References

    ArrayRef() : len(0), size(0), buf(NULL), n(1) {};
    /**
     * @brief Ensure that the buffer capacity() is >= newSize; else grow/copy into larger buffer.
     * @param newSize A size that we need to Allocate the buffer to.
     * @pre newSize is > 0 (assumed as size_t is unsigned)
     * @post The buffer is at least nsize bytes long.
     * @post If the buffer had to grow, the old data was deep copied into the new buffer and the old deleted.
     */
    void Reserve(const size_t newSize, size_t& offset) const
    {
      /* Don't new if we already have enough room! */
      if (size < newSize) {
        size = std::max(size_t(size * 1.5), newSize);

        T *newbuf = AllocBuf(size);

        if (newbuf != buf) {
          /* Copy old buffer into new - only copy the substring */
          std::copy(buf + offset, buf + offset + len, newbuf);
          FreeBuf(buf);
          buf = newbuf;
          offset = 0;
        }
      } else if ((size - offset) < newSize) {
        // There's enough room in the current buffer, but we're offsetted/shifted to a point where there's no room left
        // Shift everything to the beginning and reset the offset
        /* Only copy the substring */
        memmove(buf, buf + offset, len);
        offset = 0;
      }
    }
    //void Reserve(const size_t, size_t&) const;

    /**
     * @brief Allocates a buffer and returns it's address.
     * @param bytes The number of bytes to allocate.
     * @post A new block of memory is allocated.
     * @todo Implement mempool here.
     */
    inline T* AllocBuf(const size_t bytes) const {
      return new T[bytes];
    }

    /**
     * @brief Free's up the allocated buffer.
     * @param p The buffer to be free'd
     * @post The buffer is deleted.
     * @todo Implement mempool here.
     */
    inline void FreeBuf(T* p) const {
      delete[] p;
    }

    /**
     * @brief Is this string shared?
     */
    inline bool isShared() const { return n > 1; };
  private:
    // No copying allowed
    ArrayRef(const ArrayRef&); ///<Block implicit copy constructor
    ArrayRef& operator=(const ArrayRef&); ///<Block implicit copy constructor
};

template <class T>
/**
 * @class ReferenceCountedArray
 * @brief
 */
class ReferenceCountedArray {
  public:
    typedef T value_type;

    typedef size_t             size_type;
    typedef ptrdiff_t          difference_type;
    typedef value_type*        pointer;
    typedef const value_type*  const_pointer;
    typedef value_type&        reference;
    typedef const value_type&  const_reference;

  protected:
    /**
     * @brief Detach from the shared reference.
     * This is only called when losing the old buffer or when modifying the buffer (and copy-on-write is used)
     * @note This does not free the old reference, as it is still in use
     */
    void doDetach() const {
      decRef();
      Ref = new ArrayRef<value_type>();
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
    //pointer mdata() const { return Buf() + offset; };

    /**
     * @brief Mutable Ref->buf reference for use internally
     */
    inline pointer Buf(int i = 0) const { return &Ref->buf[offset + i]; };

    /**
     * @brief Ref->buf reference for use internally
     */
    inline const_pointer constBuf(int i = 0) const { return Buf(i); };

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
     * @brief The array reference for reference counting
     * This is mutable so that Ref->n can be modified, which really is mutable
     */
    mutable ArrayRef<value_type> *Ref;

    /**
     * This is for subarrays: so we know where the subarray starts.
     */
    mutable size_t offset;
    /**
     * This is for subarrays: so we know where the subarray ends.
     */
    mutable size_t sublen;

    /**
     * @brief Free up our reference if we have the last one.
     * @post The reference counter is decremented.
     * @post If this was the last Reference, it is free'd
     * This is only called in ~Array() and operator=(Array&).
     * It checks whether of not this Array was the last reference to the buffer, and if it was, it removes it.
     */
    inline void CheckDeallocRef() {
      if (decRef() < 1)
        delete Ref;
    }

    /**
     * @brief Detach from the reference
     * This is called when the old buffer is no longer needed for this Array.
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
     * Also take a hint size n of the new string's size as to avoid needless copying/allocing.
     */
    void COW(size_t n) const {
      const_pointer oldBuf = constBuf();
      size_t oldLength = length();
      size_t oldCapacity = capacity();

      doDetach(); //Detach from the shared reference
      Reserve( std::max(oldCapacity, n) ); //Will set capacity()/size
      std::copy(oldBuf, oldBuf + oldLength, Buf());
      setLength(oldLength);
    }

    inline void AboutToModify(size_t n) const {
      if (isShared())
        COW(n); // Clears the offset
      else {
        Reserve(n);
        /* Shift the offset away */
      }
    }
    inline void getOwnCopy() const { AboutToModify(capacity()); };
  public:
    ReferenceCountedArray() : Ref(new ArrayRef<value_type>()), offset(0), sublen(0) {};
    ReferenceCountedArray(const ReferenceCountedArray& rca) : Ref(rca.Ref), offset(rca.offset), sublen(rca.sublen) { incRef(); };
    /**
     * @brief Create an empty String container with at least the specified bytes in size.
     * @param newSize Reserve at least this many bytes for this String.
     * @post This string's memory will also never be shrunk.
     * @post A buffer has been created.
     *
     * The idea behind this is that if a specific size was asked for, the buffer is like
     * a char buf[N];
     */
    explicit ReferenceCountedArray(const size_t newSize) : Ref(new ArrayRef<value_type>()), offset(0), sublen(0) {
      if (newSize <= 0) return;
      Reserve(newSize);
    };

    /**
     * @brief Array Destructor
     * @post If the Array's Reference is not shared, it is free'd.
     * @post If the Array's Reference IS shared, it is decremented and detached.
     */
    virtual ~ReferenceCountedArray() { CheckDeallocRef(); };

    /**
     * @brief Sets our Reference to the given String reference.
     * @param rca The String object to reference.
     * @post The old buffer (if we had one) is free'd.
     * @post Our Reference now points to the given String.
     * @post Our old rca object has been deleted (disconnected).
     * @return The new rca object.
     * This handles self-assignment just fine, checking for it explicitly would be ineffecient for most cases.
     */
    const ReferenceCountedArray& operator=(const ReferenceCountedArray &rca) {
      rca.incRef();
      offset = rca.offset;
      sublen = rca.sublen;
      CheckDeallocRef();
      Ref = rca.Ref;
      return *this;
    }

    inline size_t rcount() const { return Ref->n; };

    /**
     * @sa ArrayRef::Reserve()
     * @post The String will also never shrink after this.
     */
    virtual void Reserve(const size_t newSize) const { Ref->Reserve(newSize, offset); };

    /**
     * @brief Clear contents of String and set length to 0
     */
    virtual inline void clear() { Detach(); };

    /**
     * @brief Returns capacity of the String object.
     * @return Capacity of the String object.
     */
    inline size_t capacity() const { return Ref->size; };


    /* Accessors */
    /**
     * @brief Returns length of the string.
     * @return Length of the string.
     */
    inline size_t length() const { return sublen; };
    /**
     * @sa length()
     */
    inline size_t size() const { return length(); };

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
     * @brief Data accessor
     * @return Pointer to array of characters (not necesarily null-terminated).
     */
    inline const_pointer data() const { return constBuf(); }

    virtual size_t hash() const = 0;
};
BDLIB_NS_END

#endif
