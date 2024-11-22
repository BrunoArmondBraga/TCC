#include <iostream>
#include <cstdlib>
#include <string>

#define DEBUG_TEST false

using namespace std;

class Node{
    public:
        int val;

        Node *parent;
        Node *esq;
        Node *dir;

        Node() { 
            parent = nullptr;
            esq = nullptr;
            dir = nullptr;
        }
        Node(int val){
            this->parent = nullptr;
            esq = nullptr;
            dir = nullptr;
            this->val = val;
        }
        Node(int val, Node* dad){
            this->parent = dad;
            esq = nullptr;
            dir = nullptr;
            this->val = val;
        }
        ~Node(){
        }
};

struct Node_pair{
    Node* first;
    Node* second;
};

void debug_rec(Node *u, int i){
    if(u == nullptr){
        cout << "nullptr" << endl;
        return;
    }
    if(u->esq != nullptr){
        debug_rec(u->esq, i+3);
    }

    for(int j=0;j<i;j++){
        cout << " ";
    }
    cout << u->val << " ";
    if(DEBUG_TEST){
        cout << "  pai = ";
        if(u->parent != nullptr){
            cout << u->parent->val;
        }
        else{
            cout << "nullptr";
        }
        
    }
    cout << endl;

    if(u->dir != nullptr){
        debug_rec(u->dir, i+3);
    }
}

class SplayTree {
    private: 
        Node *raiz;

        Node* right_rotation(Node *node){
            Node *new_root = node->esq;
            Node *son = new_root->dir;
            Node *top = node->parent;

            new_root->dir = node;
            new_root->parent = node->parent;
            node->parent = new_root;
            node->esq = son;
            if(son != nullptr){
                son->parent = node;
            }

            if(top != nullptr){
                if(new_root->val < top->val){
                    top->esq = new_root;
                }
                else{
                    top->dir = new_root;
                }
            }

            return new_root;
        }

        Node* left_rotation(Node *node){ //node
            Node *new_root = node->dir; // new_root = node->dir
            Node *son = new_root->esq;  // son = node->dir->esq
            Node *top = node->parent;

            new_root->esq = node; //MUDA
            new_root->parent = node->parent;
            node->parent = new_root;
            node->dir = son; //muda
            if(son != nullptr){
                son->parent = node;
            }

            if(top != nullptr){
                if(new_root->val < top->val){
                    top->esq = new_root;
                }
                else{
                    top->dir = new_root;
                }
            }

            return new_root;
        }

        void add(Node *node,int val){
            if(node->val > val){ //vai pra esquerda!
                if(node->esq == nullptr){
                    //achou!!
                    Node *new_node = new Node(val,node);
                    node->esq = new_node;
                    splay(node->esq);
                }
                else{
                    add(node->esq,val);
                }
            } 
            else{ //vai pra direita!!
                if(node->dir == nullptr){
                    //achou!!
                    Node *new_node = new Node(val,node);
                    node->dir = new_node;
                    splay(node->dir);
                }
                else{
                    add(node->dir,val);
                }
            }
        }

        void substitute(Node* old, Node* current){
            if(old->parent != nullptr){
                if(old->parent->dir == old){
                    old->parent->dir = current;
                }
                else{
                    old->parent->esq = current;
                }
            }
            if(current != nullptr){
                current->parent = old->parent;
            }
        }

        void remove(Node *node, int t){
            if(node == nullptr){
                cout << "tem algo errado aqui!" << endl;
                return;
            }
            if(node->val == t){
                if(node->dir == nullptr){
                    substitute(node, node->esq);
                    if(node->esq != nullptr && node->esq->parent == nullptr){
                        raiz = node->esq;
                    }
                    else{
                        splay(node->parent);
                    }
                }
                else if(node->esq == nullptr){
                    substitute(node, node->dir);
                    
                    if(node->dir != nullptr && node->dir->parent == nullptr){
                        raiz = node->dir;
                    }
                    else{
                        splay(node->parent);
                    }
                }
                else{
                    //problema!!!!

                    Node *min = node;
                    while(min->esq != nullptr){
                        min = min->esq;
                    }

                    if(min->parent != nullptr){
                        min->parent->esq = min->dir;
                    }

                    min->parent = node->parent;
                    min->dir = node->dir;
                    min->esq = node->esq;

                    if(min->parent != nullptr){
                        if(min->parent->esq == node){
                            min->parent->esq = min;
                        }
                        else{
                            min->parent->dir = min;
                        }
                    }
                    if(min->dir != nullptr){
                        min->dir->parent = min;
                    }
                    if(min->esq != nullptr){
                        min->esq->parent = min;
                    }

                    
                    splay(min);
                }
                return;
            }
            else if(node->val > t){
                if(node->esq == nullptr){
                    splay(node);
                    return;
                }
                remove(node->esq, t);
            }
            else{
                if(node->dir == nullptr){
                    splay(node);
                    return;
                }
                remove(node->dir, t);
            }
        }    

