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

#include "String.h"
BDLIB_NS_BEGIN


template <class Key, class Value>
/**
  * @class BinaryTree
  * @brief Binary tree data structure
  */
class BinaryTree {
  class BinaryTreeIterator;

  public:
    typedef BinaryTreeIterator iterator;
    typedef KeyValue<Key, Value> iterator_type;

  private:
    class Node {
      public:
        Node *left;
        Node *right;
        iterator_type kv;

        Node(const Key k, const Value v) : left(NULL), right(NULL), kv(k, v) { };
        Node(const Node& n) : left(n.left ? new Node(*(n.left)) : NULL),
                              right(n.right ? new Node(*(n.right)) : NULL),
                              kv(n.kv) {};
    };

    int my_size;
    Node *root;

    /** 
      * @brief Returns a reference to the node with a matching key
      * @param search Where to start the search in the tree
      * @param key The key to search for
      * @return The node searched for, if found, otherwise NULL
      */
    Node* &fetchNode(Node* const* search, const Key &key) const {
      while ((*search) != NULL) {
        if (key < (*search)->kv.key())
          search = &(*search)->left;
        else if (key > (*search)->kv.key())
          search = &(*search)->right;
        else {
          return (Node*&) *search;
        }
      }
      return (Node*&) *search;
    }

    /** 
      * @brief Insert a node into the given tree
      * @param search The tree to insert the node into
      * @param node The node to insert
      * @sa fetchNode
      * This first looks for the where the node SHOULD be in the tree.
      * It will of course fail, but this is the location to place the node.
      */
    void insertNode(Node** search, Node* const node) {
      Node* &insertAt = fetchNode(search, node->kv.key());
      insertNode(insertAt);
    }

    void insertNode(Node*& insertAt, Node* const node) {
      insertAt = node;
      ++my_size;
    }

