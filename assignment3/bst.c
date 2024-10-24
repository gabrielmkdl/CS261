/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Gabriel de Leon
 * Email: deleong@oregonstate.edu
 */

#include <stdlib.h>

#include "bst.h"
#include "stack.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  
  struct bst* bst = malloc(sizeof(struct bst));
  bst->root = NULL;

  return bst;
}

/*
 * This function frees up all the memory allocated to the nodes in the bst
 *
 * Params:
 *   node - node we want to free, should start with root and then iterate throughout the list
 */
void node_free(struct bst_node* node) {

  //iterates through left side
  if(node->left != NULL) {
    node_free(node->left);
  }

  //iterates through right side
  if(node->right != NULL) {
    node_free(node->right);
  }

  //frees node itself
  free(node);
  node = NULL;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst* bst) {

  //frees all memory allocated to the nodes in the bst
  node_free(bst->root);

  //frees the bst itself
  free(bst);
  bst = NULL;
}

/*
 * This function should recursively call this function whenever there is a node to the left and/or right of 
 * the current node and then return the size of the whole tree
 * 
 * Params:
 *   bst_node - the node that we are currently looping through to see if the right and left node are NULL or defined
 */
int bst_count(struct bst_node* root) {

  //counter for size of bst
  int count = 1;

  //checks if there is a node to the left and adds to count if so and recursively calls the function with the left node
  if(root->left != NULL) {
    count += bst_count(root->left);
  }

  //checks if there is a node to the right and adds to count if so and recursively calls the function with the right node
  if(root->right != NULL) {
    count += bst_count(root->right);
  }

  return count;
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
int bst_size(struct bst* bst) {

  //if bst is empty
  if(bst == NULL) {
    return 0;
  }

  else {
    return bst_count(bst->root);
  }
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {  

  //keeps track of previous node that's the parent
  struct bst_node* parent;
  
  //new node
  struct bst_node* child;

  //if the tree is empty
  if(bst->root == NULL) {
    child = malloc(sizeof(struct bst_node));
    bst->root = child;
    child->key = key;
    child->value = value;
    child->left = NULL;
    child->right = NULL;
  }

  //tree has at least a root
  else {

    child = bst->root;

    //runs until we find an empty place where we can add a node
    while(child != NULL) {
      
      parent = child;
      
      //traverses to the left if key is less than curr node key
      if(key < child->key) {
        child = child->left;
      }

      //travereses to the right if key is greater or equal than curr node key
      else {
        child = child->right;
      }
    } 

    //new node
    child = malloc(sizeof(struct bst_node));
    child->key = key;
    child->value = value;
    child->right = NULL;
    child->left = NULL;

    //adds to left
    if(key < parent->key) {
      parent->left = child;
    }

    //adds to right
    else {
      parent->right = child;
    }
  }
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) { 

  //parent node
  struct bst_node* parent = NULL;
  
  //current node that traverses until finds wanted node
  struct bst_node* current = bst->root;

  //finds wanted node
  while(current->key != key) {

    parent = current; 
    
    //traverses left
    if(key < current->key) {
      current = current->left;
    }

    //traverses right
    else{
      current = current->right;
    }
  }

  //no children
  if(current->right == NULL && current->left == NULL) {
    
    //if curr is left child
    if(key < parent->key) {
      parent->left = NULL;
    }

    //if curr is right child
    else {
      parent->right = NULL;
    }
  }

  //one child
  else if(current->right == NULL || current->left == NULL) {

    //left children of parent is the removed node
    if(key < parent->key) {

      //removed node has a left children
      if(current->left != NULL) {
        parent->left = current->left;
      }

      //removed node has a right children
      else {
        parent->left = current->right;
      }
    }

    //right children of parent is the removed node 
    else {

      //removed node has a left children
      if(current->left != NULL) {
        parent->right = current->left;
      }

      //removed node has a right children
      else {
        parent->right = current->right;
      }
    }
  }
  
  //two children
  else {

    //successor node
    struct bst_node* successor = current->right;

    //parent of successor
    struct bst_node* sp = parent;
  
    //finds successor node
    while(successor->left != NULL) {
      sp = successor;
      successor = successor->left;
    }

    //transfers children from curr to successor and successor to parent if needed
    if(current->left != successor) {
      successor->left = current->left;
    }

    sp->left = successor->right;
    
    if(current->right != successor) {
      successor->right = current->right;
    }

    //if root was removed
    if(current == bst->root) {
      bst->root = successor;
    }

    //if parent's left child was removed
    else if(successor->key < sp->key) {
      sp->left = successor;
    }

    //if parent's right child was removed
    else {
      sp->right = successor;
    }
  }

  free(current);
}

/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {

  struct bst_node* current = bst->root;

  //loops through the whole bst either finding the value or not
  while(current != NULL) {

    //node found
    if(current->key == key) {
      return current->value;
    }

    //travereses left if key is less than curr key
    else if(key < current->key) {
      current = current->left;
    }

    //traverses right if key greater than or equal to curr key
    else{
      current = current->right;
    }
  }
  
  return NULL;
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/
/*
 * This function finds the maximum depth of the tree
 *
 * Params:
 *   bst - the root of the bst we're trying to find the height of and then iterating through all the nodes
 *
 * Return:
 *   Should return the height of bst.
 */
int maxDepth(struct bst_node* root) {

    //one count for left tree another for the right
    int count = 0;
    int count1 = 0;
    
    //if the tree is empty
    if(root == NULL) {
        return -1;
    }  

    //traverses left tree
    if(root->left != NULL) {
        count++;
        count = count + maxDepth(root->left);
    }

    //traverses right tree
    if(root->right != NULL) {
        count1++;
        count1 = count1 + maxDepth(root->right);
    }

    //returns the highest level, left side or right side
    if(count > count1) {
        return count;
    }

    else {
        return count1;
    }
}

/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
 int bst_height(struct bst* bst) {
    return maxDepth(bst->root);
 }

/*
 * This function finds if there is a sum path from root to leaf that matches the sum that we were given
 *
 * Params:
 *   node - the current node we are iterating, should start as the root of the bst
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   returns a 1 if a sum path was found otherwise returns a 0
 */
int add(struct bst_node* node, int sum) {

  //Base case
  if(node == NULL) {
    return 0;
  }

  //if there is a path sum, then the sum would hit 0 once the function iterates through the right leaf
  sum -= node->key;

  //if the last subtraction was at a leaf node and is equal to zero then we found a working sum path
  if(node->left == NULL && node->right == NULL && sum == 0) {
    return 1;
  }

  //iterates through the left and right tree and if one of them is the right path then the function stops calling itself due to the OR statement
  return add(node->left, sum) || add(node->right, sum);
}

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
int bst_path_sum(struct bst* bst, int sum) {
  return add(bst->root, sum);
}

/*
 * This function computes the sum of all the nodes in the given range 
 *
 * Params:
 *   node - current node we are iterating through, should be the root at the start
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in the bst between `lower` and `upper`.
 */
int range(struct bst_node* node, int lower, int upper) {
  
  int count = 0;

  //checks if node fits inside the range
  if(node->key >= lower && node->key <= upper) {
    count += node->key;
  }

  //checks if there is a left node
  if(node->left != NULL) {

    //checks if the left node is higher than the lower limit
    if(node->left->key >= lower) {
      count += range(node->left, lower, upper);
    }

    //if the left node is lower than lower limit, check to see if it has a right child and if that right child is higher than the lower limit
    else if(node->left->right != NULL) {
      if(node->left->right->key >= lower) {
        count += range(node->left->right, lower, upper);
      }
    }
  }

  //checks if there is a right node
  if(node->right != NULL) {

    //checks if the right node is lower than the upper limit
    if(node->right->key <= upper) {
      count += range(node->right, lower, upper);
    }

    //if the right node is higher than the upper limit, check to see if it has a left child and if that left child is lower than the upper limit
    else if(node->right->left != NULL) {
      if(node->right->left->key <= upper) {
        count += range(node->right->left, lower, upper);
      }
    }
  }

  return count;
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
int bst_range_sum(struct bst* bst, int lower, int upper) {
  return range(bst->root, lower, upper);
}

