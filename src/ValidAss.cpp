#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <climits> // for INT_MAX

using namespace std;

bool comparePairs(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return a.second < b.second;
}

class Node{
    public:
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {};

    void print() {
        Node* current;
        cout << data << " -> ";
        current = next;
        while (current) {
            if(current->data == INT_MAX){
                cout << "+inf " << " -> ";
            }
            else{
                cout << current->data << " -> ";
            }
            current = current->next;
        }
        cout << "nullptr";
    }
};

class NodeTR{
    private:
        
    public:
        int val;
        NodeTR *esq;
        NodeTR *dir;
        int priority; //NextAcess
        int LastAcess;
        NodeTR() { 
            esq = nullptr;
            dir = nullptr;
            priority = 0;
            LastAcess = -1;
        };
        NodeTR(int val){
            esq = nullptr;
            dir = nullptr;
            priority = rand();
            this->val = val;
            LastAcess = -1;
        }
        NodeTR(int val, int priority){
            esq = nullptr;
            dir = nullptr;
            this->priority = priority;
            this->val = val;
            LastAcess = -1;
        }
        ~NodeTR(){
            if(esq != nullptr){
                delete esq;
            }
            if(dir != nullptr){
                delete dir;
            }
        }
};

class Treap {
    private: 
        NodeTR *root;

        NodeTR* put(NodeTR *node, int val, int priority){
            if(node == nullptr){
                NodeTR *inserir = new NodeTR(val, priority); 
                return inserir;
            }
            if(node->val > val){
                node->esq = put(node->esq,val,priority);
                if (node->esq != nullptr && node->esq->priority < node->priority){
                    node = right_rotation(node);
                }
            } 
            else{
                node->dir = put(node->dir,val,priority);
                if (node->dir != nullptr && node->dir->priority < node->priority){
                    node = left_rotation(node);
                }
            }
            return node;
        }

        NodeTR* right_rotation(NodeTR *node){
            NodeTR *new_root = node->esq;
            NodeTR *son = new_root->dir;

            new_root->dir = node;
            node->esq = son;

            return new_root;
        }

        NodeTR* left_rotation(NodeTR *node){
            NodeTR *new_root = node->dir;
            NodeTR *son = new_root->esq;

            new_root->esq = node;
            node->dir = son;

            return new_root;
        }

    public:
        Treap() {
            root = nullptr;
            srand(time(0));
        }

        ~Treap() {
            delete root;
        }

        void add (int val, int priority){
            root = put(root,val, priority);
        }

        void debug_rec(NodeTR* node, int space){
            if(node == nullptr){
                return;
            }

            if(node != nullptr && node->dir != nullptr){
                debug_rec(node->dir, space + 3);
            }
            for(int j = 0; j < space; j++){
                cout << " ";
            }

            cout << node->val << " ";
            if(node->priority == INT_MAX){
                cout << "+inf";
            }
            else{
                cout << node->priority;
            }
            cout << endl;

            if(node != nullptr && node->esq != nullptr){
                debug_rec(node->esq, space + 3);
            }

        }

        void print(){
            if(root == nullptr){
                cout << "Treap empty!!" << endl;
            }
            else{
                debug_rec(root, 3);
            }
            cout << endl;
            cout << endl;
        }
        

        NodeTR* sink_node(vector<Node*>& nextAcess, NodeTR* root, int time){

            NodeTR* current = root;
            if(current->priority < time){
                current->LastAcess = current->priority; //change LastAcess to now
                current->priority = nextAcess[current->val]->data; //change N(x,now) to next access
                nextAcess[current->val] = nextAcess[current->val]->next; //remove N(x,now) from the nextAcess table
            }

            
            //see if left is wrong!
            NodeTR* left = current->esq;
            if(left != nullptr){
                if(current->priority > left->priority && left->LastAcess != current->LastAcess && current->LastAcess != left->priority){
                    cout << "The pair of points { (" << current->val << "," << current->LastAcess << ") , ";
                    cout << "(" << left->val << "," << left->priority << ") } ";
                    cout << "is not arborally satisfied!" << endl;
                    return nullptr;
                }
            }

            NodeTR* right = current->dir;
            if(right != nullptr){
                if(current->priority > right->priority && right->LastAcess != current->LastAcess && current->LastAcess != right->priority){
                    cout << "The pair of points { (" << current->val << "," << current->LastAcess << ") , ";
                    cout << "(" << right->val << "," << right->priority << ") } ";
                    cout << "is not arborally satisfied!" << endl;
                    return nullptr;
                }
            }

            if(left != nullptr && current->priority > left->priority){
                /* if(left->LastAcess == current->LastAcess){
                    return right_rotation(current);
                } */
                current = right_rotation(current);
                current->dir = sink_node(nextAcess, current->dir, time);            
            }
            else if(right != nullptr){
                if(current->priority > right->priority){
                    /* if(right->LastAcess == current->LastAcess){
                        return left_rotation(current);
                    } */
                    current = left_rotation(current);
                    current->esq = sink_node(nextAcess, current->esq, time);
                }
            }
            return current;
        }


