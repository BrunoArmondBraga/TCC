#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <stack>

using namespace std;

struct split_data
{
    NodeARN* left_tree;
    NodeARN* right_tree;
};


class NodeARN{
    private:
        
    public:
        int key;
        bool preto;
        int black_height = 1;
        NodeARN *pai;
        NodeARN *esq;
        NodeARN *dir;
        NodeARN() {
            preto = false; 
            pai = nullptr;
            esq = nullptr;
            dir = nullptr;
        };
        ~NodeARN(){
            if(esq != nullptr){
                delete esq;
            }
            if(dir != nullptr){
                delete dir;
            }
        }
        NodeARN(int key){
            preto = false;
            black_height = 1;
            esq = nullptr;
            dir = nullptr;
            pai = nullptr;
            this->key = key;
        }
        NodeARN(NodeARN* pai,int key){
            preto = false;
            esq = nullptr;
            dir = nullptr;
            if(pai == nullptr){
                this->pai = nullptr;
            }
            else{
                this->pai = pai;
            }
            this->key = key;
        }
};

class Arn {
    private: 
        bool refaz;
        NodeARN *raiz;

        NodeARN *rodaEsq(NodeARN *r){
            if((r == nullptr)|| (r->dir == nullptr)){
                return r;
            }
            NodeARN *aux = r->dir;
            r->dir = aux->esq;
            if(r->dir != nullptr){
                r->dir->pai = r;
            }
            if(r->pai != nullptr){
                if(r->pai->key > r->key){
                    r->pai->esq = aux;
                }
                else{
                    r->pai->dir = aux;
                }
            }
            aux->esq =r;
            aux->pai = r->pai;
            r->pai = aux;
            if(r->dir != nullptr){
                r->dir->pai = r;
            }
            return aux;
        }
        NodeARN *rodaDir(NodeARN *r){
            if((r == nullptr)|| (r->esq == nullptr)){
                return r;
            }
            NodeARN *aux = r->esq;
            r->esq = aux->dir;
            if(r->esq != nullptr){
                r->esq->pai = r;
            }
            if(r->pai != nullptr){
                if(r->pai->key > r->key){
                    r->pai->esq = aux;
                }
                else{
                    r->pai->dir = aux;
                }
            }
            aux->dir = r;
            aux->pai = r->pai;
            r->pai = aux;
            if(r->esq != nullptr){
                r->esq->pai = r;
            }
            return aux;
        }

        NodeARN *putRN(NodeARN *raiz, int key){
            if(raiz == nullptr){
                NodeARN *inserir = new NodeARN(nullptr,key);
                return inserir;
            }
            NodeARN* p = raiz;
            bool achou = false;
            while(!achou){
                if((p->key > key) && (p->esq != nullptr)){
                    p = p->esq;
                }
                else if((p->key > key) && (p->esq == nullptr)){
                    achou = true;
                }
                else if((p->key < key) && (p->dir != nullptr)){
                    p = p->dir;
                }
                else if((p->key < key) && (p->dir == nullptr)){
                    achou = true;
                }
                else{
                    achou = true;
                }
            }
            
            NodeARN *filho = new NodeARN(p,key);

            if((p->key > key) && (p->esq == nullptr)){
                p->esq = filho;
            }
            else{
                p->dir = filho;
            }

            Fix(filho);

            NodeARN* new_root = raiz;
            while(new_root->pai != nullptr){
                new_root = new_root->pai;
            }
            return new_root;
        }

        void debug_rec(NodeARN *u, int i){
            if(u->dir != nullptr){
                debug_rec(u->dir, i+3);
            }

            for(int j=0;j<i;j++){
                cout << " ";
            }
            cout << u->key << "(" << u->black_height << ")";
            if(u->preto){
                cout << "   --- Black";
            }
            else{
                cout << "   --- Red";
            }
            cout << endl;
            if(u->esq != nullptr){
                debug_rec(u->esq, i+3);
            }
        } 

    public:
        Arn() {
            refaz = false;
            raiz = nullptr;
        }

        ~Arn() {
            //delete raiz;
        }

        void add (int key, bool& deu_erro){
           raiz = putRN(raiz,key);
           check_black_height(deu_erro);
           if(deu_erro){
            imprimir();
           }
        }

        void add_root(NodeARN* node){
            raiz = node;
        }

        NodeARN* give_root(){
            return raiz;
        }

