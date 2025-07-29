#include <random>
#include <ctime>
#include "Auxiliary.h"

using namespace std;

int main(){
    int n, mode;
    n = mode = -1;

    while(n < 0){
        cout << "n = ";
        cin >> n;
    }

    while(mode != 1 && mode != 2 && mode != 3){
        cout << "Choose mode: 1 - iteractive mode, 2 - random test mode with random seed, 3 - random test mode with specific seed." << endl;
        cout << "Mode: ";
        cin >> mode;
    }

    Node* root = build_complete_BST(n);
    root = build_tango_tree(root);

    if(mode == 1){
        while(true){
            cout << "Operations: 0 - print raw, -1 - print first, -2 - print trees, -3 -> end program, anything else - search" << endl;
            cout << "Choose operation: ";
            cin >> n;
            cout << endl;

            if(n == 0){
                print(root);
            }
            else if(n == -1){
                print_trees(root, false);
            }
            else if(n == -2){
                print_trees(root);
            }
            else if(n == -3){
                break;
            }
            else{
                bool in_tree = false;
                root = search_in_tango(root, n, in_tree);
                if(in_tree){
                    cout << "Found the key " <<  n << "!" << endl;
                }
                else{
                    cout << "The tree does not have key " << n << "!" << endl;
                }
            }
        }
    }
    else{
        int seed = 0;
        if(mode == 3){
            cout << "Choose seed: ";
            cin >> seed;
        }
        else{
            seed = time(nullptr);
        }
        
        int counter = 1;
        int num_test = 0;
        bool printing = 0;

        cout << "How many tests? ";
        cin >> num_test;

        string see;
        cout << "Do you want to see every tree? (Y/N) ";
        cin >> see;
        if(see == "Y"){
            printing = true;
        }

        while(true){
            mt19937 rng(seed);
            uniform_int_distribution<int> dist(1, n);

            for (int i = 0; i < num_test; ++i) {
                int n = dist(rng);
                cout << "Search n°" << counter << ", Searching for: " << n <<  endl;

                bool in_tree = false;
                root = search_in_tango(root, n, in_tree);
                counter++;
                if(printing){
                    print_trees(root, false);
                }
            }
            break;
        }
        print_trees(root);
    }
    delete root;
}
