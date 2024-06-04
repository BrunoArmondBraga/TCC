#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <climits>

using namespace std;

struct matrix_pair{
    vector<vector<float>> first;
    vector<vector<float>> second;
};

void print_vector(vector<float> e){
    cout << "-------------------" << endl;
    for(int i = 0; i < e.size(); i++){
        cout << e[i] << " ";
    }
    cout << "-------------------" << endl;
    cout << endl;
}

void print_matrix(vector<vector<float>> e){
    cout << "-------------------" << endl;
    for(int i = 0; i < e.size(); i++){
        for(int j = 0; j < e[0].size(); j++){
            cout << e[i][j] << "     ";
        }
        cout << endl;
    }
    cout << "-------------------" << endl;
    cout << endl;

    /* for(int i = 0; i < sizeof(c) / sizeof(c[0]); i++){
        for(int j = 0; j < sizeof(c[0]) / sizeof(c[0][0]); j++){
            cout << c[i][j] << " ";
        }
        cout << endl;
    } */
}

int optimal_BST(vector<int> e, int n){
    int c[n+1][n+1];
    int s[n + 1];
    int root[n][n];

    for(int i = 0; i < n + 1; i++){
        for(int j = 0; j < n + 1; j++){
            c[i][j] = 0;
        }
    }

    cout << "--------" << endl;
    for(int i = 1; i < n + 1; i++){
        for(int j = 0; j < n + 1; j++){
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
    cout << "--------" << endl;
    
    s[0] = 0;

    for(int i = 1; i < n + 1; i++){
        int aux = s[i - 1] + e[i - 1];
        s[i] = aux;
    } 

    for(int l = 1; l < n + 1; l++){
        for(int i = 1; i < n - l + 2; i++){
            int j = i + l - 1;

            int aux = c[i + 1][j];
            c[i][j] = aux;

            for(int k = i + 1; k < j + 1; k++){
                if(c[i][k - 1] + c[k + 1][k] < c[i][j]){
                    aux = c[i][k - 1] + c[k + 1][j];
                    c[i][j] = aux;
                }
            }


            aux =  c[i][j] + s[j] - s[i -1];
            c[i][j] = aux;
        }
    }

    cout << "--------" << endl;
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
    cout << "--------" << endl;

    cout << c[1][n] << endl;
    return c[1][n];
}



int main(){
    vector<int> p;

    p.push_back(10);
    p.push_back(20);
    p.push_back(30);
    p.push_back(15);
    p.push_back(30);

    /* p.push_back(20);
    p.push_back(10);
    p.push_back(40); */

    optimal_BST(p,p.size());    
}