#include <iostream>
#include <vector>
#include "BST.cpp"

using namespace std;

void print_keys(vector<int> e){
    cout << "-----------------chaves------------------" << endl;
    cout << "(";
    for(int i = 0; i < e.size()-1; i++){
        cout << e[i] << ", ";
    }
    cout << e[e.size() - 1] << ")" << endl << endl;
}

node* create_tree(int i, int j, int** root, node* abb){
    if(i > j){
        return abb;
    }

    int k = root[i][j];

    abb = insert(abb, k);

    abb = create_tree(i, k - 1, root, abb);
    abb = create_tree(k + 1, j, root, abb);
    return abb;
}

int optimal_BST(vector<int> e, int n, int** root, node* abb){
    int c[n + 2][n + 2];
    int s[n + 1];

    for(int i = 0; i <= n + 1; i++){
        for(int j = 0; j <= n + 1; j++){
            c[i][j] = 0;
            root[i][j] = 0;
        }
    }
    
    s[0] = 0;

    for(int i = 1; i <= n; i++){
        s[i] = s[i - 1] + e[i - 1];
    } 

    for(int l = 1; l <= n; l++){
        for(int i = 1; i <= n - l + 1; i++){
            int j = i + l - 1;

            c[i][j] = c[i + 1][j];
            root[i][j] = i;

            for(int k = i + 1; k <= j; k++){
                if(c[i][k - 1] + c[k + 1][j] < c[i][j]){
                    c[i][j] = c[i][k - 1] + c[k + 1][j];
                    root[i][j] = k;
                }
            }
            c[i][j] = c[i][j] + s[j] - s[i - 1];
        }
    }

    cout << "---------  Matriz  de  custos  ---------" << endl;
    cout << "n = " << n << endl;
    for(int i = 1; i <= n; i++){
        for(int j = 0; j <= n; j++){
            if(c[i][j] < 10){
                cout << "   ";
            }
            else if(c[i][j] < 100){
                cout << "  ";
            }
            else{
                cout << " ";
            }
            cout << c[i][j];
        }
        cout << endl;
    }

    cout << "---------  Matriz  de  raízes  ---------" << endl;
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            cout << " " << root[i][j];
        }
        cout << endl;
    }
    cout << "----------------------------------------" << endl;

    cout << "Custo mínimo para ABB: " << c[1][n] << endl << endl;
    abb = create_tree(1, n, root, abb);
    debug_rec(abb,0);
    return c[1][n];
}

    

int main(){
    vector<int> p;
    int n;

    cout << "Informe o número de chaves: ";
    cin >> n;

    cout << "Informe as frequências:" << endl;
    for(int i=0;i<n;i++){
        int aux;
        cin >> aux;
        p.push_back(aux);
    }
    print_keys(p);

    int** root = new int*[n + 2];
    for (int i = 0; i <= n + 1; ++i) {
        root[i] = new int[n + 2];
    }

    struct node* abb = nullptr;
    optimal_BST(p, n, root, abb);    

    for (int i = 0; i <= n; ++i) {
        delete[] root[i];
    }
    delete[] root;

    return 0;
}