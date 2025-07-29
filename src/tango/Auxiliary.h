#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

class Node{ //This is a red-black tree node 
    public:
        int key;
        bool is_black;
        int black_height = 1;
        bool is_root = false;
        int depth = 0;
        int min_depth;
        int max_depth;
        Node *parent;
        Node *left;
        Node *right;
        ~Node(){
            if(left != nullptr){
                delete left;
            }
            if(right != nullptr){
                delete right;
            }
        }
        Node(int key, int depth = 0){
            is_black = false;
            black_height = 1;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            this->key = key;
            this->depth = depth;
            min_depth = depth;
            max_depth = depth;
        }
        Node(Node* parent,int key,int depth){
            is_black = false;
            left = nullptr;
            right = nullptr;
            this->depth = depth;
            this->max_depth = depth;
            this->min_depth = depth;
            if(parent == nullptr){
                this->parent = nullptr;
            }
            else{
                this->parent = parent;
            }
            this->key = key;
        }
};

struct split_data
{
    Node* left_tree;
    Node* right_tree;
};

int max(int a, int b, int c);
//Given integers a,b,c, returns the biggest of all three

int min(int a, int b, int c);
//Given integers a,b,c, returns the smallest of all three 


void fix_depths(Node* u);
//Given a node u, recalculate u->min/max_depth based on its children


Node *left_rotate(Node *u);
//Given a node u, rotate it to the left and return the parent node


Node *right_rotate(Node *u);
//Given a node u, rotate it to the right and return the parent node

int predecessor(Node* root, int i);
//Given the root of a tree and an integer i, return the biggest key in the tree that is smaller than i.


int sucessor(Node* root, int i);
//Given the root of a tree and an integer i, return the biggest key in the tree that is smaller than i.

void Fix(Node* son);
//Given a red node, fix the structure/colors of the tree to be valid

Node *add_RB(Node *root, int key, Node* to_be_positioned, int depth = 0);
//Insert a key or a node to a tree. If to_be_positioned = nullptr, add key, add to_be_positioned otherwise.
//Returns the new root of the tree.

Node* add(Node* root, int key, Node* to_be_positioned, int depth = 0);
//Given a tree, and key or node, add that key/node to the tree and returns the new root.


void debug_rec(Node *u, int i = 0);
//Recursively print the tree in a tree-shaped format

void debug_rec_trees(Node *u, int i, stack<Node*>& parts, int& sum);
//Recursively print the tree in a tree-shaped format

int size_rec(Node* root);
//Calculates the number of nodes of a tree

int size(Node* root);
//Gives a node, return the number of nodes in its subtree

void print(Node* root);

void print_trees(Node* root, bool all = true);
//Given a root and a bool all, print the tree taking into account the different trees within it
//If all is equal to true, print the whole tree, if all is equal to false, print just the first tree 

void add_to_tango(Node* root, Node* new_piece);
//Given two trees, add the second tree to a leaf postion of the first one

Node* Join(Node *u, Node* k, Node *v);
//Given two trees u and v, and a node k, such that all nodes in u have keys smaller than k's key and all
//nodes in v have keys greater that k's key, then return a valid tree that has the nodes of all three trees

split_data Split(Node *root, float pivot);
//Given a tree and a float pivot, return two trees: one that has only nodes that have keys smaller or
//equal to pivot, and one with keys exclusively greater than pivot

Node* build_complete_BST(int n, int offset = 0, int depth = 0);

Node* build_tango_tree(Node* u);

Node* bring_to_front(Node* root, int item);

Node* concatenate(Node* root);
//Given a node that has a valid tree in its left and right subtree and the nodes in
//its left subtree have keys smaller than the root and all the nodes in the its right
//subtree have keys greater than the root, then return a valid tree with all nodes 

Node* cut(Node* u, int min_depth);
//Given the root of a tree and a int min_depth, put all the nodes in this tree that
//have depth greater that min_depth in another tree

Node* glue(Node* u, int searched_item);
//Given a node u, root of a tree, and an int searched item, glues together the trees
//u and the tree that has the node with key searched item

Node* tango(Node* u, int min_depth, int searched_item);

Node* search_in_tango(Node* root, int searched_item, bool& found);

#endif