#ifndef REDBLACK_H
#define REDBLACK_H

#include <iostream>
#include <stack>
#include <algorithm>
#include <cassert>
#include <vector>

using namespace std;

class Node{ //This is a red-black tree node 
    public:
        int key;
        bool is_black = true;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;

        //Used in operation join
        int black_height = 2; //number of black nodes in the path from this node to a nullptr

        //Used in tango tree
        bool is_root = false;
        int depth = 0; //depth of node with this key in the auxiliary tree
        int min_depth = 0; //min depth in its subtree
        int max_depth = 0; //max depth in its subtree

        Node(int key, int depth)
        : key(key), depth(depth), min_depth(depth), max_depth(depth) {}

        ~Node() {
            delete left;
            delete right;
        }
};

struct pair_of_trees
{
    Node* left_tree;
    Node* right_tree;
};

void fix_depths(Node* u);
//Given a node u, recalculate u->min/max_depth based on its children

int size(Node* root);
//Gives a node, return the number of nodes in its subtree

void print(Node* root);
//Given a root, print the tree

void print_trees(Node* root, bool all = true);
//Given a root and a bool all, print the tree taking into account the different trees within it
//If all is equal to true, print the whole tree, if all is equal to false, print just the first tree

pair_of_trees remove_min(Node* root);

pair_of_trees split(Node* root, float pivot);

Node* join(Node *u, Node* k, Node *v);

#endif