        void Fix(NodeARN* filho){
            NodeARN* p = filho->pai; 

            while(true){
                if(p->preto == true){
                    break;
                }
                NodeARN *avo = p->pai;
                if(avo==nullptr){
                    p->preto = true;
                    p->black_height = filho->black_height + 1;
                    break;
                }

                //ACHAR O TIO!
                NodeARN *tio = nullptr;//OUTRO FILHO!!
                if(avo->dir != nullptr){
                    if(avo->dir == p){
                        tio = avo->esq;
                    }
                    else{
                        tio = avo->dir;
                    }
                }

                if(tio != nullptr && tio->preto == false){ //caso tio vermelho
                    avo->preto = false;
                    p->preto = tio->preto = true;
                    filho = avo;
                    
                    tio->black_height = avo->black_height;
                    p->black_height = avo->black_height;
                    
                    p = avo->pai;
                    if(p==nullptr){ //pra que serve isso?
                        break;
                    }
                }
                else{ //caso tio preto
                    if(p==avo->esq && filho == p->esq){ //mesmo lado
                        NodeARN *q = rodaDir(avo);
                        q->preto = true;
                        avo->preto = false;

                        int aux = q->black_height;
                        q->black_height = avo->black_height;
                        avo->black_height = aux;

                        if(raiz == avo){
                            raiz = q;
                        }
                        break;
                    }
                    else if(p==avo->esq && filho == p->dir){ //lados diferentes
                        NodeARN *q = rodaEsq(p);
                        NodeARN *r = rodaDir(avo);
                        r->preto = true;
                        avo->preto = false;

                        int aux = avo->black_height;
                        avo->black_height = r->black_height;
                        r->black_height = aux;

                        if(raiz == avo){
                            raiz = r;
                        }
                        break;
                    }
                    else if(p==avo->dir && filho ==p->dir){ //mesmo lado
                        NodeARN *q = rodaEsq(avo);
                        q->preto = true;
                        avo->preto = false;

                        int aux = q->black_height;
                        q->black_height = avo->black_height;
                        avo->black_height = aux;

                        if(raiz == avo){
                            raiz = q;
                        }
                        break;
                    }
                    else{ //lados diferentes
                        NodeARN *q = rodaDir(p);
                        NodeARN *r = rodaEsq(avo);
                        r->preto = true;
                        avo->preto = false;

                        int aux = avo->black_height;
                        avo->black_height = r->black_height;
                        r->black_height = aux;

                        if(raiz == avo){
                            raiz = r;
                        }
                        break;
                    }
                }
            }
        }

        void imprimir(){
            debug_rec(raiz, 0);
            cout << "---------------" << endl << endl;
        }

        void check_black_height(bool& deu_erro){
            NodeARN* r = raiz;
            if(r == nullptr){
                return;
            }
            rec_check_black_height(r, r->black_height, deu_erro);
        }

        void rec_check_black_height(NodeARN* u, int bh, bool& deu_erro){
            if(u == nullptr){
                if(bh != 1){
                    cout << "ACHEI UMA FOLHA COM BH > 1" << endl;
                    deu_erro = true;
                }
                return;
            }
            int actual_black_height = u->black_height;
            if(actual_black_height == bh){
                if(u->preto == true){
                    actual_black_height = actual_black_height - 1;
                }
                rec_check_black_height(u->esq, actual_black_height, deu_erro);
                rec_check_black_height(u->dir, actual_black_height, deu_erro);
            }
            else{
                cout << "ACHEI UM NÓ COM BH = " << actual_black_height << ", PORÉM A BH ACIMA É " << bh << ". Esse nó tem chave " << u->key << endl;
                deu_erro = true;
            }
        }
};

NodeARN* Join(NodeARN *u, int k, NodeARN *v){
    NodeARN *novo = new NodeARN(nullptr, k);

    if(u->black_height == v->black_height){
        novo->preto = true;
        novo->black_height = u->black_height + 1;
        u->pai = novo;
        v->pai = novo;
        novo->esq = u;
        novo->dir = v;
        return novo;
    }

    NodeARN *x;
    NodeARN *final_root;

    if(u->black_height > v->black_height){
        final_root = u;
        NodeARN* u_aux = u;
        while(u_aux->black_height > v->black_height){
            u_aux = u_aux->dir;
        }

        x = u_aux->pai;
        novo->esq = u_aux;
        novo->black_height = u_aux->black_height;
        x->dir = novo;
        novo->dir = v;

        v->pai = novo->dir;
        u_aux->pai = novo;
        novo->pai = x;
    }
    else{ //u->black_height < v->black_height
        final_root = v;
        NodeARN* v_aux = v;
        while(v_aux->black_height > u->black_height){
            v_aux = v_aux->esq;
        }
        
        x = v_aux->pai;
        novo->dir = v_aux;
        novo->black_height = v_aux->black_height;
        x->esq = novo;
        novo->esq = u;
        
        u->pai = novo->esq;
        v_aux->pai = novo;
        novo->pai = x;
    }
    if(novo->pai->preto == false){
        Arn fix;
        fix.Fix(novo);
    }
    while(final_root->pai != nullptr){
        final_root = final_root->pai;
    }
    return final_root;
}

