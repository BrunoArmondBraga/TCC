#include "Auxiliary.h"

Node* build_complete_BST(int n, int offset = 0, int depth = 0){
    //Given a int n, recursively build a complete binary search tree in level-order
    //built from left to right. 
    if(n <= 0){
        return nullptr;
    }
    else if(n == 1){
        return new Node(offset + 1, depth);
    }

    int h = log2(n);
    int x = pow(2,h);

    int l, r = 0;
    
    l = n - x + 1;
    
    if(l > x/2){
        r = l - x/2;
        l = x/2;
    }
    
    l += (x - 2)/2;
    r += (x - 2)/2;

    Node* new_node = new Node(l + offset + 1, depth);
    new_node->is_black = true;
    new_node->black_height = 2;
    new_node->left = build_complete_BST(l, offset, depth + 1);
    new_node->right = build_complete_BST(r, offset + l + 1, depth + 1);
    return new_node;
}

Node* build_tango(Node* u); //forward declaration for mutual recursion

Node* build_BST_from_preferred_path(vector<Node*> preferred_path, stack<Node*> &child_trees, int l, int r, int h) {
    //Given a vector preferred_path, a stack child_tree, and tree integers l, r and h, build a balanced
    //binary search tree with all nodes from preferred_path and recursively call build_tango in leaf 
    //pointers of this tree
    if (l > r) {
        Node* u = child_trees.top();
        child_trees.pop();
        u = build_tango(u);
        return u;
    }
    int m = (l+r)/2;
    Node* u = preferred_path[m];
    u->left  = build_BST_from_preferred_path(preferred_path, child_trees, m+1,r,h-1);
    if(u->left != nullptr){
        u->left->parent = u;
    }
    u->right = build_BST_from_preferred_path(preferred_path, child_trees, l, m-1,h-1);
    if(u->right != nullptr){
        u->right->parent = u;
    }
    if (h == 0){
        u->is_black = false;
    }
    u->black_height = h+1;
    fix_depths(u);
    return u;
}

Node* build_tango(Node* u){
    //Given a node u of the auxiliary tree, puts all the left preferred child in a vector,
    //stack all the remaining nodes that were splitted from the tree, and recursively call
    //build_BST_from_preferred_path
    if(u == nullptr){
        return nullptr;
    }
    stack<Node*> child_trees;
    vector<Node*> preferred_path;
    Node* t;
    while(u != nullptr){
        child_trees.push(u->right);
        preferred_path.push_back(u);
        u = u->left;
    }
    child_trees.push(nullptr);

    int n = preferred_path.size();
    int h = log2(n);
    t = build_BST_from_preferred_path(preferred_path, child_trees, 0, n-1, h);

    if(!t->is_black){ //case when there is only one node
        t->black_height = 2;
    }
    t->is_black = t->is_root = true;
    return t;
}

Node* build_initial_tango(int n){
    //Given a int n, return a valid tango tree with all left pointers as preferred child
    Node* root = build_complete_BST(n);
    root = build_tango(root);
    return root;
}

Node* cut(Node* u, int min_depth){
    //Given the root of a valid red-black tree and a int min_depth, put all the nodes of this tree
    //that have depth greater than min_depth in another valid red-black tree and return the root
    //of the valid red-black tree with only nodes with depth less than or equal to min_depth
    Node* current_node = u;
    u->is_root = false;
    
    int l, r;
    l = r = -1;
    
    while(current_node != nullptr){ //find l = 
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

    //l now is the minimum key value that has depth greater than min_depth
    //r now is the maximum key value that has depth greater than min_depth

    int l_prime = predecessor(u, l); //calculate l'
    int r_prime = sucessor(u, r); //calculate r'


    Node* root = u;    
    if(l_prime != -1 && r_prime != -1){ //bigger case

        root = bring_to_root(root, l_prime);
        root->right = bring_to_root(root->right, r_prime);
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
        root = bring_to_root(root, r_prime);
        

        root->left->is_root = true;
        

        root = concatenate(root);
    }
    else{
        root = bring_to_root(root, l_prime);
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

        root = bring_to_root(root, l_prime);
        root->right = bring_to_root(root->right, r_prime);
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
        root = bring_to_root(root, r_prime);
        root->left->is_root = false;
        
        root = concatenate(root);
    }
    else{ //r_prime == -1
        root = bring_to_root(root, l_prime);
        
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