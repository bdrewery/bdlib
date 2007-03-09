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

#include "bdlib.h"
#include "Iterator.h"
BDLIB_NS_BEGIN

template <class T>
/**
  * @class List
  * @brief List data structure
  */
class List {
  private:
    struct Node {
      const T ptr;
      Node* next;
      Node* prev;
      
      Node(const T& p) : ptr(p), next(NULL), prev(NULL) {};
    };

    Node* head;
    size_t my_size;

  public:
    List() : head(NULL), my_size(0) {};
    ~List() { clear(); };

    List(const List<T>& list) : head(NULL), my_size(0) {
      for (Node* search = list.head; search; search = search->next)
        insert(search->ptr);
    }

    List& operator =(const List<T>& list) {
      if (&list != this) {
        head = NULL;
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
      head = NULL;
      my_size = 0;
    };

    const size_t size() const { return my_size; };
    bool isEmpty() const { return size() == 0; };
    operator bool() const { return !isEmpty(); };

    /**
     * @brief Insert into the list at the head
     * @param ptr The ptr to insert
     */
    void insert(const T& ptr) {
      Node* node = new Node(ptr);
      if (!head) {
        head = node;
      } else {
        head->prev = node;
        node->next = head;
        head = node;
      }
      ++my_size;
    }

    bool contains(const T& ptr) const {
      if (head) {
        for (Node* node = head; node; node = node->next) {
          if (node->ptr == ptr)
            return 1;
        }
      }
      return 0;
    }

    bool remove(const T& ptr) {
      if (head) {
        for (Node** node = &head; node; node = &(*node)->next) {
          if ((*node)->ptr == ptr) {
            Node* next = (*node)->next;
            delete (*node);
            *node = next;
            --my_size;
            return 1;
          }
        }
      }
      return 0;
    }

};

BDLIB_NS_END

#endif /* !_W_LIST_H */ 
