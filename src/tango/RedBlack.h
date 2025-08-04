#ifndef REDBLACK_H
#define REDBLACK_H

#include <iostream>
#include <stack>
#include <algorithm>

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

struct split_data
{
    Node* left_tree;
    Node* right_tree;
};

void fix_depths(Node* u);
//Given a node u, recalculate u->min/max_depth based on its children

int predecessor(Node* root, int i);
//Given the root of a tree and an integer i, return the biggest key in the tree that is smaller than i

int sucessor(Node* root, int i);
//Given the root of a tree and an integer i, return the smallest key in the tree that is bigger than i

int size(Node* root);
//Gives a node, return the number of nodes in its subtree

void print(Node* root);
//Given a root, print the tree

void print_trees(Node* root, bool all = true);
//Given a root and a bool all, print the tree taking into account the different trees within it
//If all is equal to true, print the whole tree, if all is equal to false, print just the first tree

Node* bring_to_root(Node* root, int item);
//Given a node r and a key x, such that there is a node v with key x in r's subtree, than 
//re-arrange r's subtree so the node v is at the root and the left subtree of v is a valid red-black
//tree with only nodes with keys smaller than x and the right subtree of v is a valid red-black tree
//with only nodes with keys greater than x. Return node v

Node* concatenate(Node* root);
//Given a node v that has a valid tree in its left and right subtrees and the nodes in
//its left subtree have keys smaller than the root and all the nodes in the its right
//subtree have keys greater than the root, then return a valid tree with all nodes

#endif