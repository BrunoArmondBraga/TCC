#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>

using namespace std;

bool comparePairs(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return a.second < b.second;
}

class LinkedList{
    public:
    int data;
    LinkedList* next;

    LinkedList(int value) : data(value), next(nullptr) {};

    void print() {
        LinkedList* current = this;
        while (current) {
            if(current->data == INT_MAX){
                cout << "+inf" << " -> ";
            }
            else{
                cout << current->data << " -> ";
            }
            current = current->next;
        }
        cout << "nullptr";
    }
};

class Node{
    public:
        int val;
        Node *left;
        Node *right;
        int priority; //next_acess
        int LastAcess;
        Node(int val, int priority){
            left = nullptr;
            right = nullptr;
            this->priority = priority;
            this->val = val;
            LastAcess = -1;
        }
        ~Node(){
            if(left != nullptr){
                delete left;
            }
            if(right != nullptr){
                delete right;
            }
        }
};

bool is_left_child_lower(Node* u){
    if(u->left == nullptr){
        return false;
    }
    if(u->right == nullptr){
        return true;
    }
    if(u->left->priority < u->right->priority){
        return true;
    }
    return false;
}

Node* max_with_same_priority(Node* u){
    if(u->right != nullptr && u->priority == u->right->priority){
        return max_with_same_priority(u->right);
    }
    return u;
}

Node* min_with_same_priority(Node* u){
    if(u->left != nullptr && u->priority == u->left->priority){
        return min_with_same_priority(u->left);
    }
    return u;
}

class Treap {
    private: 
        Node *root;

        Node* put(Node *node, int val, int priority){
            if(node == nullptr){
                Node *inserir = new Node(val, priority); 
                return inserir;
            }
            if(node->val > val){
                node->left = put(node->left,val,priority);
                if (node->left != nullptr && node->left->priority < node->priority){
                    node = right_rotation(node);
                }
            } 
            else{
                node->right = put(node->right,val,priority);
                if (node->right != nullptr && node->right->priority < node->priority){
                    node = left_rotation(node);
                }
            }
            return node;
        }

        Node* right_rotation(Node *node){
            Node *new_root = node->left;
            Node *son = new_root->right;

            new_root->right = node;
            node->left = son;

            return new_root;
        }

        Node* left_rotation(Node *node){
            Node *new_root = node->right;
            Node *son = new_root->left;

            new_root->left = node;
            node->right = son;

            return new_root;
        }

    public:
        Treap() {
            root = nullptr;
        }

        ~Treap() {
            delete root;
        }

        void add (int val, int priority){
            root = put(root,val, priority);
        }

