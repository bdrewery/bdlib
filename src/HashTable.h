/* HashTable.h
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
#ifndef _W_HASHTABLE_H
#define _W_HASHTABLE_H 1

#include "bdlib.h"
#include "Iterator.h"
#include "List.h"
BDLIB_NS_BEGIN

template <class Key, class Value>
/**
  * @class HashTable
  * @brief HashTable data structure
  */
class HashTable {
  private:
    typedef KeyValue<Key, Value> iterator_type;

    List<iterator_type> *list;
    size_t _size;
    size_t _capacity;

    int getIndex(const Key &key) const {
      return key % _capacity;
    }
  public:
    HashTable(size_t capacity = 100) : list(new List<iterator_type>[capacity]), _size(0), _capacity(capacity) {};
    HashTable(const HashTable<Key, Value> &table) : list(new List<iterator_type>[table._capacity]), _size(table._size), _capacity(table._capacity) {
      for (size_t i = 0; i < _capacity; ++i)
          list[i] = table.list[i];
    };

    virtual ~HashTable() {
      delete[] list;
    };

    HashTable &operator = (const HashTable<Key, Value> &table) { 
      if (&table != this) {
        delete[] list;
        _size = table._size;
        _capacity = table._capacity;
        list = new List<iterator_type>[_capacity];
        for (size_t i = 0; i < _capacity; ++i) 
          list[i] = table.list[i];
      }
      return *this; 
    }

    size_t size() const { return _size; };
    size_t capacity() const { return _capacity; };
    bool isEmpty() const { return size() == 0; };
    operator bool() const { return !isEmpty(); };

    bool insert(const Key &key, const Value &value) { 
      if (contains(key)) return false;
      int index = getIndex(key);
      list[index].insert(iterator_type(key, value));
      ++_size;
      return true;
    };
  
    bool contains(const Key &key) const {
      if (isEmpty()) return false;
      int index = getIndex(key);
      return list[index].contains(iterator_type(key, Value()));
    };

    bool remove(const Key &key) {
      if (isEmpty()) return false;
      int index = getIndex(key);
      if (list[index].remove(iterator_type(key, Value()))) {
        --_size;
        return true;
      }
      return false;
    };

    Value getValue(const Key &key) const {
      Value empty;
      if (isEmpty()) return empty;
      int index = getIndex(key);
      return list[index].find(iterator_type(key, Value())).value();
    };
};

BDLIB_NS_END

#endif /* !_W_HASHTABLE_H */ 
