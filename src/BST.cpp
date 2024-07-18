#include <iostream>

using namespace std;

struct node {
    int key;
    struct node* left, *right;
};

struct node* newNode(int key){
    struct node* newNode = new struct node;
    newNode->key = key;
    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

struct node* insert(struct node* node, int key){
    if(node == nullptr){
        return newNode(key);
    }

    if(key < node->key){
        node->left = insert(node->left, key);
    }
    else{
        node->right = insert(node->right, key);
    }

    return node;
}

struct node* search(struct node* node, int key){
    if(node == nullptr || node->key == key){
        return node;
    }

    if(node->key < key){
        return search(node->right, key);
    }

    return search(node->left, key);
}

void debug_rec(struct node* node, int t){
    if(node == nullptr){
        return;
    }
    debug_rec(node->right, t + 3);

    for(int i = 0; i < t; i++){
        cout << " ";
    }
    cout << node->key << endl;

    debug_rec(node->left, t + 3);
}
