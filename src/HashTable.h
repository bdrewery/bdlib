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
#include "Array.h"
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
    static const size_t default_list_size = 100;
    typedef KeyValue<Key, Value> iterator_type;
    typedef Hash<Key> hasher;
    typedef void (*hash_table_block)(const Key, Value, void *param);

    List<iterator_type> *_list;
    size_t _size;
    size_t _capacity;
    hasher _hash;

    inline size_t getIndex(const Key& key) const {
      return _hash(key) % _capacity;
    }
  public:
    HashTable() : _list(new List<iterator_type>[default_list_size]), _size(0), _capacity(default_list_size), _hash() {};
    explicit HashTable(size_t capacity_in) : _list(new List<iterator_type>[capacity_in]), _size(0), _capacity(capacity_in), _hash() {};
    HashTable(const HashTable<Key, Value>& table) : _list(new List<iterator_type>[table._capacity]), _size(table._size), _capacity(table._capacity), _hash(table._hash) {
      for (size_t i = 0; i < _capacity; ++i)
          _list[i] = table._list[i];
    };
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    HashTable(HashTable<Key, Value>&& table) : _list(NULL), _size(0), _capacity(default_list_size), _hash() {
      swap(*this, table);
    }
#endif
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    HashTable(std::initializer_list<iterator_type> list) : _list(new List<iterator_type>[default_list_size]), _size(0), _capacity(default_list_size), _hash() {
      *this = list;
    }
#endif

    virtual ~HashTable() {
      delete[] _list;
    }

    void clear() {
      for (size_t i = 0; i < _capacity; ++i)
        _list[i].clear();
      _size = 0;
    }

    /**
     * @brief A ruby stule block which will yield to the passed callback for each Key/Value pair.
     * @param block The block to execute for each element
     * @param param An optional parameter to pass to the block.
     */
    void each(hash_table_block block, void* param = NULL) {
      if (!size()) return;

      // Make a list of KeyValues to yield from.
      // Don't yield in this loop as the block may actually modify (this), thus making this iterator stale
      List<iterator_type> items;
      for (size_t i = 0; i < _capacity; ++i) {
        if (_list[i].size()) {
          for (auto iter = _list[i].begin(); iter; (++iter)) {
            items << *iter;
          }
        }
      }

      // Now yield on our temporary, so (this) isn't a factor.
      for (auto iter = items.begin(); iter; (++iter)) {
        iterator_type kv = *iter;
        block(kv.key(), kv.value(), param);
      }
    }

    friend void swap(HashTable<Key, Value>& a, HashTable<Key, Value>& b) {
      using std::swap;

      swap(a._list, b._list);
      swap(a._size, b._size);
      swap(a._capacity, b._capacity);
      swap(a._hash, b._hash);
    }

    HashTable& operator=(HashTable<Key, Value> table) {
      swap(*this, table);
      return *this; 
    }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    /**
     * @brief Create an array from an initializer list
     * @param list An initializer_list
     */
    HashTable& operator=(std::initializer_list<iterator_type> list) {
      for (iterator_type item : list) {
        (*this)[item.key()] = item.value();
      }
      return *this;
    }
#endif

    inline size_t size() const { return _size; };
    inline size_t capacity() const { return _capacity; };
    inline bool isEmpty() const { return size() == 0; };
    inline explicit operator bool() const { return !isEmpty(); };

    bool insert(const Key& key, const Value& value) {
      if (contains(key)) return false;
      _list[getIndex(key)] << iterator_type(key, value);
      ++_size;
      return true;
    }

    inline bool contains(const Key& key) const {
      if (isEmpty()) return false;
      return _list[getIndex(key)].contains(iterator_type(key, Value()));
    };

    bool remove(const Key& key) {
      if (isEmpty()) return false;
      if (_list[getIndex(key)].remove(iterator_type(key, Value()))) {
        --_size;
        return true;
      }
      return false;
    };

    inline Value getValue(const Key& key) const {
      Value empty;
      if (isEmpty()) return empty;
      return _list[getIndex(key)].find(iterator_type(key, Value())).value();
    };

    /**
      * @brief Associate array type accessor (rvalue)
      * @param key The key to search for
      */
    inline const Value operator[](const Key& key) const { return getValue(key); }

    /**
     * @brief Return an array of all the keys
     */
    Array<Key> keys() const {
      Array<Key> tmp(size());

      for (size_t i = 0; i < capacity(); ++i) {
        if (_list[i].size()) {
          for (auto iter = _list[i].begin(); iter; (++iter)) {
            iterator_type kv = *iter;
            tmp << kv.key();
          }
        }
      }
      return tmp;
    }

    /**
     * @brief Return an array of all the values
     */
    Array<Value> values() const {
      Array<Value> tmp(size());

      for (size_t i = 0; i < capacity(); ++i) {
        if (_list[i].size()) {
          for (auto iter = _list[i].begin(); iter; (++iter)) {
            iterator_type kv = *iter;
            tmp << kv.value();
          }
        }
      }
      return tmp;
    }
    
    /**
      * @brief Associate array type accessor (lvalue)
      * @param key The key to search for
      * @sa find_or_insert_key 
      * If the key is not in the table, it is inserted, and the value set to the rvalue given.
      */
    inline Value& operator[](const Key& key) {
      return find_or_insert_key(key);
    }
 
    /**
     * @brief Find a key in the list or insert it.
     */
    inline Value& find_or_insert_key(const Key& key) {
      if (!contains(key))
        insert(key, Value());
      return _list[getIndex(key)].findRef(iterator_type(key, Value())).v;
    }
};

BDLIB_NS_END

#endif /* _BD_HASHTABLE_H */ 
