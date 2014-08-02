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

/* SortedList.h
 */

#ifndef _BD_SORTEDLIST_H
#define _BD_SORTEDLIST_H 1

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
     * @param item The element to insert
     * This operation is O(n/2)
     */
    virtual void insert(const iterator_type& item) {
      /*
       * 1) list = NULL
       * 2) list = (X)
       * 3) list = (X)->(Y)
       * 4) list = (W)->(X)->(Y)
       */
      Node* node = new Node(item), *prev = nullptr, *current = this->head;
      while (current != nullptr && item < current->item) {
        prev = current;
        current = current->next;
      }

      if (current == nullptr && this->head == nullptr) {
        this->head = this->tail = node;
      } else if (current == nullptr) { //Ran off the end, insert AS tail
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
#endif /* _BD_SORTEDLIST_H */ 
/* vim: set sts=2 sw=2 ts=8 et: */
