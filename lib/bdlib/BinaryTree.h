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

    Node*& findNode(Node*& search, Key key) {
/*
      Node*& temp = search;

      while (temp != NULL) {
        if (key < temp->key)
          temp = temp->left;
        else if (key > temp->key)
          temp = temp->right;
        else {
          std::cout << "Found key: " << search->key << std::endl;
          return temp;
        }
      }
      return temp;
*/
      if (key < search->key)
        return findNode(search->left, key);
      else if (key > search->key)
        return findNode(search->right, key);
      else {
        return search;
      }
    }

  public:
    class IteratorHelper : public Iterator<Value> {
      private:
        int index;
        int my_size;
        Value *storage;

        void fillArray(const Node *node) {
          static int i = 0;

          if (node == NULL) return;
          fillArray(node->left);
          storage[i++] = node->value;
          fillArray(node->right);
        }
      public:
        IteratorHelper(Node *node, int _size) {
          index = 0;
          my_size = _size;
          storage = new Value[my_size];
          fillArray(node);
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
    void insert(Key key, Value value) {
      Node *node = new Node(key, value);
      insertNode(root, node);
    }

    /** 
      * @brief Remove the given key (and its value) from the tree
      * @param key The key to be searched/removed
      */
    void remove(Key key) {
      Node*& node = findNode(root, key);

      if (node != NULL)
        deleteNode(node);
    }

    /**
      * @brief Find a value in the tree from the key given
      * @param key The key to search for
      * @return The value of the key searched for, or NULL if not found
      */
    Value find(Key key) {
      Node*& node = findNode(root, key);
      if (node)
        return node->value;
      return NULL;
    }

    /**
      * @brief Returns the current size of the tree
      * @return The current size of the tree
      */
    size_t const size() { return my_size; };
};

BDLIB_NS_END
#endif /* !_W_BINARYTREE_H */ 
