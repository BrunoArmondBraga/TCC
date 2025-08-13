#include <random>
#include <ctime>
#include <iostream>
#include "Auxiliary.h"

using namespace std;

int main(){
    int n, mode;
    n = 5000;
    mode = 3;

    Node* root = build_initial_tango(n);


    int seed = 0;
    while(seed < 10){   
        Node* root = build_initial_tango(n);
        int counter = 1;
        int num_test = 100;
        bool printing = false;

        while(true){
            mt19937 rng(seed);
            uniform_int_distribution<int> dist(1, n);
            cout << "Seed = " << seed << endl;

            for (int i = 0; i < num_test; ++i) {
                int n = dist(rng);
                cout << "Search n°" << counter << ", Searching for: " << n <<  endl;

                bool in_tree = false;
                root = search_in_tango(root, n, in_tree);
                counter++;
                if(printing){
                    print_trees(root, true);
                }
            }
            break;
        }
        //print_trees(root);
        delete root;
        seed++;
    }
    cout << "OKK!" << endl;
}
