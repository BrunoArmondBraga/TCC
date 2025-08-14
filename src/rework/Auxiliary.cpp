#include "Auxiliary.h"

#define DEBUG_addtotango 0
#define DEBUG_COMPLEXITY 1

void add_to_tango(Node* root, Node* new_piece, int& compĺexity_test){
    //Given two trees root and new_piece, put the second tree in a leaf pointer of the first tree
    if(root->key > new_piece->key){
        if(root->left == nullptr || root->left->is_root){
            if(root->left != nullptr){
                if(DEBUG_addtotango){
                    compĺexity_test++;
                    cout << "Another iteration!" << endl;
                }
                Node* aux = root->left;
                aux->parent = new_piece;
                add_to_tango(new_piece, aux, compĺexity_test);
            }
            root->left = new_piece;
            new_piece->parent = root;
        }
        else{
            add_to_tango(root->left, new_piece, compĺexity_test);
        }
    }
    else{
        if(root->right == nullptr || root->right->is_root){
            if(root->right != nullptr){
                if(DEBUG_addtotango){
                    cout << "Another iteration!" << endl;
                }
                Node* aux = root->right;
                aux->parent = new_piece;
                add_to_tango(new_piece, aux, compĺexity_test);
            }
            root->right = new_piece;
            new_piece->parent = root;
        }
        else{
            add_to_tango(root->right, new_piece, compĺexity_test);
        }
    }
}

pair_of_trees split_operation(Node* root, float pivot){
    //Given the root of a tree and a float pivot, return a pair of tree that has its left tree
    //as a valid red-black tree with only nodes with key less than pivot and has its right tree
    //as a valid red-black tree with only nodes with key greater than pivot.

    //red-black tree split is not sufficient to the tango context
    pair_of_trees aux = split(root, pivot);
    if(aux.left_tree != nullptr && aux.left_tree->is_root){        
        int complexity_test = 1;
        add_to_tango(aux.right_tree, aux.left_tree, complexity_test);
        if(DEBUG_COMPLEXITY && complexity_test > 2){
            cout << "complexity = " << complexity_test << endl;
        }
        return {nullptr, aux.right_tree};
    }
    if(aux.right_tree != nullptr && aux.right_tree->is_root){
        int complexity_test = 1;
        add_to_tango(aux.left_tree, aux.right_tree, complexity_test);
        if(DEBUG_COMPLEXITY && complexity_test > 2){
            cout << "complexity = " << complexity_test << endl;
        }
        return {aux.left_tree, nullptr};
    }
    return aux;
}

