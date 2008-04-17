/* SortedList.h
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
#ifndef _W_SORTEDLIST_H
#define _W_SORTEDLIST_H 1

#include "bdlib.h"
#include "List.h"

BDLIB_NS_BEGIN

template <class T>
/**
 * @class SortedList
 * @brief A List which sorts on insert
 */
class SortedList : public List<T> {
  typedef T iterator_type;
  typedef typename List<iterator_type>::Node Node;

  public:
    SortedList() : List<iterator_type>() {};
    virtual ~SortedList() {};

    /**
     * @brief Insert an element into the list (ordered)
     * @param ptr The element to insert
     * This operation is O(n/2)
     */
    virtual void insert(const iterator_type& ptr) {
      /*
       * 1) list = NULL
       * 2) list = (X)
       * 3) list = (X)->(Y)
       * 4) list = (W)->(X)->(Y)
       */
      Node* node = new Node(ptr), *prev = NULL, *current = this->head;
      while (current != NULL && ptr < current->ptr) {
        prev = current;
        current = current->next;
      }

      if (current == NULL && this->head == NULL) {
        this->head = this->tail = node;
      } else if (current == NULL) { //Ran off the end, insert AS tail
        this->tail = prev->next = node;
        node->prev = prev; //prev=tail
      } else if (current == this->head) {
        node->next = this->head;
        current->prev = node;
        this->head = node;
      } else {
        node->next = current;
        node->prev = prev;
        current->prev = node;
        prev->next = node;
      }

      ++(this->my_size);
    }

};

BDLIB_NS_END
#endif /* !_W_SORTEDLIST_H */ 
