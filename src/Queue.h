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
#ifndef _BD_QUEUE_H
#define _BD_QUEUE_H 1

#include "bdlib.h"
#include "List.h"

BDLIB_NS_BEGIN

template <class T>
/**
 * @class Queue
 * @brief A FIFO Queue, what else!
 */
class Queue : public List<T> {
  class QueueIterator;
  typedef typename List<T>::Node Node;

  public:
    typedef QueueIterator iterator;
    typedef T iterator_type;

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
    inline friend Queue<iterator_type>& operator>> (Queue<iterator_type>& queue, iterator_type& item) {
      item = queue.dequeue();
      return queue;
    }

    /**
     * @brief Peek at the first element, but don't dequeue it
     * @todo Throw an exception?
     */
    inline const iterator_type peek() const {
      return this->isEmpty() ? iterator_type() : this->tail->ptr;
    }


    /*
     * @brief Enqueue an element into the queue
     * @param item The element to enqueue
     */
    inline void enqueue(const iterator_type &item) {
      insert(item);
    };

    /**
     * @sa push
     */
    inline friend Queue<iterator_type>& operator<< (Queue<iterator_type>& queue, const iterator_type& item) {
      queue.enqueue(item);
      return queue;
    }

  private:
    class QueueIterator : public Iterator<iterator_type> {
      friend class Queue;
      private:
        Queue *queue;
        Node *current;
        bool end;

        QueueIterator(Queue &_queue, bool _end = 0) : Iterator<iterator_type>(),
                                                      queue(&_queue),
                                                      current(_end ? _queue.tail : _queue.head), /* head is beginning */
                                                      end(_end) {
        };
      public:
        QueueIterator(const iterator& iter) : Iterator<iterator_type>(),
                                              queue(iter.queue),
                                              current(iter.current),
                                              end(iter.end) {
        };
        QueueIterator() : Iterator<iterator_type>(),
                          queue(NULL),
                          current(NULL),
                          end(0) {
        };

        iterator& operator =(const iterator& iter) {
          queue = iter.queue;
          current = iter.current;
          end = iter.end;
          return *this;
        }

        virtual ~QueueIterator() { }

        virtual void remove() {
          Node *nextValue = end == 0 ? current->next : current->prev;

          queue->deleteNode(current);

          current = nextValue;
        }

        virtual operator bool() { return (current != NULL); };
        virtual operator iterator_type () { return operator*(); };

        virtual iterator_type& operator *() { return (iterator_type&)current->ptr; }

        //Postfix
        virtual iterator operator ++(int) {
          iterator tmp(*this);
          current = current->next;
          return tmp;
        }

        virtual iterator& operator ++() {
          current = current->next;
          return *this;
        }

        //Postfix
        virtual iterator operator --(int) {
          iterator tmp(*this);
          current = current->prev;
          return tmp;
        }

        virtual iterator& operator --() {
          current = current->prev;
          return *this;
        }
    };
  public:
    inline iterator begin() { return iterator(*this); };
    inline iterator end() { return iterator(*this, 1); };
};

BDLIB_NS_END
#endif /* !_BD_QUEUE_H */