        void debug_rec(Node* node, int space){
            if(node == nullptr){
                return;
            }

            if(node != nullptr && node->right != nullptr){
                debug_rec(node->right, space + 3);
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

            if(node != nullptr && node->left != nullptr){
                debug_rec(node->left, space + 3);
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
        }

        Node* sink_node(vector<LinkedList*>& next_acess, Node* root, int time, bool& stop){

            Node* current = root;
            if(current->priority < time){ //only change if it wasn't changed already
                current->LastAcess = current->priority; //change LastAcess to now
                current->priority = next_acess[current->val]->data; //change N(x,now) to next access
                next_acess[current->val] = next_acess[current->val]->next; //remove N(x,now) from the next_acess table
            }

            
            //see if left is wrong!
            Node* left = current->left;
            if(left != nullptr){
                if(current->priority > left->priority && left->LastAcess != current->LastAcess && current->LastAcess != left->priority){
                    Node* finalNode = max_with_same_priority(left); //get the correct point
                    cout << "The pair of points { (" << current->val << "," << current->LastAcess << ") , ";
                    cout << "(" << finalNode->val << "," << finalNode->priority << ") } ";
                    cout << "is not arborally satisfied!" << endl;
                    stop = true; //stop execution
                    return current;
                }
            }

            //see if right is wrong!
            Node* right = current->right;
            if(right != nullptr){
                if(current->priority > right->priority && right->LastAcess != current->LastAcess && current->LastAcess != right->priority){
                    Node* finalNode = min_with_same_priority(right); //get the correct point
                    cout << "The pair of points { (" << current->val << "," << current->LastAcess << ") , ";
                    cout << "(" << finalNode->val << "," << finalNode->priority << ") } ";
                    cout << "is not arborally satisfied!" << endl;
                    stop = true; //stop execution
                    return current;
                }
            }

            if(is_left_child_lower(current)){ //if the left priority lower, start searching on the left child
                if(left != nullptr && current->priority > left->priority){
                    current = right_rotation(current);
                    current->right = sink_node(next_acess, current->right, time, stop);            
                }
                else if(right != nullptr && current->priority > right->priority){
                    current = left_rotation(current);
                    current->left = sink_node(next_acess, current->left, time, stop);
                }
            }
            else{ //if the right priority is lower, start searching on the right child
                if(right != nullptr && current->priority > right->priority){
                    current = left_rotation(current);
                    current->left = sink_node(next_acess, current->left, time, stop);
                }
                else if(left != nullptr && current->priority > left->priority){
                    current = right_rotation(current);
                    current->right = sink_node(next_acess, current->right, time, stop);            
                }
            }
            return current;
        }

        int sink_root(vector<LinkedList*>& next_acess, int now, bool& stop){
            if(isEmpty()){
                return -1;
            }

            while(root != nullptr && root->priority < now && !stop){ //sink all roots with priority < now
                root = sink_node(next_acess, root, now, stop);
            }
            print();
            return 0;
        }

        bool isEmpty(){
            if(root == nullptr){
                return true;
            }
            return false;
        }
};

int main(){
    cout << "Fale o número de pontos e, em seguida, os pontos na forma (x, y)." << endl;

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

    sort(data.begin(), data.end(), comparePairs); //sort based on the y-coordinaate


    for(int i = 0; i < data.size(); i++){ //get n and m values
        if(data[i].first > n){
            n = data[i].first;
        }
        if(data[i].second > m){
            m = data[i].second;
        }
    }

    cout << "\nn = " << n << ", e m = " << m << endl << endl; //print n and m values 

    vector<LinkedList*> next_acess;

    for(int i = 0; i < n + 1; i++){ //add +inf to all entries
        next_acess.push_back(new LinkedList(INT_MAX));
    }


    for(int i = data.size(); i >= 0; i--){ //add data backwards
        if(next_acess[data[i].first] != nullptr){
            LinkedList* newNode = new LinkedList(data[i].second);
            newNode->next = next_acess[data[i].first];
            next_acess[data[i].first] = newNode;
        }
        else{
            next_acess[data[i].first] = new LinkedList(data[i].second);
        }
    }

    Treap* tr = new Treap(); //create treap

    for(int i = 1; i < next_acess.size(); i++){ //build initial treap
        if(next_acess[i] == nullptr){
            tr->add(i, INT_MAX);
        }
        else{
            tr->add(i,next_acess[i]->data);
            LinkedList* deleted = next_acess[i];
            next_acess[i] = next_acess[i]->next;
            delete deleted;
        }
    }

    cout << "Initial Treap:------------" << endl;
    tr->print();
    cout << "--------------------------" << endl;

    for(int i = 1; i < next_acess.size(); i++){ //print data vector after initial treap
        cout << "i = " << i << ": ";
        if(next_acess[i] != nullptr){
            next_acess[i]->print();
        }
        cout << endl;
    }
    cout << endl;

    int time = 1;
    
    bool stop_signal = false;

    while(time < m + 2 && !stop_signal){ //move time
        cout << "Time = " << time;
        if(time < 10){
            cout << " -----------------" << endl;
        }
        else{
            cout << " ----------------" << endl;
        }
        if(tr->sink_root(next_acess, time, stop_signal) == -1){
            break;
        }
        time = time + 1;
        cout << "--------------------------" << endl;
    }


    delete tr;
}