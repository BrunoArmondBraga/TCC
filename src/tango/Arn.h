#ifndef ARN_H
#define ARN_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <stack>
#include <climits>

using namespace std;

class Node{        
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

int max(int a, int b, int c){
    //Given integers a,b,c, returns the biggest of all three
    if(a >= b && a >= c){
        return a;
    }
    if(b >= a && b >= c){
        return b;
    }
    return c;
}

int min(int a, int b, int c){
    //Given integers a,b,c, returns the smallest of all three 
    if(a <= b && a <= c){
        return a;
    }
    if(b <= a && b <= c){
        return b;
    }
    return c;
}

void fix_depths(Node* u){
    //Given a node u, recalculate u->min/max_depth based on its children
    if(u == nullptr){
        return;
    }
    int left_min_depth, left_max_depth, right_min_depth, right_max_depth;
    left_min_depth = left_max_depth = right_min_depth = right_max_depth = u->depth;

    if(u->left != nullptr && (!u->left->is_root)){
        left_min_depth = u->left->min_depth;
        left_max_depth = u->left->max_depth;
    }
    if(u->right != nullptr && (!u->right->is_root)){
        right_min_depth = u->right->min_depth;
        right_max_depth = u->right->max_depth;
    }

    u->min_depth = min(left_min_depth, u->depth, right_min_depth);
    u->max_depth = max(left_max_depth, u->depth, right_max_depth);
}

Node *left_rotate(Node *u){
    //Given a node u, rotate it to the left and return the parent node
    if((u == nullptr)|| (u->right == nullptr)){
        return u;
    }
    Node *aux = u->right;
    u->right = aux->left;
    if(u->right != nullptr){
        u->right->parent = u;
    }
    if(u->parent != nullptr){
        if(u->parent->key > u->key){
            u->parent->left = aux;
        }
        else{
            u->parent->right = aux;
        }
    }
    aux->left =u;
    aux->parent = u->parent;
    u->parent = aux;
    if(u->right != nullptr){
        u->right->parent = u;
    }
    if(u->is_root){
        u->is_root = false;
        aux->is_root = true;
    }
    fix_depths(u);
    fix_depths(aux);
    return aux;
}

Node *right_rotate(Node *u){
    //Given a node u, rotate it to the right and return the parent node
    if((u == nullptr)|| (u->left == nullptr)){
        return u;
    }
    Node *aux = u->left;
    u->left = aux->right;
    if(u->left != nullptr){
        u->left->parent = u;
    }
    if(u->parent != nullptr){
        if(u->parent->key > u->key){
            u->parent->left = aux;
        }
        else{
            u->parent->right = aux;
        }
    }
    aux->right = u;
    aux->parent = u->parent;
    u->parent = aux;
    if(u->left != nullptr){
        u->left->parent = u;
    }
    if(u->is_root){
        u->is_root = false;
        aux->is_root = true;
    }
    fix_depths(u);
    fix_depths(aux);
    return aux;
}

int predecessor(Node* root, int i){
    //Given the root of a tree and an integer i, return the biggest key in the tree that is smaller than i.
    int pred = -1;
    Node* current_node = root;

    while(current_node != nullptr && (!current_node->is_root)){
        if(current_node->key < i){ //go right
            if(current_node->key > pred){
                pred = current_node->key;
            }
            current_node = current_node->right;
        }
        else{
            current_node = current_node->left;
        }
    }
    return pred;
}

int sucessor(Node* root, int i){
    //Given the root of a tree and an integer i, return the biggest key in the tree that is smaller than i.
    int suc = -1;
    Node* current_node = root;

    while(current_node != nullptr && (!current_node->is_root)){
        if(current_node->key > i){ //go left
            if(current_node->key < suc || suc == -1){
                suc = current_node->key;
            }
            current_node = current_node->left;
        }
        else{
            current_node = current_node->right;
        }
    }
    return suc;
}

void Fix(Node* son){
    //Given a red node, fix the structure/colors of the tree to be valid
    Node* p = son->parent; 

    while(true){
        if(p->is_black == true){ //if parent is black, it is fixed
            break;
        }

        Node *grandparent = p->parent;
        if(grandparent==nullptr){
            p->is_black = true;
            p->black_height = son->black_height + 1;
            break;
        }

        //finding uncle!
        Node *uncle = nullptr;
        if(grandparent->right != nullptr){
            if(grandparent->right == p){
                uncle = grandparent->left;
            }
            else{
                uncle = grandparent->right;
            }
        }

        if(uncle != nullptr && uncle->is_black == false){ //case red uncle
            grandparent->is_black = false;
            p->is_black = uncle->is_black = true;
            son = grandparent;
            
            uncle->black_height = grandparent->black_height;
            p->black_height = grandparent->black_height;
            
            p = grandparent->parent;
            if(p == nullptr){
                break;
            }
        }
        else{ //case uncle is black
            if(p==grandparent->left && son == p->left){ //same side
                Node *q = right_rotate(grandparent);
                q->is_black = true;
                grandparent->is_black = false;

                int aux = q->black_height;
                q->black_height = grandparent->black_height;
                grandparent->black_height = aux;
                break;
            }
            else if(p==grandparent->left && son == p->right){ //different sides
                Node *q = left_rotate(p);
                Node *r = right_rotate(grandparent);
                r->is_black = true;
                grandparent->is_black = false;

                int aux = grandparent->black_height;
                grandparent->black_height = r->black_height;
                r->black_height = aux;
                break;
            }
            else if(p==grandparent->right && son ==p->right){ //same side
                Node *q = left_rotate(grandparent);
                q->is_black = true;
                grandparent->is_black = false;

                int aux = q->black_height;
                q->black_height = grandparent->black_height;
                grandparent->black_height = aux;
                break;
            }
            else{ //different sides
                Node *q = right_rotate(p);
                Node *r = left_rotate(grandparent);
                r->is_black = true;
                grandparent->is_black = false;

                int aux = grandparent->black_height;
                grandparent->black_height = r->black_height;
                r->black_height = aux;
                break;
            }
        }
    }
}

Node *add_RB(Node *root, int key, Node* to_be_positioned, int depth = 0){
    //Insert a key or a node to a tree. If to_be_positioned = nullptr, add key, add to_be_positioned otherwise.
    //Returns the new root of the tree.
    if(root == nullptr){
        if(to_be_positioned != nullptr){
            Node* insert = to_be_positioned;
            insert->is_black = true;
            insert->black_height = 2;
            insert->is_root = true;
        }
        Node *insert = new Node(nullptr,key,depth);
        insert->is_black = true;
        insert->black_height = 2;
        insert->is_root = true;
        return insert;
    }

    //sometimes add_RB is used by a JOIN, and needs to keep the upper_nodes other of the equation.
    Node* upper_nodes = root->parent;
    if(root->parent != nullptr){
        root->parent = nullptr;
    }

    int actual_depth = depth;
    if(to_be_positioned != nullptr){
        actual_depth = to_be_positioned->depth;
    }

    Node* p = root;
    bool found = false;

    //fix root's min/max depth
    if(p->min_depth > actual_depth){
        p->min_depth = actual_depth;
    }
    if(p->max_depth < actual_depth){
        p->max_depth = actual_depth;
    }

    while(!found){
        if((p->key > key) && ((p->left == nullptr) || (p->left->is_root == true))){
            found = true;
        }
        else if((p->key < key) && ((p->right == nullptr)  || (p->right->is_root == true))){
            found = true;
        }
        else if((p->key > key)){
            p = p->left;
        }
        else{
            p = p->right;
        }

        //go down updating min/max depths of visited nodes
        if(p->min_depth > actual_depth){
            p->min_depth = actual_depth;
        }
        if(p->max_depth < actual_depth){
            p->max_depth = actual_depth;
        }
    }

    Node *son;
    if(to_be_positioned == nullptr){
        son = new Node(p,key,depth);
    }
    else{
        son = to_be_positioned;
    }

    if(p->key > key){
        if(p->left != nullptr){
            Node* other_root = p->left;
            if(other_root->key < son->key){
                son->left = other_root;
                other_root->parent = son;
            }
            else{
                son->right = other_root;
                other_root->parent = son;
            }
        }
        p->left = son;
    }
    else{
        if(p->right != nullptr){
            Node* other_root = p->right;
            if(other_root->key < son->key){
                son->left = other_root;
                other_root->parent = son;
            }
            else{
                son->right = other_root;
                other_root->parent = son;
            }
        }
        p->right = son;
    }
    son->parent = p;

    if(p->is_black){
        son->black_height = p->black_height - 1; //new_node is red, father must be black
    }

    Fix(son);

    //update new_root
    Node* new_root = root;
    while(new_root->parent != nullptr){
        new_root = new_root->parent;
    }

    //put the upper_nodes back
    if(upper_nodes != nullptr){
        new_root->parent = upper_nodes;
    }
    return new_root;
}

Node* add(Node* root, int key, Node* to_be_positioned, int depth = 0){
    //Given a tree, and key or node, add that key/node to the tree and returns the new root.
    bool first_node = false;
    if(root == nullptr){
        first_node = true;
    }
    root = add_RB(root, key, to_be_positioned, depth);
    if(root != nullptr){
        if(!root->is_black){
            root->black_height++;
            root->is_black = true;
        }
    }
    if(first_node){
        root->is_root = true;
    }
    return root;
}

void debug_rec(Node *u, int i = 0){
    //Recursively print the tree in a tree-shaped format
    if(u == nullptr){
        return;
    }
    if(u->right != nullptr){
        debug_rec(u->right, i+3);
    }

    for(int j=0;j<i;j++){
        cout << " ";
    }
    if(u->is_root){
        cout << " ->";
    }
    else{
        cout << "   ";
    }
    cout << u->key << "(" << u->black_height << ")";
    cout << "  [" << u->min_depth << " " << u->max_depth << "]";
    if(u->is_black){
        cout << "   --- Black";
    }
    else{
        cout << "   --- Red";
    }
    cout << endl;
    if(u->left != nullptr){
        debug_rec(u->left, i+3);
    }
}

void debug_rec_trees(Node *u, int i, stack<Node*>& parts, int& sum){
    //Recursively print the tree in a tree-shaped format
    if(u == nullptr){
        return;
    }
    sum++;
    if(u->right != nullptr){
        if(u->right->is_root){
            parts.push(u->right);
        }
        else{   
            debug_rec_trees(u->right, i+3, parts, sum);
        }
    }

    for(int j=0;j<i;j++){
        cout << " ";
    }
    if(u->is_root){
        cout << " ->";
    }
    else{
        cout << "   ";
    }
    cout << u->key << "(" << u->black_height << "/" << u->depth << ")";
    cout << "  [" << u->min_depth << " " << u->max_depth << "]";
    if(u->is_black){
        cout << "   --- Black";
    }
    else{
        cout << "   --- Red";
    }
    cout << "         -> " << u->depth << " <-" << endl;
    if(u->left != nullptr){
        if(u->left->is_root){
            parts.push(u->left);
        }
        else{
            debug_rec_trees(u->left, i+3, parts, sum);
        }
    }
} 

int size_rec(Node* root){
    //Calculates the number of nodes of a tree
    if(root == nullptr){
        return 0;
    }
    int e = size_rec(root->left);
    int d = size_rec(root->right); 
    return e + d + 1;
}

int size(Node* root){
    //Gives a node, return the number of nodes in its subtree
    return size_rec(root);
}

void print(Node* root){
    cout << "Size of the tree = " << size(root) << endl;
    debug_rec(root);
    cout << "---------------" << endl << endl;
}

void print_trees(Node* root, bool all = true){
    //Given a root and a bool all, print the tree taking into account the different trees within it
    //If all is equal to true, print the whole tree, if all is equal to false, print just the first tree 
    if(root == nullptr){
        return;
    }
    int counter = 1;
    int sum = 0;
    stack<Node*> parts;
    parts.push(root);
    while(!parts.empty()){
        cout << "Tree number  =  " << counter << endl;
        Node* debug = parts.top();
        parts.pop();
        debug_rec_trees(debug, 0, parts, sum);
        cout << "-------------" << endl;
        counter += 1;
        if(!all){
            break;
        }
    }
    cout << "There are " << sum << " nodes in total" << endl;
}

void add_to_tango(Node* root, Node* new_piece){
    //Given two trees, add the second tree to a leaf postion of the first one
    if(root == nullptr || new_piece == nullptr){
        return;
    }

    Node* current_node = root;
    while(true){
        if(current_node->key > new_piece->key){
            if(current_node->left == nullptr){
                current_node->left = new_piece;
                break;
            }
            else{
                current_node = current_node->left;
            }
        }
        else{
            if(current_node->right == nullptr){
                current_node->right = new_piece;
                break;
            }
            else{
                current_node = current_node->right;
            }
        }
    }
}

Node* Join(Node *u, Node* k, Node *v){
    //Given two trees u and v, and a node k, such that all nodes in u have keys smaller than k's key and all
    //nodes in v have keys greater that k's key, then return a valid tree that has the nodes of all three trees
    fix_depths(k);
    if(u == nullptr && v == nullptr){
        k->is_black = true;
        k->black_height = 2;
        return k;
    }
    else{
        k->black_height = 1;
        k->is_black = false;
    }
    if(u == nullptr || u->is_root){
        if(u != nullptr){
            k->left = u;
        }
        Node* to_be_returned = add(v, k->key, k, k->depth);
        return to_be_returned;
    }
    if(v == nullptr || v->is_root){
        if(v != nullptr){
            k->right = v;
        }
        Node* to_be_returned = add(u, k->key, k, k->depth);
        return to_be_returned;
    }
    Node *novo = k;

    if(u->black_height == v->black_height){
        novo->is_black = true;
        novo->black_height = u->black_height + 1;
        u->parent = novo;
        v->parent = novo;
        novo->left = u;
        novo->right = v;
        novo->left->is_root = false;
        novo->right->is_root = false;
        fix_depths(novo);
        return novo;
    }

    Node *x;
    Node *final_root;

    int min_depth_being_added, max_depth_being_added; 

    if(u->black_height > v->black_height){
        min_depth_being_added = min(k->min_depth, k->min_depth, v->min_depth);
        max_depth_being_added = max(k->max_depth, k->max_depth, v->max_depth);
        final_root = u;
        Node* u_aux = u;
        while(u_aux->black_height > v->black_height){
            if(u_aux->min_depth > min_depth_being_added){
                u_aux->min_depth = min_depth_being_added;
            }
            if(u_aux->max_depth < max_depth_being_added){
                u_aux->max_depth = max_depth_being_added;
            }
            u_aux = u_aux->right;
        }

        x = u_aux->parent;
        novo->left = u_aux;
        novo->black_height = u_aux->black_height;
        x->right = novo;
        novo->right = v;

        v->parent = novo;
        u_aux->parent = novo;
        novo->parent = x;
    }
    else{ //u->black_height < v->black_height
        min_depth_being_added = min(k->min_depth, k->min_depth, u->min_depth);
        max_depth_being_added = max(k->max_depth, k->max_depth, u->max_depth);
        final_root = v;
        Node* v_aux = v;
        while(v_aux->black_height > u->black_height){
            if(v_aux->min_depth > min_depth_being_added){
                v_aux->min_depth = min_depth_being_added;
            }
            if(v_aux->max_depth < max_depth_being_added){
                v_aux->max_depth = max_depth_being_added;
            }
            v_aux = v_aux->left;
        }
        
        x = v_aux->parent;
        novo->right = v_aux;
        novo->black_height = v_aux->black_height;
        x->left = novo;
        novo->left = u;
        
        u->parent = novo;
        v_aux->parent = novo;
        novo->parent = x;
    }
    fix_depths(novo);
    if(novo->parent->is_black == false){
        Fix(novo);
    }
    while(final_root->parent != nullptr){
        final_root = final_root->parent;
    }
    //final_root->is_root = true;
    return final_root;
}

split_data Split(Node *root, float pivot){
    //Given a tree and a float pivot, return two trees: one that has only nodes that have keys smaller or
    //equal to pivot, and one with keys exclusively greater than pivot
    stack<Node*> left_trees;
    stack<Node*> right_trees;

    Node* search = root;
    search->is_root = false;

    bool is_there_remaning_node = false;
    Node* remaining_node = nullptr;

    while(search != nullptr){
        if(search->key > pivot){ //go left
            Node* right_node = search;
            search = search->left;
            right_trees.push(right_node);
            
            
            if(right_node->right != nullptr && right_node->right->is_black == false && (!right_node->right->is_root)){
                right_node->right->is_black = true;
                right_node->right->black_height += 1; 
            }
            if(right_node->left != nullptr){
                if(right_node->left->is_root){ //got to a node that is a new root
                    if(left_trees.size() != 0 && right_node->left->key < pivot){ //nodes in between need to be moved
                        remaining_node = right_node->left;
                        is_there_remaning_node = true;
                        right_node->left->parent = nullptr;
                        right_node->left = nullptr;
                    }
                    break;
                }
                right_node->left->parent = nullptr;
                right_node->left = nullptr;
            }
            else{
                break;
            }
        }
        else{ //go right
            Node* left_node = search;
            search = search->right;
            left_trees.push(left_node);

            if(left_node->left != nullptr && left_node->left->is_black == false){
                left_node->left->is_black = true;
                left_node->left->black_height += 1;
            }
            if(left_node->right != nullptr){
                if(left_node->right->is_root){
                    if(right_trees.size() != 0 && left_node->right->key > pivot){ //put the remaining nodes in the other side
                        remaining_node = left_node->right;
                        is_there_remaning_node = true;
                        left_node->right->parent = nullptr;
                        left_node->right = nullptr;
                    }
                    break;
                }
                left_node->right->parent = nullptr;
                left_node->right = nullptr;
            }
            else{
                break;
            }
        }
    }

    Node* final_left_tree = nullptr;
    if(left_trees.size() > 0){
        final_left_tree = left_trees.top();
        left_trees.pop();

        Node* final_insertion = final_left_tree; //get last
        if(is_there_remaning_node && remaining_node->key < pivot){
            final_insertion->right = remaining_node;
            remaining_node->parent = final_insertion;
            is_there_remaning_node = false;
        }
        fix_depths(final_insertion);

        if(final_left_tree->left != nullptr && (!final_left_tree->left->is_root)){
            final_left_tree = final_left_tree->left;
            if(final_left_tree->parent != nullptr){
                final_left_tree->parent->left = nullptr;
            }
            final_left_tree->parent = nullptr;
        }
        else{
            final_left_tree = nullptr;
        }        
        
        while(left_trees.size() > 0){
            Node* aux_tree = left_trees.top();
            left_trees.pop();

            Node* insertion_node = aux_tree;

            if(aux_tree->left != nullptr){
                aux_tree = aux_tree->left;
                if(aux_tree->parent != nullptr){
                    aux_tree->parent->left = nullptr;
                }
                aux_tree->parent = nullptr;
            }
            else{
                aux_tree = nullptr;
            }

            if(aux_tree != nullptr && aux_tree->is_root){
                insertion_node->left = aux_tree;
                aux_tree->parent = insertion_node;
                aux_tree = nullptr;
            }
            fix_depths(insertion_node);

            final_left_tree = Join(aux_tree, insertion_node, final_left_tree);
        }

        final_left_tree = Join(final_left_tree, final_insertion, nullptr);
    }

    Node* final_right_tree = nullptr;
    if(right_trees.size() > 0){
        final_right_tree = right_trees.top();
        right_trees.pop();

        Node* final_insertion = final_right_tree;
        if(is_there_remaning_node && remaining_node->key > pivot){
            final_insertion->left = remaining_node;
            remaining_node->parent = final_insertion;
            is_there_remaning_node = false;
        }
        fix_depths(final_insertion);

        if(final_right_tree->right != nullptr && (!final_right_tree->right->is_root)){
            final_right_tree = final_right_tree->right;
            if(final_right_tree->parent != nullptr){
                final_right_tree->parent->right = nullptr;
            }
            final_right_tree->parent = nullptr;
        }
        else{
            final_right_tree = nullptr;
        }
        
        while(right_trees.size() > 0){
            Node* aux_tree = right_trees.top();
            right_trees.pop();

            Node* insertion_node = aux_tree;

            if(aux_tree->right != nullptr){
                aux_tree = aux_tree->right;
                if(aux_tree->parent != nullptr){
                    aux_tree->parent->right = nullptr;
                }
                aux_tree->parent = nullptr;
            }
            else{
                aux_tree = nullptr;
            }

            if(aux_tree != nullptr && aux_tree->is_root){
                insertion_node->right = aux_tree;
                aux_tree->parent = insertion_node;
                aux_tree = nullptr;
            }
            fix_depths(insertion_node);

            final_right_tree = Join(final_right_tree, insertion_node, aux_tree);
        }
        final_right_tree = Join(nullptr, final_insertion, final_right_tree);
    }

    //cases when one of the trees is made of only one node and needs to be black
    if(final_left_tree != nullptr && !final_left_tree->is_black){
        final_left_tree->is_black = true;
        final_left_tree->black_height = 2;
    }
    if(final_right_tree != nullptr && !final_right_tree){
        final_left_tree->is_black = true;
        final_right_tree->black_height = 2;
    }
    return {final_left_tree, final_right_tree};
}

Node* build_complete_BST(int n, int offset = 0, int depth = 0){
    if(n <= 0){
        return nullptr;
    }
    else if(n == 1){
        return new Node(offset + 1, depth);
    }

    int x = 1;
    while(x*2 - 1 <= n){
        x = x * 2;
    }

    int l, r = 0;
    
    l = n - x + 1;
    
    if(l > x/2){
        r = l - x/2;
        l = x/2;
    }
    
    l += (x - 2)/2;
    r += (x - 2)/2;

    Node* new_node = new Node(l + offset + 1, depth);
    new_node->left = build_complete_BST(l, offset, depth + 1);
    new_node->right = build_complete_BST(r, offset + l + 1, depth + 1);
    return new_node;
}

Node* build_tango_tree(Node* u){
    if(u == nullptr){
        return nullptr;
    }
    stack<Node*> all_nodes;
    all_nodes.push(u);

    Node* tango = nullptr;

    while(!all_nodes.empty()){
        Node* node_in_auxiliary_tree = all_nodes.top();
        all_nodes.pop();
        Node* new_node = add(nullptr, node_in_auxiliary_tree->key, nullptr, node_in_auxiliary_tree->depth);
        //new Node(node_in_auxiliary_tree->key,node_in_auxiliary_tree->depth);
        int min_depth = new_node->depth;
        int max_depth = new_node->depth;

        while(node_in_auxiliary_tree->left != nullptr){
            if(node_in_auxiliary_tree->right != nullptr){
                all_nodes.push(node_in_auxiliary_tree->right);
            }
            new_node = add(new_node, node_in_auxiliary_tree->left->key,nullptr,node_in_auxiliary_tree->left->depth);
            node_in_auxiliary_tree = node_in_auxiliary_tree->left;
            if(node_in_auxiliary_tree->depth < min_depth){
                min_depth = node_in_auxiliary_tree->depth;
            }
            if(node_in_auxiliary_tree->depth > max_depth){
                max_depth = node_in_auxiliary_tree->depth;
            }
        }

        new_node->is_root = true;

        if(tango == nullptr){
            tango = new_node;
        }
        else{
            add_to_tango(tango, new_node);
        }
    }
    return tango;
}

Node* bring_to_front(Node* root, int item){
    if(root == nullptr){
        return nullptr;
    }
    root->is_root = false;
    //print(root);
    split_data first_split = Split(root, item - 0.5);
    //print_trees(first_split.left_tree);
    //print_trees(first_split.right_tree);
    split_data second_split = Split(first_split.right_tree, item + 0.5);
    //print_trees(second_split.left_tree);
    //print_trees(second_split.right_tree);

    Node* first_fragment = first_split.left_tree;
    Node* single_node = second_split.left_tree;
    Node* second_fragment = second_split.right_tree;

    if(first_fragment != nullptr){
        single_node->left = first_fragment;
        first_fragment->parent = single_node;
    }
    if(second_fragment != nullptr){
        single_node->right = second_fragment;
        second_fragment->parent = single_node;
    }

    single_node->is_black = true;
    
    if(first_fragment != nullptr){
        single_node->black_height = first_fragment->black_height + 1;
    }
    else if(second_fragment != nullptr){
        single_node->black_height = second_fragment->black_height + 1;
    }
    else{
        single_node->black_height = 2;
    }
    
    return single_node;
}

Node* concatenate(Node* root){
    //Given a node that has a valid tree in its left and right subtree and the nodes in
    //its left subtree have keys smaller than the root and all the nodes in the its right
    //subtree have keys greater than the root, then return a valid tree with all nodes 
    if(root == nullptr){
        return nullptr;
    }
    root->is_root = false;
    
    Node* first_fragment = nullptr;
    Node* second_fragment = nullptr;
    Node* single_node = root;

    if(single_node->left != nullptr && (!single_node->left->is_root)){
        first_fragment = single_node->left;
        single_node->left = nullptr;
        first_fragment->parent = nullptr;
    }
    if(single_node->right != nullptr && (!single_node->right->is_root)){
        second_fragment = single_node->right;
        single_node->right = nullptr;
        second_fragment->parent = nullptr;
    }
    Node* final = Join(first_fragment, single_node, second_fragment);
    return final;
}

Node* cut(Node* u, int min_depth){
    //Given the root of a tree and a int min_depth, put all the nodes in this tree that
    //have depth greater that min_depth in another tree
    Node* current_node = u;
    u->is_root = false;
    
    int l, r;
    l = r = -1;
    
    while(current_node != nullptr){ //find l
        if(current_node->is_root){
            break;
        }
        if(current_node->depth > min_depth && (current_node->key < l || l == -1)){
            l = current_node->key;
        }
        if(current_node->left != nullptr && current_node->left->max_depth > min_depth && (!current_node->left->is_root)){
            current_node = current_node->left;
        }
        else{
            current_node = current_node->right;
        }
    }

    current_node = u; //reset current_node
    
    while(current_node != nullptr){ //find r
        if(current_node->is_root){
            break;
        }
        if(current_node->depth > min_depth && current_node->key > r){
            r = current_node->key;
        }
        if(current_node->right != nullptr && current_node->right->max_depth > min_depth && (!current_node->right->is_root)){
            current_node = current_node->right;
        }
        else{
            current_node = current_node->left;
        }
    }

    int l_prime = predecessor(u, l); //calculate l'
    int r_prime = sucessor(u, r); //calculate r'


    Node* root = u;    
    if(l_prime != -1 && r_prime != -1){ //bigger case

        root = bring_to_front(root, l_prime);
        root->right = bring_to_front(root->right, r_prime);
        if(root->right != nullptr){
            root->right->parent = root;
        }

        
        root->right->left->is_root = true;
        

        root->right = concatenate(root->right);
        if(root->right != nullptr){
            root->right->parent = root;
        }
        root = concatenate(root);
    }
    else if(l_prime == -1 && r_prime != -1){
        root = bring_to_front(root, r_prime);
        

        root->left->is_root = true;
        

        root = concatenate(root);
    }
    else{
        root = bring_to_front(root, l_prime);
        root->right->is_root = true;

        root = concatenate(root);
    }
    return root;
}

Node* glue(Node* u, int searched_item){
    //Given a node u, root of a tree, and an int searched item, glues together the trees
    //u and the tree that has the node with key searched item
    int l_prime, r_prime;
    l_prime = r_prime = -1;

    Node* current_node = u;

    while(!current_node->is_root){
        if(current_node->key > searched_item){ //go left
            r_prime = current_node->key;
            current_node = current_node->left;
        }
        else{ //go right
            l_prime = current_node->key;
            current_node = current_node->right;
        }
    }

    Node* root = u;
    if(l_prime != -1 && r_prime != -1){ //bigger case

        root = bring_to_front(root, l_prime);
        root->right = bring_to_front(root->right, r_prime);
        if(root->right != nullptr){
            root->right->parent = root;
        }

        root->right->left->is_root = false;
        //print(root);

        root->right = concatenate(root->right);
        if(root->right != nullptr){
            root->right->parent = root;
        }
        root = concatenate(root);
    }
    else if(l_prime == -1 && r_prime != -1){ //l_prime == -1
        root = bring_to_front(root, r_prime);
        root->left->is_root = false;
        
        root = concatenate(root);
    }
    else{ //r_prime == -1
        root = bring_to_front(root, l_prime);
        
        root->right->is_root = false;
        root = concatenate(root);
    }
    root->is_root = true;
    return root;
}

Node* tango(Node* u, int min_depth, int searched_item){
    //
    Node* new_root = cut(u, min_depth);
    new_root = glue(new_root, searched_item);
    return new_root;
}

Node* search_in_tango(Node* root, int searched_item, bool& found){
    if(root == nullptr){
        return nullptr;
    }

    Node* last_root = root;
    Node* current_node = root;
    int contador = 0;

    while(current_node != nullptr && !found){
        if(current_node->is_root && current_node != last_root){
            last_root = tango(last_root, current_node->min_depth - 1, searched_item);
            current_node = last_root;
            contador++;
        }

        if(current_node->key == searched_item){
            found = true;
        }
        else if(current_node->key > searched_item){ //go left
            current_node = current_node->left;
        }
        else{ //go right
            current_node = current_node->right;
        }
    }
    return last_root;
}

#endif