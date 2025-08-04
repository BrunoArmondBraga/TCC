#ifndef AUXILIARY_H
#define AUXILIARY_H

#include "RedBlack.h"

#include <vector>
#include <cmath>

Node* build_initial_tango(int n);
//Given a int n, recursively build a complete binary search tree in level-order
//built from left to right.

Node* cut(Node* u, int min_depth);
//Given the root of a valid red-black tree and a int min_depth, put all the nodes of this tree
//that have depth greater than min_depth in another valid red-black tree and return the root
//of the valid red-black tree with only nodes with depth less than or equal to min_depth

Node* glue(Node* u, int searched_item);
//Given a node u, root of a tree, and an int searched item, glue together the subtree of u
//the tree with the node with key searched_item in a single valid red-black tree and return
//its root

Node* search_in_tango(Node* root, int searched_item, bool& found);
//Given a root u and a int searched, search in tango for that key, changing the red-black
//trees accordingly and return the root of the tango at the end of the execution

#endif