/* Queue.h
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
#ifndef _W_QUEUE_H
#define _W_QUEUE_H 1

#include "bdlib.h"
#include "List.h"

BDLIB_NS_BEGIN

template <class T>
/**
 * @class Queue
 * @brief A FIFO Queue, what else!
 */
class Queue : public List<T> {
  typedef T iterator_type;
  typedef typename List<iterator_type>::Node Node;

  public:
    Queue() : List<iterator_type>() {};
    virtual ~Queue() {};

    /**
     * @brief Dequeue an element from the tail
     */
    iterator_type dequeue() {
      iterator_type item = this->tail->ptr;
      this->deleteNode(this->tail);
      return item;
    };

    /**
     * @sa dequeue
     */
    friend Queue<iterator_type>& operator>> (Queue<iterator_type>& queue, iterator_type& item) {
      item = queue.dequeue();
      return queue;
    }


    /*
     * @brief Enqueue an element into the queue
     * @param item The element to enqueue
     */
    void enqueue(const iterator_type &item) {
      insert(item);
    };

    /**
     * @sa push
     */
    friend Queue<iterator_type>& operator<< (Queue<iterator_type>& queue, const iterator_type& item) {
      queue.enqueue(item);
      return queue;
    }
};

BDLIB_NS_END
#endif /* !_W_QUEUE_H */
