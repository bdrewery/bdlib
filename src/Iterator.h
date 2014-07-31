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
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Iterator.h
 */

#ifndef _BD_ITERATOR_H
#define _BD_ITERATOR_H 1

#include <iterator>
#include "bdlib.h"

BDLIB_NS_BEGIN
template <typename T>
class Iterator { //This should not exist.
/*
      ** http://www.fifi.org/doc/stl-manual/html/Iterators.html **

      typedef hashtable<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc>			_Hashtable;
      typedef _Hashtable_iterator<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc>		iterator;
      typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc>	const_iterator;
//      typedef _Hashtable_node<_Val> _Node;
      typedef forward_iterator_tag 								iterator_category;
      typedef _Val value_type;
      typedef std::ptrdiff_t 									difference_type;
      typedef size_t 										size_type;
      typedef _Val& 										reference;
      typedef _Val* 										pointer;

*/
  public:
    virtual bool hasNext() { return bool(*this); };
    virtual ~Iterator() {};

    virtual explicit operator bool() const = 0;
    virtual T& operator*() = 0;
    virtual T* operator->() { return &(operator*()); };

//    virtual bool operator==(const Iterator& iter) const;
//    virtual bool operator!=(const Iterator& iter) const;
//    virtual Iterator& operator++() = 0;
//    virtual Iterator operator++(int) = 0;
//    virtual Iterator& operator--() = 0; //should not be in all
//    virtual Iterator operator--(int) = 0; //should not be in all

//    virtual void remove() = 0;
};

/*
template<class Iter> struct iterator_traits {
  typedef typename Iter::iterator_category iterator_category;		//S(19.2.3)
  typedef typename Iter::value_type value_type;				//Type of element
  typedef typename Iter::difference_type difference_type;
  typedef typename Iter::pointer pointer;				//return type of operator->()
  typedef typename Iter::reference reference;				//return type of operator*()
};

template<class T> struct iterator_traits<T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef T* pointer;
  typedef T& reference;
}
*/

template <class Key, class Value>
struct KeyValue {
  public:
    Key k;
    Value v;

    KeyValue() : k(), v() {};
    KeyValue(const Key& _key, const Value& _value) : k(_key), v(_value) {};
    KeyValue(const KeyValue& kv) : k(kv.k), v(kv.v) {};
    KeyValue(KeyValue&& kv) : k(), v() {
      swap(*this, kv);
    }
      friend void swap(KeyValue& a, KeyValue& b) {
        using std::swap;

        swap(a.k, b.k);
        swap(a.v, b.v);
      }
      KeyValue& operator=(KeyValue kv) {
        swap(*this, kv);
        return *this;
      }

    inline const Key& key() const { return k; };
    inline const Value& value() const { return v; };
//    template <class K, class V> inline friend bool operator == (const KeyValue<K, V> &lhs, const KeyValue<K, V> &rhs) { return lhs.key() == rhs.key(); }
//    template <>
      inline friend bool operator == (const KeyValue &lhs, const KeyValue &rhs) { return lhs.key() == rhs.key(); }
};

BDLIB_NS_END
#endif /* _BD_ITERATOR_H */ 
/* vim: set sts=2 sw=2 ts=8 et: */