        int sink_root(vector<Node*>& nextAcess, int t){
            if(root == nullptr){
                return -1;
            }
            if(root->priority > t){
                print();
                return 0;
            }
            if(root != nullptr && root->priority < t){
                while(root != nullptr && root->priority < t){
                    root = sink_node(nextAcess, root, t);
                    print();
                }
                
                if(root == nullptr){
                    print();
                    return -1;
                }
                return 0;
            }
            print();
            return 0;
        }

        bool isEmpty(){
            if(root == nullptr){
                return true;
            }
        }
};

int main(){
    cout << "Informações sobre o problema:\nFale o número de pontos e, em seguida, os pontos na forma (x, y)." << endl;

    int n,u,v;
    int m = 0;
    cin >> n;

    vector<pair<int, int>> data;

    for(int i = 0; i < n; i++){
        cin >> u;
        cin >> v;
        data.push_back({u,v});
    }
    n = 0;

    sort(data.begin(), data.end(), comparePairs);


    for(int i = 0; i < data.size(); i++){
        if(data[i].first > n){
            n = data[i].first;
        }
        if(data[i].second > m){
            m = data[i].second;
        }
    }

    /* std::cout << "Array ordenado:\n";
    for (const auto& pair : data) {
        std::cout << "(" << pair.first << ", " << pair.second << ")\n";
    } */

    vector<Node*> nextAcess;
    for(int i = 0; i < n + 1; i++){
        //nextAcess.push_back(nullptr);
        nextAcess.push_back(new Node(INT_MAX));
    }
    cout << "\nn = " << n << ", e m = " << m << endl << endl;; 
    for(int i = data.size(); i >= 0; i--){
        //cout << "Colocando " << data[i].second << " em " << data[i].first << endl;
        if(nextAcess[data[i].first] != nullptr){
            Node* newNode = new Node(data[i].second);
            newNode->next = nextAcess[data[i].first];
            nextAcess[data[i].first] = newNode;
        }
        else{
            nextAcess[data[i].first] = new Node(data[i].second);
        }
    }

    Treap* tr = new Treap();

    for(int i = 1; i < nextAcess.size(); i++){
        if(nextAcess[i] == nullptr){
            tr->add(i,INT_MAX);
        }
        else{
            tr->add(i,nextAcess[i]->data);
            Node* deleted = nextAcess[i];
            nextAcess[i] = nextAcess[i]->next;
            delete deleted;
        }
    }

    for(int i = 1; i < nextAcess.size(); i++){
        cout << "i = " << i << ": ";
        if(nextAcess[i] != nullptr){
            nextAcess[i]->print();
        }
        cout << endl;
    }

    tr->print();

    
    
    int time = 1;

    cout << "m = " << m << endl;

    while(time < m + 1){
        cout << "Time = " << time  << "-------" << endl;
        if(tr->sink_root(nextAcess, time) == -1){
            break;
        }
        time = time + 1;
        cout << "---------------" << endl;
        for(int i = 1; i < nextAcess.size(); i++){
            cout << "i = " << i << ": ";
            if(nextAcess[i] != nullptr){
                nextAcess[i]->print();
            }
            cout << endl;
        }
        cout << "---------------" << endl;
    }


    delete tr;
}


/* 
29
1 9
2 6
2 9
3 1
3 2
3 4
3 6
4 4
4 5
4 6
4 7
4 9
4 10
5 5
5 7
6 7
6 10
7 10
8 2
8 3
8 4
8 5
8 7
8 8
8 9
8 10
9 8
10 3
10 8 
*/

/* 
8
3 1
3 2
3 4
4 4
8 2
8 3
8 4
10 3
*/