Node* join_operation(Node *u, Node* k, Node *v){
    //Given two trees u and v, and a node k that can be nullptr, where all nodes in u have keys smaller than all nodes
    //in v and if k is not nullptr has key in between the maximum key of u and the minimum key of v. Return a single
    //valid tree with all nodes given.
    if(k == nullptr){
        if(v == nullptr){ //case where u is the only node
            return u;
        }
        //otherwise remove from v and join_operation(u,k,v)
        pair_of_trees remover = remove_min(v);
        return join_operation(u, remover.right_tree, remover.left_tree);
    }
    //hold k's left tree
    Node* hold_left = nullptr;
    if(k->left != nullptr && k->left->is_root){
        hold_left = k->left;
        hold_left->parent->left = nullptr;
        hold_left->parent = nullptr;
    }
    //hold k's right tree
    Node* hold_right = nullptr;
    if(k->right != nullptr && k->right->is_root){
        hold_right = k->right;
        hold_right->parent->right = nullptr;
        hold_right->parent = nullptr;
    }
    Node* final_tree = join(u, k, v);
    //put k's left tree back
    if(hold_left != nullptr){
        if(DEBUG_addtotango){
            cout << "ADD TO TANGO from join!!" << endl;
        }
        int complexity_test = 1;
        add_to_tango(final_tree, hold_left, complexity_test);
        if(DEBUG_COMPLEXITY && complexity_test > 2){
            cout << "complexity = " << complexity_test << endl;
        }
    }
    //put k's left tree back
    if(hold_right != nullptr){
        if(DEBUG_addtotango){
            cout << "ADD TO TANGO from join!!" << endl;
        }
        int complexity_test = 1;
        add_to_tango(final_tree, hold_right, complexity_test);
        if(DEBUG_COMPLEXITY && complexity_test > 2){
            cout << "complexity = " << complexity_test << endl;
        }
    }
    return final_tree;
}
    
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
    
    if(l > x/2){ //if l > x/2, there are some nodes in the last layer in the right part
        r = l - x/2; //calculate how many nodes in the right part
        l = x/2;
    }
    
    //add the remaining the nodes
    l += x/2 - 1;
    r += x/2 - 1;

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
        u->left->parent = u; //update parent node
    }

    u->right = build_BST_from_preferred_path(preferred_path, child_trees, l, m-1,h-1);
    if(u->right != nullptr){
        u->right->parent = u; //update parent node
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

    //put the whole preferred path in the vector
    while(u != nullptr){
        child_trees.push(u->right);
        preferred_path.push_back(u);
        u = u->left;
    }
    child_trees.push(nullptr);

    int n = preferred_path.size();
    int h = log2(n);
    t = build_BST_from_preferred_path(preferred_path, child_trees, 0, n-1, h);

    if(!t->is_black){ //case single node
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
    
    //find l := minimum key with depth greater than min_depth
    while(current_node != nullptr){
        if(current_node->is_root){
            break;
        }
        if(current_node->depth > min_depth && (current_node->key < l || l == -1)){
            l = current_node->key;
        }
        if(current_node->left != nullptr && !current_node->left->is_root && current_node->left->max_depth > min_depth){
            current_node = current_node->left;
        }
        else{
            current_node = current_node->right;
        }
    }

    current_node = u; //reset current_node
    
    //find r := maximum key with depth greater than min_depth
    while(current_node != nullptr){
        if(current_node->is_root){
            break;
        }
        if(current_node->depth > min_depth && current_node->key > r){
            r = current_node->key;
        }
        if(current_node->right != nullptr && !current_node->right->is_root && current_node->right->max_depth > min_depth){
            current_node = current_node->right;
        }
        else{
            current_node = current_node->left;
        }
    }

    pair_of_trees first_split = split_operation(u, l - 0.5);
    pair_of_trees second_split = split_operation(first_split.right_tree, r + 0.5);

    //tree cut out is going to be considered as a new tree new
    Node* tree_cut_out = second_split.left_tree;
    tree_cut_out->is_root = true;

    //put the tree cut out back into the tree
    if(second_split.right_tree != nullptr){
        if(DEBUG_addtotango){
            cout << "ADD TO TANGO from cut!!" << endl;
        }
        int complexity_test = 1;
        add_to_tango(second_split.right_tree, tree_cut_out, complexity_test);
        if(DEBUG_COMPLEXITY && complexity_test > 2){
            cout << "complexity = " << complexity_test << endl;
        }
    }
    else{
        if(DEBUG_addtotango){
            cout << "ADD TO TANGO from cut!!" << endl;
        }
        int complexity_test = 1;
        add_to_tango(first_split.left_tree, tree_cut_out, complexity_test);
        if(DEBUG_COMPLEXITY && complexity_test > 2){
            cout << "complexity = " << complexity_test << endl;
        }
    }

    //join the initial tree
    Node* final_tree = join_operation(first_split.left_tree, nullptr, second_split.right_tree);
    return final_tree;
}

Node* glue(Node* u, int searched){
    //Given a node u, root of a tree, and an int searched item, glues together the trees
    //u and the tree that has the node with key searched item
    int l, r;
    l = r = -1;

    Node* current_node = u;

    //l = maximum key that is smaller than searched in the path
    //r = minimum key that is bigger than searched in the path
    while(!current_node->is_root){
        if(current_node->key > searched){ //go left
            r = current_node->key;
            current_node = current_node->left;
        }
        else{ //go right
            l = current_node->key;
            current_node = current_node->right;
        }
    }

    Node* final_tree;

    if(l != -1 && r != -1){
        //int searched is between two keys in u 
        pair_of_trees first_split = split_operation(u, l + 0.5);
        pair_of_trees second_split = split_operation(first_split.right_tree, r + 0.5);

        Node* middle_node = second_split.left_tree;

        //tree included is going to be considered as part of the initial tree
        Node* tree_included = middle_node->left;
        tree_included->is_root = false;

        final_tree = join_operation(tree_included, middle_node, second_split.right_tree);
        final_tree = join_operation(first_split.left_tree, nullptr, final_tree);
    }
    else if (l != -1){ //r == -1
        //int searched is greater than all keys in u
        
        //split the tree so the node with key l is alone at the right tree
        pair_of_trees first_split = split_operation(u, l - 0.5);

        Node* middle_node = first_split.right_tree;

        //tree included is going to be considered as part of the initial tree
        Node* tree_included = middle_node->right;
        tree_included->is_root = false;

        final_tree = join_operation(first_split.left_tree, middle_node, tree_included);
    }
    else{ //l == -1
        //int searched is smaller than all keys in u

        //split the tree so the node with key r is alone at the left tree
        pair_of_trees first_split = split_operation(u, r + 0.5);

        Node* middle_node = first_split.left_tree;

        //tree included is going to be considered as part of the initial tree
        Node* tree_included = middle_node->left;
        tree_included->is_root = false;
        final_tree = join_operation(tree_included, middle_node, first_split.right_tree);
    }

    return final_tree;
}

Node* tango(Node* u, int min_depth, int searched){
    //Given the root of the tango and two ints min_depth and searched, change the structure of
    //two red-black trees with cut and glue to match the preferred paths in the auxiliary tree.
    Node* new_root = cut(u, min_depth);
    new_root = glue(new_root, searched);
    return new_root;
}

Node* search_in_tango(Node* root, int searched, bool& found){
    //Given a root u and a int searched, search in tango for that key, changing the red-black
    //trees accordingly and return the root of the tango at the end of the execution
    if(root == nullptr){
        return nullptr;
    }

    Node* last_root = root;
    Node* current_node = root;

    while(current_node != nullptr && !found){
        if(current_node->is_root && current_node != last_root){ //found a new tree (crossed a non-preferred pointer)
            last_root = tango(last_root, current_node->min_depth - 1, searched);
            current_node = last_root;
        }

        if(current_node->key == searched){ //found the key
            found = true;
        }
        else if(current_node->key > searched){ //go left
            current_node = current_node->left;
        }
        else{ //go right
            current_node = current_node->right;
        }
    }
    return last_root;
}