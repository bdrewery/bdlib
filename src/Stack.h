/* Stack.h
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
      iterator_type item = this->head->ptr;
      this->remove(item);
      return item;
    };

    /**
     * @sa pop
     */
    friend Stack<iterator_type>& operator>> (Stack<iterator_type>& stack, iterator_type& item) {
      item = stack.pop();
      return stack;
    }

    /**
     * @brief Peek at the first element, but don't pop it off
     * @todo Throw an exception?
     */
    const iterator_type peek() const {
      return this->isEmpty() ? iterator_type() : this->head->ptr;
    }

    /*
     * @brief Push an element on to the top of stack
     * @param item The element to push back
     */
    void push(const iterator_type &item) {
      insert(item);
    };

    /**
     * @sa push
     */
    friend Stack<iterator_type>& operator<< (Stack<iterator_type>& stack, const iterator_type& item) {
      stack.insert(item);
      return stack;
    }
};

BDLIB_NS_END
#endif /* !_BD_STACK_H */ 
