/*
 * Copyright (c) 2006-2013, Bryan Drewery <bryan@shatow.net>
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

/* Stack.h
 */

#ifndef _BD_STACK_H
#define _BD_STACK_H 1

#include "bdlib.h"
#include "List.h"

BDLIB_NS_BEGIN

template <class T>
/**
 * @class Stack
 * @brief A FILO Stack, what else!
 */
class Stack : public List<T> {
  typedef T iterator_type;
  typedef typename List<iterator_type>::Node Node;

  public:
    Stack() : List<iterator_type>() {};
    virtual ~Stack() {};

    /**
     * @brief Pop an element off the top of the stack
     */
    iterator_type pop() {
      iterator_type item = this->head->item;
      this->remove(item);
      return item;
    };

    /**
     * @sa pop
     */
    inline friend Stack<iterator_type>& operator>> (Stack<iterator_type>& stack, iterator_type& item) {
      item = stack.pop();
      return stack;
    }

    /**
     * @brief Peek at the first element, but don't pop it off
     * @todo Throw an exception?
     */
    inline const iterator_type peek() const {
      return this->isEmpty() ? iterator_type() : this->head->item;
    }

    /**
     * @brief Push an element on to the top of stack
     * @param item The element to push back
     */
    inline void push(const iterator_type &item) {
      this->insert(item);
    };

    /**
     * @sa push
     */
    inline friend Stack<iterator_type>& operator<< (Stack<iterator_type>& stack, const iterator_type& item) {
      stack.insert(item);
      return stack;
    }
};

BDLIB_NS_END
#endif /* _BD_STACK_H */ 
