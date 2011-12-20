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
 */
#ifndef _BD_QUEUE_H
#define _BD_QUEUE_H 1

#include <iterator>
#include "bdlib.h"
#include "List.h"

BDLIB_NS_BEGIN

template <class T> class QueueIterator;
template <class T> class QueueConstIterator;

template <class T>
/**
 * @class Queue
 * @brief A FIFO Queue, what else!
 */
class Queue : public List<T> {
  typedef typename List<T>::Node Node;

  public:
    friend class QueueIterator<T>;
    friend class QueueConstIterator<T>;

    typedef QueueIterator<T> iterator;
    typedef QueueConstIterator<T> const_iterator;
    typedef T value_type;

    typedef size_t             size_type;
    typedef std::ptrdiff_t     difference_type;
    typedef value_type*        pointer;
    typedef const value_type*  const_pointer;
    typedef value_type&        reference;
    typedef const value_type&  const_reference;


    Queue() : List<value_type>() {};
    virtual ~Queue() {};

    /**
     * @brief Dequeue an element from the tail
     */
    value_type dequeue() {
      value_type item = this->tail->item;
      this->deleteNode(this->tail);
      return item;
    };

    /**
     * @sa dequeue
     */
    inline friend Queue<value_type>& operator>> (Queue<value_type>& queue, reference item) {
      item = queue.dequeue();
      return queue;
    }

    /**
     * @brief Return the first element, but don't dequeue it
     */
    inline const_iterator front() {
      return this->isEmpty() ? this->end() : const_iterator(this, this->tail);
    }


    /**
     * @brief Enqueue an element into the queue
     * @param item The element to enqueue
     * @todo Make this take an iterator as a position to insert at
     */
    inline void enqueue(const_reference item) {
      this->insert(item);
    };

    /**
     * @sa push
     */
    inline friend Queue<value_type>& operator<< (Queue<value_type>& queue, const_reference item) {
      queue.enqueue(item);
      return queue;
    }

    inline iterator begin() { return iterator(this, this->tail); };
    inline iterator end() { return iterator(this, NULL); };
    inline const_iterator begin() const { return const_iterator(this, this->tail); };
    inline const_iterator end() const { return const_iterator(this, NULL, 1); };
};

template <class T>
class QueueIterator {
  friend class Queue<T>;
  friend class QueueConstIterator<T>;

  public:
    typedef Queue<T> Queue_t;
    typedef QueueIterator<T> iterator;
    typedef QueueConstIterator<T> const_iterator;
    typedef typename Queue_t::Node Node;
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T& reference;
    typedef T* pointer;

  private:
    Queue_t *queue;
    Node *current;

    QueueIterator(Queue_t* _queue, Node* start) : queue(_queue), current(start) {};
  public:
    QueueIterator() : queue(NULL), current(NULL) {};
    ~QueueIterator() { }

    reference operator *() const { return current->item; }
    pointer operator ->() const { return &(operator*()); }


    bool operator==(const iterator& iter) const { return iter.current == current; }
    bool operator!=(const iterator& iter) const { return iter.current != current; }

    //Postfix
    iterator operator ++(int) {
      iterator tmp(*this);
      current = current->next;
      return tmp;
    }

    iterator& operator ++() {
      current = current->next;
      return *this;
    }
};

template <class T>
class QueueConstIterator {
  friend class Queue<T>;
  friend class QueueIterator<T>;

  public:
    typedef Queue<T> Queue_t;
    typedef QueueIterator<T> iterator;
    typedef QueueConstIterator<T> const_iterator;
    typedef typename Queue_t::Node Node;
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef const T& reference;
    typedef const T* pointer;

  private:
    const Queue_t *queue;
    const Node *current;

    QueueConstIterator(const Queue_t* _queue, const Node* start) : queue(_queue), current(start) {};
  public:
    QueueConstIterator(const iterator& iter) : queue(iter.queue), current(iter.current) {};
    QueueConstIterator() : queue(NULL), current(NULL) {};
    ~QueueConstIterator() { }
/* This should be implemented in Queue.remove(iterator) */
/*
* void remove() {
      Node *nextValue = end == 0 ? current->next : current->prev;

      queue->deleteNode(current);

      current = nextValue;
    }
*/
    virtual explicit operator bool() const { return (current != NULL); };
    reference operator *() const { return current->item; }
    pointer operator ->() const { return &(operator*()); }


    bool operator==(const iterator& iter) const { return iter.current == current; }
    bool operator!=(const iterator& iter) const { return iter.current != current; }

    //Postfix
    const_iterator operator ++(int) {
      iterator tmp(*this);
      current = current->next;
      return tmp;
    }

    const_iterator& operator ++() {
      current = current->next;
      return *this;
    }
};

BDLIB_NS_END
#endif /* _BD_QUEUE_H */
