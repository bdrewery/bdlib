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
 */
#ifndef _BD_HASHTABLE_H
#define _BD_HASHTABLE_H 1

#include "bdlib.h"
#include "hash.h"
#include "Iterator.h"
#include "List.h"
BDLIB_NS_BEGIN

template <class Key, class Value>
/**
  * @class HashTable
  * @brief HashTable data structure
  * @todo replace()
  * @todo iterators
  * @todo resizing/growing
  *
  */
class HashTable {
  private:
    typedef KeyValue<Key, Value> iterator_type;
    typedef hash<Key> hasher;

    List<iterator_type> *list;
    size_t _size;
    size_t _capacity;
    hasher _hash;

    inline size_t getIndex(const Key &key) const {
      return _hash(key) % _capacity;
    }
  public:
    HashTable() : list(new List<iterator_type>[100]), _size(0), _capacity(100), _hash() {};
    explicit HashTable(size_t capacity) : list(new List<iterator_type>[capacity]), _size(0), _capacity(capacity), _hash() {};
    HashTable(const HashTable<Key, Value> &table) : list(new List<iterator_type>[table._capacity]), _size(table._size), _capacity(table._capacity), _hash(table._hash) {
      for (size_t i = 0; i < _capacity; ++i)
          list[i] = table.list[i];
    };

    virtual ~HashTable() {
      delete[] list;
    }

    void clear() {
      for (size_t i = 0; i < _capacity; ++i)
        list[i].clear();
      _size = 0;
    }

    HashTable &operator = (const HashTable<Key, Value> &table) { 
      if (&table != this) {
        delete[] list;
        list = new List<iterator_type>[_capacity];
        for (size_t i = 0; i < _capacity; ++i) 
          list[i] = table.list[i];
        _size = table._size;
        _capacity = table._capacity;
        _hash = table._hash;
      }
      return *this; 
    }

    inline size_t size() const { return _size; };
    inline size_t capacity() const { return _capacity; };
    inline bool isEmpty() const { return size() == 0; };
    //inline operator bool() const { return !isEmpty(); };

    bool insert(const Key &key, const Value &value) { 
      if (contains(key)) return false;
      list[getIndex(key)] << iterator_type(key, value);
      ++_size;
      return true;
    }

    inline bool contains(const Key &key) const {
      if (isEmpty()) return false;
      return list[getIndex(key)].contains(iterator_type(key, Value()));
    };

    bool remove(const Key &key) {
      if (isEmpty()) return false;
      if (list[getIndex(key)].remove(iterator_type(key, Value()))) {
        --_size;
        return true;
      }
      return false;
    };

    inline Value getValue(const Key &key) const {
      Value empty;
      if (isEmpty()) return empty;
      return list[getIndex(key)].find(iterator_type(key, Value())).value();
    };

    /**
      * @brief Associate array type accessor (rvalue)
      * @param key The key to search for
      */
    inline const Value operator [](const Key& key) const { return getValue(key); }
    
    /**
      * @brief Associate array type accessor (lvalue)
      * @param key The key to search for
      * @sa find_or_insert_key 
      * @code table["key"] = "value";
      * If the key is not in the table, it is inserted, and the value set to the rvalue given.
      */
    inline Value& operator [](const Key& key) {
      return find_or_insert_key(key);
    }
 
    /**
     * @brief Find a key in the list or insert it.
     */
    inline Value& find_or_insert_key(const Key& key) {
      if (!contains(key))
        insert(key, Value());
      return list[getIndex(key)].findRef(iterator_type(key, Value())).v;
    }
};

BDLIB_NS_END

#endif /* !_BD_HASHTABLE_H */ 
