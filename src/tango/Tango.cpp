#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <stack>
#include "Arn.h"

using namespace std;

/* int main(){
    int n;
    cout << "Quantos nós deseja utilizar?" << endl;
    //cin >> n;

    // NodeARN* root = build_complete_BST(n);
    //imprimir_em_partes(root);
    
    //root = build_tango_tree(root);
    //imprimir_em_partes(root);
    

    bool deu_erro = false;
    vector<int> element = {1,2,3,4,5,6,7,8,9,10};
    for(int i = 0; i < 1000; i++){
        element.push_back(11 + i);
    }


    NodeARN* root = new NodeARN(1);
    for(int i = 1; i < element.size(); i++){
        root = add(root,element[i],deu_erro);
    }

    imprimir(root);

    for(int i = 0; i < element.size(); i++){
        root = bring_to_front(root, element[i]);
        imprimir(root);
        if(root->key != element[i]){
            cout << "DEU ERRO!" << endl;
            break;
        }
    }
    
    delete root;
} */

int main(){
    vector<int> numeros_adicionados = {1,2,3,4,5,6,7,8,9,10,11};
    for(int i = 0; i < 100; i++){
        numeros_adicionados.push_back(12 + i);
    }
    NodeARN* root = nullptr;
    for(int i = 0; i < numeros_adicionados.size(); i++){
        root = add(root, numeros_adicionados[i], nullptr);
    }
    imprimir(root);
    int n = 0;
    bool deu_errado = false;
    while(!deu_errado){
        cout << "Qual quer tchombers?" << endl;
        cin >> n;
        if(n == -1){
            break;
        }

        root = bring_to_front(root, n);
        cout << "print bring_to_front" << endl;
        imprimir(root);
        
        root = concatenate(root);
        cout << "print concatenate" << endl;
        imprimir(root);
    }
    
    if(deu_errado){
        cout << "NAO NAO NAO NAO DROGA" << endl;
    }
    else{
        cout << "UFAAAA :)" << endl;
    }
    
    /* NodeARN* root2 = nullptr;
    for(int i = 0; i < numeros_adicionados.size()/2 + 1; i++){
        root = add(root,numeros_adicionados[i],nullptr);
    }
    for(int i = numeros_adicionados.size()/2 + 1; i < numeros_adicionados.size(); i++){
        root2 = add(root2,numeros_adicionados[i],nullptr);
    }

    imprimir(root);
    imprimir(root2);

    NodeARN* six = new NodeARN(6);

    root = Join(root,six,root2);
    imprimir(root); */

    delete root;
}


/*
int main(){
    bool deu_erro = false;
    bool deu_erro_contagem = false;

    for(unsigned int seed = 1; seed < 2; seed++){
        cout << "SEED = " << seed << endl;
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> distrib(1, 10000);
        std::uniform_int_distribution<> distrib2(20001, 30000);
        vector<int> numeros_adicionados;


        for(int i = 1; i < 1000; i++){
            int numero_sorteado1 = distrib(gen);
            int numero_sorteado2 = distrib2(gen);
            while(std::find(numeros_adicionados.begin(), numeros_adicionados.end(), numero_sorteado1) != numeros_adicionados.end()){
                numero_sorteado1 = distrib(gen);
            }
            numeros_adicionados.push_back(numero_sorteado1);
        }

        
    for(int j = 1; j < numeros_adicionados.size(); j++){
        NodeARN* new_tree = nullptr;

        for(int i=0; i < numeros_adicionados.size(); i++){
            new_tree = add(new_tree, numeros_adicionados[i], deu_erro);
        }
        
        imprimir(new_tree);
        
        
        int searched = numeros_adicionados[j] + 0.5;
        int size_before = size(new_tree);    
        
        split_data oi = Split(new_tree, searched);
        
        if(!deu_erro){
            check_black_height(oi.left_tree, deu_erro);
        }
        if(!deu_erro){
            check_black_height(oi.right_tree, deu_erro);
        }
        else{
            imprimir(oi.left_tree);
        }
        if(deu_erro){
            imprimir(oi.right_tree);
        }
        
        if(size_before != size(oi.left_tree) + size(oi.right_tree)){
            deu_erro_contagem = true;
        }
        
        
        cout << "ESQUERDA -------------------------" << endl;
        imprimir(oi.left_tree);
        cout << "----------------------------------" << endl;
        cout << "DIREITA --------------------------" << endl;
        imprimir(oi.right_tree);
        cout << "----------------------------------" << endl;
    }
        
    }
    if(deu_erro){
        cout << "ERRO BLACK HEIGHT" << endl;
    }
    else if(deu_erro_contagem){
        cout << "ERRO CONTAGEM" << endl;
    }
    else{
        cout << "TUDO OK!" << endl;
    }
    
}

*/


/* // teste 1
    //NodeARN* root = new NodeARN(numeros_adicionados[0]);
    NodeARN* root = add(nullptr,numeros_adicionados[0],nullptr);
    //NodeARN* root2 = new NodeARN(numeros_adicionados[numeros_adicionados.size()/2]);
    NodeARN* root2 = add(nullptr,numeros_adicionados[numeros_adicionados.size()/2],nullptr);
    for(int i = 1; i < numeros_adicionados.size()/2; i++){
        root = add(root,numeros_adicionados[i],nullptr);
    }
    for(int i = numeros_adicionados.size()/2 + 1; i < numeros_adicionados.size(); i++){
        root2 = add(root2,numeros_adicionados[i],nullptr);
    }

    cout << "printe 1 -----------------------------------" << endl;
    imprimir(root);
    cout << "printe 2 -----------------------------------" << endl;
    imprimir(root2);

    NodeARN* six = new NodeARN(2);

    root2 = Join(root, six, root2);
    imprimir(root2);

    root2 = concatenate(root2);
    imprimir(root2); */



    // teste 2
    /* NodeARN* root = new NodeARN(numeros_adicionados[0]);
    root->is_root = true;
    root->preto = true;
    root->black_height = 2;
    for(int i = 1; i < numeros_adicionados.size(); i++){
        imprimir(root);
        root = add(root, numeros_adicionados[i], nullptr);
    }
    imprimir(root);
    root = bring_to_front(root,5);
    imprimir(root); 

    root = concatenate(root);
    imprimir(root); */