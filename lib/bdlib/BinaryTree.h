/* BinaryTree.h
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
#ifndef _W_BINARYTREE_H
#define _W_BINARYTREE_H 1

#include "bdlib.h"
#include "Iterator.h"
BDLIB_NS_BEGIN

template <class Key, class Value>
/**
  * @class BinaryTree
  * @brief Binary tree data structure
  */
class BinaryTree {
  private:
    class Node {
      public:
        Node *left;
        Node *right;
        Key key;
        Value value;

        Node(Key k, Value v) {
          key = k;
          value = v;
          left = right = NULL;
        }
    };

    int my_size;
    Node *root;

    void insertNode(Node*& search, Node* node) {
      if (search == NULL) {
        search = node;
        ++my_size;
      }
      else if (node->key < search->key)
        insertNode(search->left, node);
      else if (node->key > search->key)
        insertNode(search->right, node);
    }

    void deleteNode(Node*& node) {
      if (node->left == NULL) {
        Node* temp = node->right;
        delete node;
        node = temp;
        --my_size;
      } else if (node->right == NULL) {
/* in here */
        Node* temp = node->left;
        delete node;
        node = temp;
        --my_size;
      } else {
        //Two children, find max of left subtree and swap
        Node*& temp = node->left;

        while (temp->right != NULL)
         temp = temp->right;

        node->key = temp->key;
        node->value = temp->value;
        deleteNode(temp);
      }
    }

    Node* &fetchNode(Node** search, Key key) {
      while ((*search) != NULL) {
        if (key < (*search)->key)
          search = &(*search)->left;
        else if (key > (*search)->key)
          search = &(*search)->right;
        else {
          return *search;
        }
      }
      return *search;
    }

  public:
    /** 
      * @brief Checks whether the given key is in the tree
      * @param key The key to search for
      * @return true/false depending on found/not-found
      */
    bool contains(Key key) {
      if (isEmpty())
        return false;
      if (fetchNode(&root, key))
        return true;
      return false;
    }

    /**
      * @brief Returns the current size of the tree
      * @return The current size of the tree
      */
    size_t size() const { return my_size; };
    bool isEmpty() const { return size() == 0; };

    class IteratorHelper : public Iterator<Value> {
      private:
        int index;
        int my_size;
        Value *storage;

        void fillArray(int &i, const Node *node) {

          if (node == NULL) return;
          fillArray(i, node->left);
          storage[i++] = node->value;
          fillArray(i, node->right);
        }

//        IteratorHelper& operator=(const IteratorHelper&); ///<Block implicit copy constructor
      public:
        IteratorHelper(Node *node, int size) : Iterator<Value>(), index(0), my_size(size), storage(new Value[my_size]) {
          int i = 0;
          fillArray(i, node);
        }

        IteratorHelper(const IteratorHelper &iter) : Iterator<Value>(), index(0), my_size(iter.my_size), storage(new Value[iter.my_size]) {
          for (int i = 0; i < my_size; i++)
            storage[i] = iter.storage[i];
        }

        IteratorHelper &operator = (const IteratorHelper &iter) {
          index = iter.index;
          my_size = iter.my_size;
          for (int i = 0; i < my_size; i++)
            storage[i] = iter.storage[i];
          return *this;
        }

        virtual ~IteratorHelper() {
          delete[] storage;
        }

        virtual bool hasNext() {
          return (index < my_size);
        }

        virtual Value next() {
          return storage[index++];
        }
/* not done
        virtual const Value& operator ++() { //prefix
          Value v = Q.front();
          Q.pop();
          return v;
        }

        virtual const Value operator ++(int) { //postfix

        }
*/    
    };

    typedef IteratorHelper iterator;

    iterator begin() {
      return IteratorHelper(root, my_size);
    }

  public:
    BinaryTree() : my_size(0), root(NULL) {};
    virtual ~BinaryTree() {};

    /**
      * @brief insert Key/Value pair into tree
      * @param key The key to be inserted
      * @param value The value to be inserted
      * @post The tree's size is increased by 1 if the element was not already in the tree
      */
    bool insert(Key key, Value value) {
      if (contains(key)) 
        return false;
      Node *node = new Node(key, value);
      insertNode(root, node);
      return true;
    }

    /** 
      * @brief Remove the given key (and its value) from the tree
      * @param key The key to be searched/removed
      * @return Whether or not the key was removed
      */
    bool remove(Key key) {
      Node* &node = fetchNode(&root, key);

      if (node != NULL) {
        deleteNode(node);
        return true;
      }
      return false;
    }

    /**
      * @brief Find a value in the tree from the key given
      * @param key The key to search for
      * @return The value of the key searched for, or NULL if not found
      */
    Value getValue(Key key) {
      Value empty;
      if (isEmpty()) return empty;

      Node* node = fetchNode(&root, key);
      if (node)
        return node->value;
      return empty;
    }

#ifdef DISABLED
    /**
      * @brief Find the first key from the given value in the tree
      * @param key The value to search for
      * @return The key of the value searched for, or NULL if not found
      */
    Value getKey(Value value) {
      Key empty;
      if (isEmpty()) return empty;

      iterator iter = begin();
      while (iter.hasNext()) {
        Node *node = iter.next();
        if (node->value == value)
          return node->key;
      }
      return empty;
    }
#endif
};

BDLIB_NS_END
#endif /* !_W_BINARYTREE_H */ 