    /**
      * @brief Delete a node from the tree. (possibly recursive)
      * @param node A reference to the node to delete.
      */
    void deleteNode(Node*& node) {
      if (node == NULL) 
        return;
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
        Node** temp = &(node->left);

        while ((*temp)->right != NULL)
          temp = &(*temp)->right;
        node->kv = (*temp)->kv;
        deleteNode(*temp);
      }
    }
  public:
    /** 
      * @brief Checks whether the given key is in the tree
      * @param key The key to search for
      * @return true/false depending on found/not-found
      */
    bool contains(const Key &key) const {
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
    operator bool() const { return !isEmpty(); };

  public:
    BinaryTree() : my_size(0), root(NULL) {};
    BinaryTree(const BinaryTree& tree) : my_size(tree.my_size), root(new Node(*(tree.root))) {};

    BinaryTree& operator = (const BinaryTree& tree) {
      if (&tree != this) {
        clear();
        my_size = tree.my_size;
        root = new Node(*(tree.root));
      }
      return *this;
    }

    /**
      * @brief Destructor
      * @sa deleteNode
      * This simply deletes the root node over and over until the root is NULL.
      * There may possibly be a quicker way that does not use recursion (deleteNode does) but this seems the simplest.
      */
    virtual ~BinaryTree() { clear(); };

    /**
      * @brief insert Key/Value pair into tree
      * @param key The key to be inserted (or replaced)
      * @param value The value to be inserted (or replaced)
      * @post The tree's size is increased by 1 if the element was not already in the tree
      */
    bool insert(const Key &key, const Value &value) {
      Node*& rNode = fetchNode(&root, key);
      if (rNode)
        rNode->kv = iterator_type(key, value);
      else {
        Node *newNode = new Node(key, value);
        insertNode(rNode, newNode);
      }
      return true;
    }

    /**
      * @brief Associate array type accessor (rvalue)
      * @param key The key to search for
      */
    const Value operator [](const Key& key) const {
      Node*& node = fetchNode(&root, key);
      if (node)
        return node->kv.value();
      return Value();
    }

    /**
      * @brief Associate array type accessor (lvalue)
      * @param key The key to search for
      * @sa insert
      * @code tree["key"] = "value";
      * If the key is not in the tree, it is inserted, and the value set to the rvalue given.
      */
    Value& operator [](const Key& key) {
      Node*& node = fetchNode(&root, key);
      if (!node)
        insertNode(node, new Node(key, Value()));
      //The key was inserted at the return node's address! node is now guranteed to be NON-NULL ! */
      return node->kv.v;
    }

    /** 
      * @brief Remove the given key (and its value) from the tree
      * @param key The key to be searched/removed
      * @return Whether or not the key was removed
      */
    bool remove(const Key &key) {
      Node* &node = fetchNode(&root, key);

      if (node != NULL) {
        deleteNode(node);
        return true;
      }
      return false;
    }

    void clear() {
      while (root != NULL)
        deleteNode(root);
    }

    /**
      * @brief Find a value in the tree from the key given
      * @param key The key to search for
      * @return The value of the key searched for, or NULL if not found
      */
    Value getValue(const Key &key) const {
      Value empty;
      if (isEmpty()) return empty;

      Node* node = fetchNode(&root, key);
      if (node)
        return node->kv.value();
      return empty;
    }

#ifdef DISABLED
    /**
      * @brief Find the first key from the given value in the tree
      * @param key The value to search for
      * @return The key of the value searched for, or NULL if not found
      */
    Value getKey(const Value &value) const {
      Key empty;
      if (isEmpty()) return empty;

      for (iterator iter = begin(); iter; ++iter) {
        if (iter->value() == value)
          return iter->key();
      }
      return empty;
    }
#endif

  private:
    class BinaryTreeIterator : public Iterator<iterator_type> {
      friend class BinaryTree;
      typedef BinaryTree<Key, Value> Tree;
      private:
        Tree* tree;
        int index;
        int my_size;
        iterator_type* storage;

        void fillArray(int& i, const Node* node) {
          if (node == NULL) 
            return;
          fillArray(i, node->left);
          storage[i++] = iterator_type(node->kv);
          fillArray(i, node->right);
        }

        BinaryTreeIterator(Tree* t, Node* node, int size, bool end = 0) : Iterator<iterator_type>(), 
                                                                          tree(t),
                                                                          index(end ? size - 1: 0), 
                                                                          my_size(size), 
                                                                          storage(new iterator_type[size]) {
          int i = 0;
          fillArray(i, node);
        }
        //BinaryTreeIterator& operator=(const BinaryTreeIterator&); ///<Block implicit copy constructor
      public:
        BinaryTreeIterator(const iterator& iter) : Iterator<iterator_type>(), 
                                                   tree(iter.tree),
                                                   index(iter.index), 
                                                   my_size(iter.my_size), 
                                                   storage(new iterator_type[iter.my_size]) {
          for (int i = 0; i < my_size; ++i)
            storage[i] = iter.storage[i];
        }

        iterator& operator =(const iterator& iter) {
          if (&iter != this) {
            if (my_size != iter.my_size) {
              delete[] storage;
              storage = new iterator_type[iter.my_size];
            }

            tree = iter.tree;
            index = iter.index;
            my_size = iter.my_size;

            for (int i = 0; i < my_size; ++i)
              storage[i] = iter.storage[i];
          }
          return *this;
        }

        virtual ~BinaryTreeIterator() { delete[] storage; }

        virtual void remove() {
          tree->remove(storage[index].key());
/*
          --my_size;
          for (int i = index; i < my_size; ++i)
            storage[i] = storage[i + 1];
          //The iterator is 'invalid' now, but the index is shifter down in case iter++ is called
          --index;
*/
        }

        virtual operator bool() { return (index > 0 && index < my_size); };

        virtual operator iterator_type () { return operator*(); };
        virtual iterator_type& operator *() { return storage[index]; }

        //Postfix
        virtual iterator operator ++(int) {
          iterator tmp(*this);
          ++index;
          return tmp;
        }

        virtual iterator& operator ++() {
          ++index;
          return *this;
        }

        //Postfix
        virtual iterator operator --(int) {
          iterator tmp(*this);
          --index;
          return tmp;
        }

        virtual iterator& operator --() {
          --index;
          return *this;
        }
    };
  public:
    iterator begin() { return iterator(this, root, my_size); };
    iterator end() { return iterator(this, root, my_size, 1); };

};

BDLIB_NS_END
#endif /* !_W_BINARYTREE_H */ 
