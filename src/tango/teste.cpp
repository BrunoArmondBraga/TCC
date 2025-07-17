#include <iostream>
#include <queue>

using namespace std;

/* class NodeARN {
public:
    int key;
    NodeARN* esq;
    NodeARN* dir;
    
    NodeARN(int k) : key(k), esq(nullptr), dir(nullptr) {}

    ~NodeARN(){
        if(esq != nullptr){
            delete esq;
        }
        if(dir != nullptr){
            delete dir;
        }
    }
}; */

class NodeARN{
    private:
        
    public:
        int key;
        bool preto;
        int black_height = 1;
        NodeARN *pai;
        NodeARN *esq;
        NodeARN *dir;
        NodeARN() {
            preto = false; 
            pai = nullptr;
            esq = nullptr;
            dir = nullptr;
        };
        ~NodeARN(){
            if(esq != nullptr){
                delete esq;
            }
            if(dir != nullptr){
                delete dir;
            }
        }
        NodeARN(int key){
            preto = false;
            black_height = 1;
            esq = nullptr;
            dir = nullptr;
            pai = nullptr;
            this->key = key;
        }
        NodeARN(NodeARN* pai,int key){
            preto = false;
            esq = nullptr;
            dir = nullptr;
            if(pai == nullptr){
                this->pai = nullptr;
            }
            else{
                this->pai = pai;
            }
            this->key = key;
        }
};

void debug_tree(NodeARN* u, int i = 0){
    if(u == nullptr){
        return;
    }
    if(u->dir != nullptr){
        debug_tree(u->dir, i+3);
    }

    for(int j=0;j<i;j++){
        cout << " ";
    }
    cout << u->key <<  endl;
    if(u->esq != nullptr){
        debug_tree(u->esq, i+3);
    }
} 

NodeARN* build_BST(int n, int offset = 0){
    if(n <= 0){
        return nullptr;
    }
    else if(n == 1){
        return new NodeARN(offset + 1);
    }

    int x = 1;
    while(x*2 - 1 <= n){
        x = x * 2;
    }

    int left, right = 0;
    
    left = n - x + 1;
    
    if(left > x/2){
        right = left - x/2;
        left = x/2;
    }
    
    left += (x - 2)/2;
    right += (x - 2)/2;

    NodeARN* new_node = new NodeARN(left + offset + 1);
    new_node->esq = build_BST(left, offset);
    new_node->dir = build_BST(right, offset + left + 1);
    return new_node;
}

int main() {
    int n = 1;
    std::cin >> n;

    NodeARN* root = build_BST(n);

    debug_tree(root);

    delete root;

    return 1;
}
