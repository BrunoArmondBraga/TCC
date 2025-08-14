#include <random>
#include <ctime>
#include <iostream>
#include "Auxiliary.h"

using namespace std;

int main(){
    int n, mode;
    n = mode = -1;

    while(n < 0){
        cout << "n = ";
        cin >> n;
    }

    while(mode < 1 || mode > 3){
        cout << "Choose mode: 1 - iteractive mode, 2 - random test mode with random seed, 3 - random test mode with specific seed." << endl;
        cout << "Mode: ";
        cin >> mode;
    }

    Node* root = build_initial_tango(n);

    if(mode == 1){
        bool running = true;
        while(running){
            cout << "Operations: 0 - print raw, -1 - print first, -2 - print trees, -3 -> end program, anything else - search" << endl;
            cout << "Choose operation: ";
            cin >> n;

            switch(n) {
                case 0:
                    print(root);
                    break;
                case -1:
                    print_trees(root, false);
                    break;
                case -2:
                    print_trees(root);
                    break;
                case -3:
                    running = false;
                    break;
                default:
                    bool in_tree = false;
                    root = search_in_tango(root, n, in_tree);
                    if(in_tree){
                        cout << "Found the key " <<  n << "!" << endl;
                    }
                    else{
                        cout << "The tree does not have key " << n << "!" << endl;
                    }
                    break;
            }
        }
    }
    else{
        int seed = 0;
        int counter = 1;
        int num_test = 1;
        bool printing = false;


        if(mode == 3){
            cout << "Choose seed: ";
            cin >> seed;
        }
        else{
            seed = time(nullptr);
        }

        cout << "How many tests? ";
        cin >> num_test;

        string see;
        cout << "Do you want to see every tree? (Y/N) ";
        cin >> see;
        if(see == "Y"){
            printing = true;
        }
        mt19937 rng(seed);
        uniform_int_distribution<int> dist(1, n);

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
        print_trees(root);
    }     
    delete root;
}
