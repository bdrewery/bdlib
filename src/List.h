/* List.h
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
#ifndef _W_LIST_H
#define _W_LIST_H 1

#include <iostream>
#include "String.h"
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
    typedef T iterator_type;


  protected:
    struct Node {
      iterator_type ptr;
      Node* next;
      Node* prev;
      
      Node(const iterator_type& p) : ptr(p), next(NULL), prev(NULL) {};
    };

    Node* head;
    Node* tail;
    size_t my_size;

  public:
    List() : head(NULL), tail(NULL), my_size(0) {};
    virtual ~List() { clear(); };

    List(const List& list) : head(NULL), tail(NULL), my_size(0) {
      for (Node* search = list.head; search; search = search->next)
        insert(search->ptr);
    }

    List& operator =(const List& list) {
      if (&list != this) {
        clear();
        for (Node* search = list.head; search; search = search->next)
          insert(search->ptr);
      }
      return *this;
    }

    void clear() {
      Node* node = head, *node_n = NULL;
      while (node) {
        node_n = node->next;
        delete node;
        node = node_n;
      }
      head = tail = NULL;
      my_size = 0;
    };

    const size_t size() const { return my_size; };
    bool isEmpty() const { return size() == 0; };
    operator bool() const { return !isEmpty(); };

    /**
     * @brief Insert into the list at the head
     * @param ptr The ptr to insert
     */
    void insert(const iterator_type& ptr) {
      Node* node = new Node(ptr);
      if (!head) {
        head = tail = node;
      } else {
        head->prev = node;
        node->next = head;
        head = node;
      }
      ++my_size;
    }

    bool contains(const iterator_type& ptr) const {
      if (head) {
        for (Node* node = head; node; node = node->next) {
          if (node->ptr == ptr)
            return 1;
        }
      }
      return 0;
    }

   const iterator_type find(const iterator_type& ptr) const {
      const iterator_type empty;
      if (isEmpty()) return empty;

      for (Node* current = head; current; current = current->next) {
        if (current->ptr == ptr)
          return current->ptr;
      }
      return empty;
    }

    /**
     * @brief Return a reference to the found element
     */
    iterator_type& findRef(const iterator_type& ptr) {
      Node* current = head;
      for (; current; current = current->next) {
        if (current->ptr == ptr)
          break;
      }
      return current->ptr;
    }

    bool remove(const iterator_type& ptr) {
      if (head) {
      /* cases (removing X):
       * 1) list = (X)
       * 2) list = (X)->(Y)
       * 3) list = (W)->(X)
       * 4) list = (W)->(X)->(Y)
       */
        for (Node* node = head; node; node = node->next) {
          if (node->ptr == ptr) {
            Node* next = node->next; //NULL if node = tail
            Node* prev = node->prev; //NULL if node = head

            delete node;

            if (next) next->prev = prev;
            if (prev) prev->next = next;

            if (node == head) head = next;
            if (node == tail) tail = prev;

            --my_size;
            return true;
          }
        }
      }
      return false;
    }

    private:
    class ListIterator : public Iterator<iterator_type> {
      friend class List;
      private:
        List *list;
        Node *current;
        bool end;

        ListIterator(List &_list, bool _end = 0) : Iterator<iterator_type>(),
                                                              list(&_list),
                                                              current(_end ? _list.head : _list.tail), /* tail is beginning */
                                                              end(_end) {
        };
      public:
        ListIterator(const iterator& iter) : Iterator<iterator_type>(),
                                             list(iter.list),
                                             current(iter.current),
                                             end(iter.end) {
        };
        ListIterator() : Iterator<iterator_type>(),
                         list(NULL),
                         current(NULL),
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

          list->remove(current->ptr);

          current = nextValue;
/*
          --my_size;
          for (int i = index; i < my_size; ++i)
            storage[i] = storage[i + 1];
          //The iterator is 'invalid' now, but the index is shifter down in case iter++ is called
          --index;
*/
        }

        virtual operator bool() { return (current != NULL); };
        virtual operator iterator_type () { return operator*(); };
          
        virtual iterator_type& operator *() { return (iterator_type&)current->ptr; }

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
    iterator begin() { return iterator(*this); };
    iterator end() { return iterator(*this, 1); };
};

BDLIB_NS_END

#endif /* !_W_LIST_H */ 
