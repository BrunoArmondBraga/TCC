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

        //Used in join operation
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
//Given a node u, calculate u->min/max_depth based on its children

int size(Node* root);
//Gives a node, return the number of nodes in its subtree

void print(Node* root);
//Given a root, print the tree

void print_trees(Node* root, bool all = true);
//Given a root and a bool all, print the tree taking into account the different trees within it
//If all is equal to true, print the whole tree, if all is equal to false, print just the first tree

pair_of_trees remove_min(Node* root);
//Given the root of a valid redblack-tree, remove the node with minimum key of this tree
//and return the pair {remaining tree, min_node} 

pair_of_trees split(Node* root, float pivot);
//Given the root of a tree and a float pivot, return a pair of tree that has its left tree as a valid
//red-black tree with only nodes with key less than pivot and has its right tree as a valid red-black
//tree with only nodes with key greater than pivot

Node* join(Node *u, Node* k, Node *v);
//Given two trees u and v, and a node k, where all nodes in u have keys smaller than k's key and all nodes
//in v have keys greater that k's key, then return a valid tree that has the nodes of all three trees

#endif