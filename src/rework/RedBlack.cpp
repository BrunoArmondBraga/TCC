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

void fix(Node* child){
    //Given a node, fix the structure/colours of the tree to be valid considering only the nodes above child
    if(child == nullptr || child->is_black){
        return;
    }
    Node* p = child->parent; 

    while(true){
        if(p->is_black == true){ //if parent is black, it is fixed
            break;
        }
        Node *grandparent = p->parent;

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

Node* reset_min_node(Node* u){
    //detach u from the tree it was in
    if(u->parent != nullptr){
        if(u->right != nullptr){
            u->parent->left = u->right;
            u->right->parent = u->parent;
            u->right = nullptr;
        }
        else if(u->parent->left == u){
            u->parent->left = nullptr;
        }
        u->parent = nullptr;
    }
    fix_depths(u);
    return u;
}

Node* fix_double_black(Node* double_black){
    //case u and v black
    if(double_black->parent == nullptr){
        return double_black;
    }
    Node* new_root = double_black->parent;
    Node* sibling = double_black->parent->right;

    //case black sibling with red right child
    if(sibling->is_black && sibling->right != nullptr && !sibling->right->is_root && !sibling->right->is_black){
        Node* local_root = left_rotate(double_black->parent);

        if(local_root->left->is_black){
            local_root->black_height++;
            local_root->left->black_height--;
        }
        else{
            local_root->is_black = false;
            local_root->left->is_black = true;
        }

        local_root->right->is_black = true;
        local_root->right->black_height++;
    }
    //case black sibling with red left child
    else if(sibling->is_black && sibling->left != nullptr && !sibling->left->is_root && !sibling->left->is_black){
        Node* local_root = right_rotate(sibling);
        local_root = left_rotate(local_root->parent);

        if(local_root->left->is_black){
            local_root->black_height++;
        }
        else{
            local_root->right->is_black = false;
            local_root->right->black_height--;
        }
        local_root->is_black = true;
        local_root->black_height++;
        local_root->left->black_height--;
    }
    //case black sibling with no red child
    else if(sibling->is_black){
        sibling->is_black = false;
        sibling->black_height--;
        if(sibling->parent->is_black){
            sibling->parent->black_height--;
            return fix_double_black(sibling->parent);
        }
        else{
            sibling->parent->is_black = true;
        }
    }
    //case red sibling
    else{
        Node* local_root = left_rotate(sibling->parent);

        local_root->is_black = true;
        local_root->black_height++;
        local_root->left->is_black = false;
        local_root->left->black_height--;
        return fix_double_black(double_black);
    }

    while(new_root->parent != nullptr){
        new_root = new_root->parent;
    }
    return new_root;
}

Node* get_min_node(Node* root){
    //Given a root, return the Node with minimum key and fix all min/max depth of the nodes in the path
    if(root->left != nullptr && !root->left->is_root){
        Node* to_be_returned = get_min_node(root->left);
        fix_depths(root);
        return to_be_returned;
    }
    //root is the node with the minimum key value
    if(root->parent != nullptr){ //put artificial depth to fix the depth of the nodes above 
        root->min_depth = root->parent->depth;
        root->max_depth = root->parent->depth;
    }
    return root;
}

pair_of_trees remove_min(Node* root){
    //Given the root of a valid redblack-tree, remove the node with minimum key of this tree
    //and return the pair {remaining tree, min_node} 

    //case root single node
    if((root->left == nullptr || root->left->is_root) && (root->right == nullptr || root->right->is_root)){
        return {nullptr, root};
    }

    Node* min_node = get_min_node(root);

    //case u or v red
    if(!min_node->is_black || (min_node->right != nullptr && !min_node->right->is_root)){
        if(!min_node->is_black){ //min_node is a red leaf
            if(min_node->right != nullptr){ //put right new_tree in the old place of min_node
                min_node->parent->left = min_node->right;
                min_node->right->parent = min_node->parent;
            }
            min_node->right = nullptr;
            return {root, reset_min_node(min_node)};
        }
        else{ //min_node is parent of a red right-child that is a leaf
            Node* aux = min_node->right;
            aux->parent = min_node->parent;
            aux->black_height = 2;
            aux->is_black = true;
            min_node->right = nullptr;
            if(aux->parent != nullptr){
                aux->parent->left = aux;
                fix_depths(aux->parent);
                return {root, reset_min_node(min_node)};
            }
            return {aux, reset_min_node(min_node)};
        }
    }
    //case u and v black
    return {fix_double_black(min_node), reset_min_node(min_node)};
}

void debug_rec(Node *u, int i = 0){
    //Recursively print the whole tree in a tree-shaped format
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
    //Recursively print each tree in a tree-shaped format
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

void fix_join_conditions(Node *u, Node* k, Node *v){
    //Given three nodes, check if k is child of u/v, make u and v black nodes, k red
    //fix depths of all pointers.
    if(u != nullptr){
        if(u->parent == k){
            k->left = nullptr;
            u->parent = nullptr;
        }
        if(!u->is_black){
            u->is_black = true;
            u->black_height++;
        }
    }
    if(v != nullptr){
        if(v->parent == k){
            k->right = nullptr;
            v->parent = nullptr;
        }
        if(!v->is_black){
            v->is_black = true;
            v->black_height++;
        }
    }
    fix_depths(k);
    fix_depths(u);
    fix_depths(v);
    k->black_height = 1;
    k->is_black = false;
    k->parent = nullptr;
}

pair_of_trees aux_join(Node *u, Node* k, Node *v){
    //Given two trees u and v, and a node k, where all nodes in u have keys smaller than k's key and all nodes
    //in v have keys greater that k's key, return a pair of trees {new root, k}
    int left_height, right_height;
    left_height = right_height = 1;

    if(u != nullptr && !u->is_root){
        left_height = u->black_height;
    }
    if(v != nullptr && !v->is_root){
        right_height = v->black_height;
    }

    if(left_height == right_height){
        if(u != nullptr || v != nullptr){
            k->black_height = left_height;
            if(u != nullptr){
                k->left = u;
                u->parent = k;
            }
            if(v != nullptr){
                k->right = v;
                v->parent = k;
            }
            //update k min/max depths
            fix_depths(k);
        }
        return {k, k};
    }

    int min = k->min_depth;
    int max = k->max_depth;
    if(u != nullptr){
        min = std::min({min, u->min_depth});
        max = std::max({max, u->max_depth});
    }
    if(v != nullptr){
        min = std::min({min, v->min_depth});
        max = std::max({max, v->max_depth});
    }

    if(left_height > right_height){
        u->min_depth = min;
        u->max_depth = max;
        pair_of_trees next_step = aux_join(u->right, k, v);
        u->right = next_step.left_tree;
        if(u->right != nullptr){
            u->right->parent = u;
        }
        return {u, next_step.right_tree};
    }
    v->min_depth = min;
    v->max_depth = max;
    pair_of_trees next_step = aux_join(u, k, v->left);
    v->left = next_step.left_tree;
    if(v->left != nullptr){
        v->left->parent = v;
    }
    return {v, next_step.right_tree};
}

Node* join(Node *u, Node* k, Node *v){
    //Given two trees u and v, and a node k, where all nodes in u have keys smaller than k's key and all nodes
    //in v have keys greater that k's key, then return a valid tree that has the nodes of all three trees

    //fix conditions and join trees
    fix_join_conditions(u,k,v);
    pair_of_trees join = aux_join(u, k, v);

    //get new root
    Node* root = join.left_tree;

    //fix possible structural/colour problem
    fix(join.right_tree->right);
    fix(join.right_tree->left);

    //check if root->parent is nullptr
    if(root->parent != nullptr){
        root = root->parent;
    }
    if(!root->is_black){
        if(root->left != nullptr && !root->left->is_root){
            root->black_height = root->left->black_height + 1;
        }
        else{
            root->black_height = 2;
        }
        root->is_black = true;
    }
    return root;
}

pair_of_trees split(Node* root, float pivot){
    //Given the root of a tree and a float pivot, return a pair of tree that has its left tree
    //as a valid red-black tree with only nodes with key less than pivot and has its right tree
    //as a valid red-black tree with only nodes with key greater than pivot
    if(root == nullptr){
        return {nullptr, nullptr};
    }
    root->parent = nullptr;
    if(root->is_root){
        if(pivot < root->key){
            return {nullptr, root};
        }
        return {root, nullptr};
    }
    if(pivot < root->key){ //go left
        pair_of_trees aux = split(root->left, pivot);
        root->left = nullptr;
        if(aux.right_tree != nullptr && aux.right_tree->is_root){
            root->left = aux.right_tree;
            aux.right_tree->parent = root;
            //root is the first node of the actual tree
            root->is_black = true;
            root->black_height = 2;
            if(root->right == nullptr || root->right->is_root){
                fix_depths(root);
                return{aux.left_tree, root};
            }
            return {aux.left_tree, join(nullptr, root, root->right)};
        }
        if(root->right != nullptr && root->right->is_root){
            return {aux.left_tree, join(aux.right_tree, root, nullptr)};
        }
        return {aux.left_tree, join(aux.right_tree, root, root->right)};
    }
    //go right
    pair_of_trees aux = split(root->right, pivot);
    root->right = nullptr;
    if(aux.left_tree != nullptr && aux.left_tree->is_root){
        root->right = aux.left_tree;
        aux.left_tree->parent = root;
        //root is the first node of the actual tree
        root->is_black = true;
        root->black_height = 2;
        if(root->left == nullptr || root->left->is_root){
            fix_depths(root);
            return{root, aux.right_tree};
        }
        return {join(root->left, root, nullptr), aux.right_tree};
    }
    if(root->left != nullptr && root->left->is_root){
        return {join(nullptr, root, aux.left_tree), aux.right_tree};
    }
    return {join(root->left, root, aux.left_tree), aux.right_tree};
}