        int min(Node *u){
            if(u->esq == nullptr){
                splay(u);
                return u->val;
            }
            return min(u->esq);
        }

        Node* max(Node *u){
            if(u == nullptr){
                return nullptr;
            }
            if(u->dir == nullptr){
                return u;
            }
            return max(u->dir);
        }

        bool search(Node *u, int x){
            if(u == nullptr){
                return false;
            }
            if(u->val == x){
                splay(u);
                return true;
            }
            else if(u->val > x){
                if(u->esq == nullptr){
                    splay(u);
                    return false;
                }
                return search(u->esq,x);
            }
            else{
                if(u->dir == nullptr){
                    splay(u);
                    return false;
                }
                return search(u->dir,x);
            }
        }

        void splay(Node *u){
            if(u->parent == nullptr){
                raiz = u;
                return;
            }
            if(u->parent->parent == nullptr){ //filho da raiz!
                //rotação única!
                if(u->parent->esq == u){
                    //roda pra direita
                    raiz = right_rotation(u->parent);
                }
                else{
                    //roda pra esquerda
                    raiz = left_rotation(u->parent);
                }
                return;
            }
            if(u == u->parent->esq && u->parent == u->parent->parent->esq){
                //caso / pagina 14.
                u->parent = right_rotation(u->parent->parent);
                u = right_rotation(u->parent);
            }
            else if(u == u->parent->esq && u->parent == u->parent->parent->dir){
                //análogo \ / pagina 15.
                u = right_rotation(u->parent);
                u = left_rotation(u->parent);
            }
            else if(u == u->parent->dir && u->parent == u->parent->parent->dir){
                //análogo \ pagina 14.
                u->parent = left_rotation(u->parent->parent);
                u = left_rotation(u->parent);
            }
            else if(u == u->parent->dir && u->parent == u->parent->parent->esq){
                //caso / \ pagina 15.
                u = left_rotation(u->parent);
                u = right_rotation(u->parent);
            }
            splay(u);
        }

        Node* find_split_node(Node *u, int t){
            if(u->val == t){
                return u;
            }
            if(u->val > t){
                if(u->esq == nullptr){
                    return u;
                }
                return find_split_node(u->esq, t);
            }
            else{
                if(u->dir == nullptr){
                    return u;
                }
                return find_split_node(u->dir, t);
            }
        }

        void join(Node_pair pair){
            Node *a = pair.first;

            if(a != nullptr){
                splay(max(a));
                a->dir = pair.second;
                if(a->dir != nullptr){
                    a->dir->parent = a;
                }

                raiz = a;
            }
            else{
                raiz = pair.second;
            }
        }

    public:
        SplayTree() {
            raiz = nullptr;
        }
        ~SplayTree() {
            delete raiz;
        }

        void insert(int t){
            if(raiz == nullptr){
                raiz = new Node(t);
            }
            else{
                add(this->raiz, t);
            }   
        }

        void delete_node(int val){
            if(raiz == nullptr){
                cout << "Não é possível retirar elementos de uma Splay Tree vazia!" << endl;
                return;
            }
            if(raiz->esq == nullptr && raiz->dir == nullptr){
                raiz = nullptr;
                return;
            }
            remove(this->raiz, val);
        }

        void print(){
            debug_rec(this->raiz,0);
        }

        int print_min(){
            if(raiz == nullptr){
                cout << "Treap vazia!" << endl;
                return -1;
            }
            return min(this->raiz);
        }

        int print_search(int x){
            if(raiz == nullptr){
                return false;
            }
            return (search(this->raiz, x)? 1 : 0);
        }

        void print_split(int x){
            splay(find_split_node(this->raiz, x));
            Node_pair split;

            if(this->raiz->val <= x){
                Node* right_split = raiz->dir;
                if(raiz->dir != nullptr){
                    raiz->dir->parent = nullptr;
                }
                raiz->dir = nullptr;

                split.first = raiz;
                split.second = right_split;
            }
            else{
                split.first = nullptr;
                split.second = raiz;
            }

            debug_rec(split.first, 0);
            debug_rec(split.second, 0);

            join(split);
        }
};

int main(){
    int numero, x;
    SplayTree *ST = new SplayTree(); 

    while(cin >> numero){
        switch (numero)
        {
        case 1:
            cin >> x;
            ST->insert(x);
            break;
        case 2:
            cin >> x;
            ST->delete_node(x);
            break;
        case 3:
            cin >> x;
            cout << ST->print_search(x) << endl;
            break;
        case 4:
            cout << ST->print_min() << endl;;
            break;
        case 5:
            ST->print();
            break;
        case 6:
            cin >> x;
            ST->print_split(x);
            break;
        }
    }
}
