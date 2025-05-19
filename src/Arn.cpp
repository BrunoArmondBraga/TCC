#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

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

        void Fix(NodeARN* filho, bool is_left_child){
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
            return raiz;
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
                cout << "ACHEI UM NÓ COM BH = " << actual_black_height << ", PORÉM A BH ACIMA É " << u->black_height << ". Esse nó tem chave " << u->key << endl;
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
        return novo;
    }

    NodeARN *x;

    if(u->black_height > v->black_height){
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
    }
    if(u->black_height < v->black_height){
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
    }
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
    for(unsigned int seed = 1; seed < 1500; seed++){
        cout << "SEED = " << seed << endl;
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> distrib(1, 100000);
        vector<int> numeros_adicionados;

        Arn rn;

        //vector<int> oi = {1,3,2};

        /* for(int i = 0; i < oi.size(); i++){
            rn.add(oi[i]);
            rn.imprimir();
        } */

        for(int i = 1; i < 200; i++){
            int numero_sorteado = distrib(gen);
            while(std::find(numeros_adicionados.begin(), numeros_adicionados.end(), numero_sorteado) != numeros_adicionados.end()){
                numero_sorteado = distrib(gen);
            }
            //cout << "ADICIONANDO NÚMERO  " << numero_sorteado << endl;
            rn.add(numero_sorteado, deu_erro);
            numeros_adicionados.push_back(numero_sorteado);
            if(deu_erro){
                cout << "DEU MERDA na seed " << seed << "! :(" << endl;
                break;
            }
        }
    }
    if(!deu_erro){
        cout << "DEU CERTO!" << endl;
    }
}