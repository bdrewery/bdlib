/* Iterator.h
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
 * $Id$
 */
#ifndef _BD_ITERATOR_H
#define _BD_ITERATOR_H 1

#include <iterator>
#include "bdlib.h"

BDLIB_NS_BEGIN
template <typename T>
class Iterator {
  public:
    virtual bool hasNext() { return bool(*this); };
    virtual ~Iterator() {};

    virtual operator bool() = 0;
    virtual T& operator*() = 0;
    virtual T* operator->() { return &(operator*()); };

//    virtual Iterator& operator++() = 0;
//    virtual Iterator operator++(int) = 0;
//    virtual Iterator& operator--() = 0;
//    virtual Iterator operator--(int) = 0;

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
  typedef ptrdiff_t difference_type;
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

    inline const Key& key() const { return k; };
    inline const Value& value() const { return v; };
    template <class K, class V> inline friend bool operator == (const KeyValue<K, V> &lhs, const KeyValue<K, V> &rhs) { return lhs.key() == rhs.key(); }
/*
  KeyValue& operator=(const KeyValue& kv) {
    key = Key(kv.key);
    value = Value(kv.value);
    return *this;
  }
*/
};

BDLIB_NS_END
#endif /* !_BD_ITERATOR_H */ 
