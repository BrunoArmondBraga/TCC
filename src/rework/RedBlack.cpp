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
    //Given the root of a tree and an integer i, return the biggest key in the tree that is smaller than i
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
    //Given a node, fix the structure/colors of the tree to be valid considering only the nodes above child
    if(child == nullptr || child->is_black){
        return;
    }
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

Node* reset_min_node(Node* u){
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
    //case u and v black (2.)
    //case sibling has red right child
    if(double_black->parent == nullptr){
        return double_black;
    }
    Node* new_root = double_black->parent;
    Node* sibling = double_black->parent->right;
    if(sibling->is_black && sibling->right != nullptr && !sibling->right->is_root && !sibling->right->is_black){
        Node* local_root = left_rotate(double_black->parent);

        if(local_root->left->is_black){
            local_root->is_black = true;
            local_root->black_height++;
            local_root->left->black_height--;
        }
        else{
            local_root->is_black = false;
        }

        local_root->right->is_black = true;
        local_root->right->black_height++;
    }
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
    else{
        Node* local_root = left_rotate(sibling->parent);

        local_root->is_black = true;
        local_root->black_height++;
        local_root->left->is_black = false;
        local_root->left->black_height--;
        /* if(local_root->left->right != nullptr && !local_root->left->right->is_root){
            local_root->left->right->is_black = false;
            local_root->left->right->black_height--;
        } */
        return fix_double_black(double_black);
    }

    while(new_root->parent != nullptr){
        new_root = new_root->parent;
    }
    return new_root;
}

Node* get_min_node(Node* root){
    if(root->left != nullptr && !root->left->is_root){
        Node* to_be_returned = get_min_node(root->left);
        fix_depths(root);
        return to_be_returned;
    }
    else{
        //root is the node with the minimum key value
        if(root->parent != nullptr){
            root->min_depth = root->parent->depth;
            root->max_depth = root->parent->depth;
        }
        return root;
    }
}

split_data remove_min(Node* root){
    //case root single node
    if((root->left == nullptr || root->left->is_root) && (root->right == nullptr || root->right->is_root)){
        return {nullptr, root};
    }

    Node* min_node = get_min_node(root);

    //case u or v red (Case 1.)
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
    return {fix_double_black(min_node), reset_min_node(min_node)};
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

void fix_join_conditions(Node *u, Node* k, Node *v){
    if(u != nullptr){
        if(u->parent == k){
            k->left = nullptr;
            u->parent = nullptr;
        }
        if(!u->is_black){ //this if statement should be inside this upper if?
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
    k->parent = nullptr; //maybe problematic?
}

split_data aux_join(Node *u, Node* k, Node *v){
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
                /* if(u->parent != nullptr){
                    root = u->parent;
                } */
                k->left = u;
                u->parent = k;
            }
            if(v != nullptr){
                /* if(v->parent != nullptr){
                    root = v->parent;
                } */
                k->right = v;
                v->parent = k;
            }

            //update k min/max depths
            fix_depths(k);
        }

        /* //check for structural problems
        fix(k->right);
        fix(k->left); */
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
        split_data next_step = aux_join(u->right, k, v);
        u->right = next_step.left_tree;
        if(u->right != nullptr){
            u->right->parent = u;
        }
        return {u, next_step.right_tree};
    }
    v->min_depth = min;
    v->max_depth = max;
    split_data next_step = aux_join(u, k, v->left);
    v->left = next_step.left_tree;
    if(v->left != nullptr){
        v->left->parent = v;
    }
    return {v, next_step.right_tree};
}

Node* join(Node *u, Node* k, Node *v){
    //Given two trees u and v, and a node k, such that all nodes in u have keys smaller than k's key and all
    //nodes in v have keys greater that k's key, then return a valid tree that has the nodes of all three trees
    fix_join_conditions(u,k,v);
    split_data join_operation = aux_join(u, k, v);
    Node* root = join_operation.left_tree;
    fix(join_operation.right_tree->right);
    fix(join_operation.right_tree->left);
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

split_data split(Node* root, float pivot){
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
        split_data aux = split(root->left, pivot);
        root->left = nullptr;
        if(aux.right_tree != nullptr && aux.right_tree->is_root){
            root->left = aux.right_tree;
            aux.right_tree->parent = root;
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
    split_data aux = split(root->right, pivot);
    root->right = nullptr;
    if(aux.left_tree != nullptr && aux.left_tree->is_root){
        root->right = aux.left_tree;
        aux.left_tree->parent = root;
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