NodeARN* Split(NodeARN *root, int pivot){
    stack<NodeARN*> left_trees;
    stack<NodeARN*> right_trees;

    NodeARN* search = root;
    while(search != nullptr){
        if(search->key > pivot){ //go left
            NodeARN* right = search;
            right_trees.push(right);
            search = search->esq;
        }
        else{ //go right
            NodeARN* left = search;
            left_trees.push(left);
            search = search->dir;
        }
    }

    NodeARN* final_left_tree = left_trees.top();
    left_trees.pop();
    if(left_trees.size() > 0){
        int final_insertion = final_left_tree->key; //get last 
        final_left_tree = final_left_tree->esq;

        final_left_tree->pai->esq = nullptr;
        delete final_left_tree->pai;
        final_left_tree->pai = nullptr;
        
        while(left_trees.size() > 1){
            NodeARN* aux_tree = left_trees.top();
            left_trees.pop();

            int aux_index = aux_tree->key;

            aux_tree = aux_tree->esq;
            aux_tree->pai->esq = nullptr;
            delete aux_tree->pai;
            aux_tree->pai = nullptr;

            final_left_tree = Join(aux_tree, aux_index, final_left_tree);
        }
    }

    NodeARN* final_right_tree = right_trees.top();
    right_trees.pop();
    
    int next_add, current_add;
    
    if(right_trees.size() > 0){
        current_add = final_right_tree->key;
        final_right_tree = final_right_tree->dir;
        
        final_right_tree->pai->dir = nullptr;
        delete final_right_tree->pai;
        final_right_tree->pai = nullptr;
        
        while(right_trees.size() > 0){
            NodeARN* aux_tree = right_trees.top();
            right_trees.pop();

            next_add = aux_tree->key;
            aux_tree = aux_tree->dir;   
            aux_tree->pai->dir = nullptr;

            delete aux_tree->pai;
            aux_tree->pai = nullptr;

            final_right_tree = Join(aux_tree, current_add, final_right_tree);
            current_add = next_add;
        }

        final_right_tree = Join(nullptr, current_add, final_right_tree);
    }

    return nullptr;
}

/* int main(){
    unsigned int seed = 30;
    cout << "SEED = " << seed << endl;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(1, 400);
    vector<int> numeros_adicionados;
    Arn rn;


    for(int i = 1; i < 10; i++){
        int numero_sorteado = distrib(gen);
        while(std::find(numeros_adicionados.begin(), numeros_adicionados.end(), numero_sorteado) != numeros_adicionados.end()){
            numero_sorteado = distrib(gen);
        }
        cout << "ADICIONANDO NÚMERO  " << numero_sorteado << endl;
        rn.add(numero_sorteado);
        numeros_adicionados.push_back(numero_sorteado);
    }
} */

int main(){
    bool deu_erro = false;
    for(unsigned int seed = 0; seed < 0; seed++){
        cout << "SEED = " << seed << endl;
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> distrib(1, 100);
        std::uniform_int_distribution<> distrib2(201, 300);
        vector<int> numeros_adicionados;
        Arn rn;
        Arn rn2;

        for(int i = 1; i < 100; i++){
            int numero_sorteado1 = distrib(gen);
            int numero_sorteado2 = distrib2(gen);
            while(std::find(numeros_adicionados.begin(), numeros_adicionados.end(), numero_sorteado1) != numeros_adicionados.end()){
                numero_sorteado1 = distrib(gen);
            }
            while(std::find(numeros_adicionados.begin(), numeros_adicionados.end(), numero_sorteado2) != numeros_adicionados.end()){
                numero_sorteado2 = distrib(gen);
            }
            //cout << "ADICIONANDO NÚMERO  " << numero_sorteado << endl;
            if(i < 90){
                rn.add(numero_sorteado1, deu_erro);
                numeros_adicionados.push_back(numero_sorteado1);
            }
            else{
                rn2.add(numero_sorteado2, deu_erro);
                numeros_adicionados.push_back(numero_sorteado2);
            }
            //rn.imprimir();
            if(deu_erro){
                cout << "DEU MERDA na seed " << seed << "! :(" << endl;
                break;
            }
        }

        cout << "TESTE DO JOIN!" << endl;
        rn.imprimir();
        rn2.imprimir();

        int middle = 150;
        numeros_adicionados.push_back(middle);

        Arn rn3;
        NodeARN* teste = Join(rn.give_root(), middle, rn2.give_root());
        rn3.add_root(teste);
        rn3.imprimir();
        rn3.check_black_height(deu_erro);


        /* for(int i = 1; i < 20000; i++){
            int numero_sorteado = distrib(gen);
            while(std::find(numeros_adicionados.begin(), numeros_adicionados.end(), numero_sorteado) != numeros_adicionados.end()){
                numero_sorteado = distrib(gen);
            }
            //cout << "ADICIONANDO NÚMERO  " << numero_sorteado << endl;
            rn.add(numero_sorteado, deu_erro);
            //rn.imprimir();
            numeros_adicionados.push_back(numero_sorteado);
            if(deu_erro){
                cout << "DEU MERDA na seed " << seed << "! :(" << endl;
                break;
            }
        }*/

        if(!deu_erro){
            sort(numeros_adicionados.begin(),numeros_adicionados.end());
            for(int i = 0; i < numeros_adicionados.size(); i++){
                cout << numeros_adicionados[i] << " ";
            }
            cout << endl;
        }
    }
    if(!deu_erro){
        cout << "DEU CERTO!" << endl;
    }
}