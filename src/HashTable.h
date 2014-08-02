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

/* HashTable.h
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
    HashTable(HashTable<Key, Value>&& table) : _list(std::move(table._list)), _size(std::move(table._Size)), _capacity(std::move(table._capacity)), _hash(std::move(table._hash)) {
      table._list = nullptr;
      table._size = 0;
      table._capacity = default_list_size;
      table._hash = _hash();
    }
    HashTable(std::initializer_list<iterator_type> list) : _list(new List<iterator_type>[default_list_size]), _size(0), _capacity(default_list_size), _hash() {
      *this = list;
    }

    virtual ~HashTable() {
      delete[] _list;
    }

    void clear() {
      for (size_t i = 0; i < _capacity; ++i)
        _list[i].clear();
      _size = 0;
    }

    /**
     * @brief A ruby style block which will yield to the passed callback for each Key/Value pair.
     * @param block The block to execute for each element
     * @param param An optional parameter to pass to the block.
     * @return How many iterations were called
     */
    int each(hash_table_block block, void* param = nullptr) {
      int n = 0;
      if (!size()) return n;

      // Make a list of KeyValues to yield from.
      // Don't yield in this loop as the block may actually modify (this), thus making this iterator stale
      typename List<iterator_type>::iterator iter;
      List<iterator_type> items;
      for (size_t i = 0; i < _capacity; ++i) {
        if (_list[i].size()) {
          for (iter = _list[i].begin(); iter; (++iter)) {
            items << *iter;
          }
        }
      }

      // Now yield on our temporary, so (this) isn't a factor.
      for (iter = items.begin(); iter; (++iter)) {
        iterator_type kv = *iter;
        ++n;
        block(kv.key(), kv.value(), param);
      }
      return n;
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

      typename List<iterator_type>::iterator iter;
      for (size_t i = 0; i < capacity(); ++i) {
        if (_list[i].size()) {
          for (iter = _list[i].begin(); iter; (++iter)) {
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

      typename List<iterator_type>::iterator iter;
      for (size_t i = 0; i < capacity(); ++i) {
        if (_list[i].size()) {
          for (iter = _list[i].begin(); iter; (++iter)) {
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
/* vim: set sts=2 sw=2 ts=8 et: */
