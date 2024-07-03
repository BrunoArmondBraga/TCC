#include <iostream>
#include <vector>

using namespace std;

void print_vector(vector<int> e){
    cout << "-----------------chaves------------------" << endl;
    cout << "(";
    for(int i = 0; i < e.size()-1; i++){
        cout << e[i] << ", ";
    }
    cout << e[e.size() - 1] << ")" << endl << endl;
}

void debug_tree_rec(int i, int j, int spacing, int** root){
    if(i > j){
        return;
    }

    int k = root[i][j];
    
    debug_tree_rec(k+1, j, spacing + 3, root);
    for(int i = 0; i < spacing; i++){
        cout << " ";
    }

    cout << k << endl;
    
    debug_tree_rec(i, k-1, spacing + 3, root);
}

int optimal_BST(vector<int> e, int n, int** root){
    int c[n+1][n+1];
    int s[n + 1];

    for(int i = 0; i < n + 1; i++){
        for(int j = 0; j < n + 1; j++){
            c[i][j] = 0;
            root[i][j] = 0;
        }
    }
    
    s[0] = 0;

    for(int i = 1; i < n + 1; i++){
        s[i] = s[i - 1] + e[i - 1];
    } 

    for(int l = 1; l < n + 1; l++){
        for(int i = 1; i < n - l + 2; i++){
            int j = i + l - 1;

            c[i][j] = c[i + 1][j];
            root[i][j] = i;

            for(int k = i + 1; k < j + 1; k++){
                if(c[i][k - 1] + c[k + 1][k] < c[i][j]){
                    int aux = c[i][k - 1] + c[k + 1][j];
                    c[i][j] = aux;
                    root[i][j] = k;
                }
            }
            c[i][j] = c[i][j] + s[j] - s[i -1];
        }
    }

    cout << "---------  Matriz  de  custos  ---------" << endl;
    for(int i = 1; i < n + 1; i++){
        for(int j = 0; j < n + 1; j++){
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
    for(int i = 1; i < n+1; i++){
        for(int j = 1; j < n+1; j++){
            cout << " " << root[i][j];
        }
        cout << endl;
    }
    cout << "----------------------------------------" << endl;

    cout << "Custo mínimo para ABB: " << c[1][n] << endl << endl;
    debug_tree_rec(1, n, 0, root);
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
    print_vector(p);

    int** root = new int*[n+1];
    for (int i = 0; i <= n; ++i) {
        root[i] = new int[n+1];
    }

    optimal_BST(p, n, root);    

    for (int i = 0; i <= n; ++i) {
        delete[] root[i];
    }
    delete[] root;

    return 0;
}