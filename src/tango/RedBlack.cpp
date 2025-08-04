#include "RedBlack.h"

//This implementation of a red-black tree is heavily based in the Chapter 13
//of the book Introduction to Algorithms of Cormen, Leiserson, Rivest and Stein

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

    u->min_depth = std::min({left_min_depth, u->depth, right_min_depth});
    u->max_depth = std::max({left_max_depth, u->depth, right_max_depth});
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
    //update depths
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
    //update depths
    fix_depths(u);
    fix_depths(aux);
    return aux;
}

int predecessor(Node* root, int i){
    //Given the root of a tree and an integer i, return the biggest key in the tree that is smaller than i.
    int pred = -1;
    Node* current_node = root;

    while(current_node != nullptr && !current_node->is_root){
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
    //Given the root of a tree and an integer i, return the smallest key in the tree that is bigger than i
    int suc = -1;
    Node* current_node = root;

    while(current_node != nullptr && !current_node->is_root){
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

void fix(Node* child){
    //Given a red node, fix the structure/colors of the tree to be valid
    Node* p = child->parent; 

    while(true){
        if(p->is_black == true){ //if parent is black, it is fixed
            break;
        }

        Node *grandparent = p->parent;
        /* if(grandparent==nullptr){
            p->is_black = true;
            p->black_height = child->black_height + 1;
            break;
        } */

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
            child = grandparent;
            
            uncle->black_height = grandparent->black_height;
            p->black_height = grandparent->black_height;
            
            p = grandparent->parent;
            if(p == nullptr){
                break;
            }
        }
        else{ //case uncle is black
            if(p==grandparent->left && child == p->left){ //same side
                Node *q = right_rotate(grandparent);
                q->is_black = true;
                grandparent->is_black = false;

                int aux = q->black_height;
                q->black_height = grandparent->black_height;
                grandparent->black_height = aux;
                break;
            }
            else if(p==grandparent->left && child == p->right){ //different sides
                Node *q = left_rotate(p);
                Node *r = right_rotate(grandparent);
                r->is_black = true;
                grandparent->is_black = false;

                int aux = grandparent->black_height;
                grandparent->black_height = r->black_height;
                r->black_height = aux;
                break;
            }
            else if(p==grandparent->right && child ==p->right){ //same side
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

Node *add_RB(Node *root, Node* unplaced){
    //Insert a node to a tree and return the new root of the tree.
    if(root == nullptr){
        Node* insert = unplaced;
        insert->is_black = true;
        insert->black_height = 2;
        insert->is_root = true;
        return insert;
    }

    //sometimes add_RB is used by a join, and needs to keep the upper_nodes other of the equation.
    Node* upper_nodes = root->parent;
    if(root->parent != nullptr){
        root->parent = nullptr;
    }

    Node* p = root;
    bool found = false;
    
    int depth = unplaced->depth;

    //fix root's min/max depth
    if(p->min_depth > depth){
        p->min_depth = depth;
    }
    if(p->max_depth < depth){
        p->max_depth = depth;
    }

    while(!found){
        if((p->key > unplaced->key) && ((p->left == nullptr) || (p->left->is_root == true))){
            found = true;
        }
        else if((p->key < unplaced->key) && ((p->right == nullptr)  || (p->right->is_root == true))){
            found = true;
        }
        else if((p->key > unplaced->key)){
            p = p->left;
        }
        else{
            p = p->right;
        }

        //go down updating min/max depths of visited nodes
        if(p->min_depth > depth){
            p->min_depth = depth;
        }
        if(p->max_depth < depth){
            p->max_depth = depth;
        }
    }

    if(p->key > unplaced->key){
        if(p->left != nullptr){
            Node* other_root = p->left;
            if(other_root->key < unplaced->key){
                unplaced->left = other_root;
                other_root->parent = unplaced;
            }
            else{
                unplaced->right = other_root;
                other_root->parent = unplaced;
            }
        }
        p->left = unplaced;
    }
    else{
        if(p->right != nullptr){
            Node* other_root = p->right;
            if(other_root->key < unplaced->key){
                unplaced->left = other_root;
                other_root->parent = unplaced;
            }
            else{
                unplaced->right = other_root;
                other_root->parent = unplaced;
            }
        }
        p->right = unplaced;
    }
    unplaced->parent = p;

    if(p->is_black){
        unplaced->black_height = p->black_height - 1; //new_node is red, father must be black
    }

    fix(unplaced);

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

Node* add(Node* root, Node* unplaced){
    //Given a tree and a node, add this node to the tree and return the new root of the tree.
    bool first_node = false;
    if(root == nullptr){
        first_node = true;
    }
    root = add_RB(root, unplaced);
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
    cout << u->key << "(" << u->black_height << ")";
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
    //Given a root, print the tree
    cout << "Size of the tree = " << size(root) << endl;
    debug_rec(root);
    cout << "---------------" << endl << endl;
}

void print_trees(Node* root, bool all){
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

Node* join(Node *u, Node* k, Node *v){
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
        Node* to_be_returned = add(v, k);
        return to_be_returned;
    }
    if(v == nullptr || v->is_root){
        if(v != nullptr){
            k->right = v;
        }
        Node* to_be_returned = add(u, k);
        return to_be_returned;
    }
    Node *new_node = k;

    if(u->black_height == v->black_height){
        new_node->is_black = true;
        new_node->black_height = u->black_height + 1;
        u->parent = new_node;
        v->parent = new_node;
        new_node->left = u;
        new_node->right = v;
        new_node->left->is_root = false;
        new_node->right->is_root = false;
        fix_depths(new_node);
        return new_node;
    }

    Node *x;
    Node *final_root;

    int min_depth_being_added, max_depth_being_added; 

    if(u->black_height > v->black_height){
        min_depth_being_added = std::min(k->min_depth, v->min_depth);
        max_depth_being_added = std::max(k->max_depth, v->max_depth);
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
        new_node->left = u_aux;
        new_node->black_height = u_aux->black_height;
        x->right = new_node;
        new_node->right = v;

        v->parent = new_node;
        u_aux->parent = new_node;
        new_node->parent = x;
    }
    else{ //u->black_height < v->black_height
        min_depth_being_added = std::min({k->min_depth, k->min_depth, u->min_depth});
        max_depth_being_added = std::max({k->max_depth, k->max_depth, u->max_depth});
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
        new_node->right = v_aux;
        new_node->black_height = v_aux->black_height;
        x->left = new_node;
        new_node->left = u;
        
        u->parent = new_node;
        v_aux->parent = new_node;
        new_node->parent = x;
    }
    fix_depths(new_node);
    while(final_root->parent != nullptr){
        final_root = final_root->parent;
    }
    return final_root;
}

split_data split(Node *root, float pivot){
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

            final_left_tree = join(aux_tree, insertion_node, final_left_tree);
        }

        final_left_tree = join(final_left_tree, final_insertion, nullptr);
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

            final_right_tree = join(final_right_tree, insertion_node, aux_tree);
        }
        final_right_tree = join(nullptr, final_insertion, final_right_tree);
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

Node* bring_to_root(Node* root, int item){
    //Given a node r and a key x, such that there is a node v with key x in r's subtree, than 
    //re-arrange r's subtree so the node v is at the root and the left subtree of v is a valid red-black
    //tree with only nodes with keys smaller than x and the right subtree of v is a valid red-black tree
    //with only nodes with keys greater than x. Return node v
    if(root == nullptr){
        return nullptr;
    }
    root->is_root = false;
    split_data first_split = split(root, item - 0.5);
    split_data second_split = split(first_split.right_tree, item + 0.5);

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
    Node* final = join(first_fragment, single_node, second_fragment);
    return final;
}