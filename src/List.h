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

/* List.h
 */
#ifndef _BD_LIST_H
#define _BD_LIST_H 1

#include "bdlib.h"
#include "Iterator.h"
BDLIB_NS_BEGIN

template <class T>
/**
  * @class List
  * @brief List data structure
  */
class List {
  class ListIterator;

  public:
    typedef ListIterator iterator;
    typedef T value_type;

    typedef size_t             size_type;
    typedef std::ptrdiff_t     difference_type;
    typedef value_type*        pointer;
    typedef const value_type*  const_pointer;
    typedef value_type&        reference;
    typedef const value_type&  const_reference;

  protected:
    struct Node {
      value_type item;
      Node* next;
      Node* prev;
      
      Node(const_reference p) : item(p), next(nullptr), prev(nullptr) {};
      /* To avoid -Weffc++ warnings */
      Node(const Node& n) : item(n.item), next(n.next), prev(n.prev) {};
      Node(const value_type&& p) : item(std::move(p.item)), next(std::move(p.next)), prev(std::move(p.prev)) {
        item = nullptr;
        next = nullptr;
        prev = nullptr;
      };

      friend void swap(Node& a, Node& b) {
        using std::swap;

        swap(a.item, b.item);
        swap(a.next, b.next);
        swap(a.prev, b.prev);
      }

      Node& operator=(Node n) {
        swap(*this, n);
        return *this;
      }
    };

    void deleteNode(Node* node) {
      Node* next = node->next; //NULL if node = tail
      Node* prev = node->prev; //NULL if node = head

      delete node;

      if (next) next->prev = prev;
      if (prev) prev->next = next;

      if (node == head) head = next;
      if (node == tail) tail = prev;

      --my_size;
    }

    Node* findNode(const_reference item) const {
      for (Node* current = head; current; current = current->next) {
        if (current->item == item) {
          return current;
        }
      }
      return nullptr;
    }

    Node* head;
    Node* tail;
    size_type my_size;

  public:
    List() : head(nullptr), tail(nullptr), my_size(0) {};
    virtual ~List() { clear(); };

    List(const List& list) : head(nullptr), tail(nullptr), my_size(0) {
      for (Node* search = list.head; search; search = search->next)
        insert(search->item);
    }

    List(List&& list) : head(nullptr), tail(nullptr), my_size(0) {
      swap(*this, list);
    }

    friend void swap(List& a, List& b) {
      using std::swap;

      swap(a.head, b.head);
      swap(a.tail, b.tail);
      swap(a.my_size, b.my_size);
    }

    List& operator=(List list) {
      swap(*this, list);
      return *this;
    }

    void clear() {
      Node* node = head, *node_n = nullptr;
      while (node) {
        node_n = node->next;
        delete node;
        node = node_n;
      }
      head = tail = nullptr;
      my_size = 0;
    };

    inline size_type size() const { return my_size; };
    inline bool isEmpty() const { return size() == 0; };
    inline explicit operator bool() const { return !isEmpty(); };

    /**
     * @brief Insert into the list at the head
     * @param item The item to insert
     */
    void insert(const_reference item) {
      Node* node = new Node(item);
      if (!head) {
        head = tail = node;
      } else {
        head->prev = node;
        node->next = head;
        head = node;
      }
      ++my_size;
    }

    /**
     * @sa insert
     */
    inline friend List<value_type>& operator<< (List<value_type>& list, const_reference item) {
      list.insert(item);
      return list;
    }

    bool contains(const_reference item) const {
      return findNode(item) ? true : false;
    }

   const value_type find(const_reference item) const {
      Node* node = findNode(item);
      return node ? node->item : value_type();
    }

    /**
     * @brief Return a reference to the found element
     */
    reference findRef(const_reference item) {
      Node* node = findNode(item);
      return node ? node->item : head->item;
    }

    bool remove(const_reference item) {
      /* cases (removing X):
       * 1) list = (X)
       * 2) list = (X)->(Y)
       * 3) list = (W)->(X)
       * 4) list = (W)->(X)->(Y)
       */
      Node* node = findNode(item);
      if (node) {
        deleteNode(node);
        return true;
      }
      return false;
    }

    private:
    class ListIterator : public Iterator<value_type> {
      friend class List;
      private:
        List *list;
        Node *current;
        bool end;

        ListIterator(List &_list, bool _end = 0) : Iterator<value_type>(),
                                                              list(&_list),
                                                              current(_end ? _list.head : _list.tail), /* tail is beginning */
                                                              end(_end) {
        };
      public:
        ListIterator(const iterator& iter) : Iterator<value_type>(),
                                             list(iter.list),
                                             current(iter.current),
                                             end(iter.end) {
        };
        ListIterator() : Iterator<value_type>(),
                         list(nullptr),
                         current(nullptr),
                         end(0) {
        };

        iterator& operator =(const iterator& iter) {
          list = iter.list;
          current = iter.current;
          end = iter.end;
          return *this;
        }

        virtual ~ListIterator() { }

        virtual void remove() {
          Node *nextValue = end == 0 ? current->next : current->prev;

          list->deleteNode(current);

          current = nextValue;
        }

        virtual explicit operator bool() const { return (current != nullptr); };
        virtual operator value_type () { return operator*(); };
          
        virtual value_type& operator *() { return static_cast<value_type&>(current->item); }

        //Postfix
        virtual iterator operator ++(int) {
          iterator tmp(*this);
          current = current->prev;
          return tmp;
        }

        virtual iterator& operator ++() {
          current = current->prev;
          return *this;
        }

        //Postfix
        virtual iterator operator --(int) {
          iterator tmp(*this);
          current = current->next;
          return tmp;
        }

        virtual iterator& operator --() {
          current = current->next;
          return *this;
        }
    };
  public:
    inline iterator begin() { return iterator(*this); };
    inline iterator end() { return iterator(*this, 1); };
};

BDLIB_NS_END

#endif /* _BD_LIST_H */ 
