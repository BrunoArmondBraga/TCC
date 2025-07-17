#ifndef ARN_H
#define ARN_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <stack>

using namespace std;

class NodeARN{
    private:
        
    public:
        int key;
        bool preto;
        int black_height = 1;
        bool is_root = false;
        int depth = 0;
        int min_depth = 0;
        int max_depth = 0;
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
        NodeARN(int key, int depth = 0){
            preto = false;
            black_height = 1;
            esq = nullptr;
            dir = nullptr;
            pai = nullptr;
            this->key = key;
            this->depth = depth;
        }
        NodeARN(NodeARN* pai,int key,int depth){
            preto = false;
            esq = nullptr;
            dir = nullptr;
            this->depth = depth;
            if(pai == nullptr){
                this->pai = nullptr;
            }
            else{
                this->pai = pai;
            }
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

struct split_data
{
    NodeARN* left_tree;
    NodeARN* right_tree;
};

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

bool search(NodeARN* root, int key){
    if(root == nullptr){
        return false;
    }
    if(root->key == key){
        return true;
    }
    if(root->key > key){ //go left
        return search(root->esq, key);
    }
    return search(root->dir, key); //go right
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

                /* if(raiz == avo){
                    raiz = q;
                } */
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

                /* if(raiz == avo){
                    raiz = r;
                } */
                break;
            }
            else if(p==avo->dir && filho ==p->dir){ //mesmo lado
                NodeARN *q = rodaEsq(avo);
                q->preto = true;
                avo->preto = false;

                int aux = q->black_height;
                q->black_height = avo->black_height;
                avo->black_height = aux;

                /* if(raiz == avo){
                    raiz = q;
                } */
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

                /* if(raiz == avo){
                    raiz = r;
                } */
                break;
            }
        }
    }
}

NodeARN *putRN(NodeARN *raiz, int key, int depth = 0){
    if(raiz == nullptr){
        NodeARN *inserir = new NodeARN(nullptr,key,depth);
        inserir->preto = true;
        inserir->black_height = 2;
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
    
    NodeARN *filho = new NodeARN(p,key,depth);

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

void debug_rec(NodeARN *u, int i = 0){
    if(u == nullptr){
        return;
    }
    if(u->dir != nullptr){
        debug_rec(u->dir, i+3);
    }

    for(int j=0;j<i;j++){
        cout << " ";
    }
    if(u->is_root){
        cout << " ->";
    }
    else{
        cout << "   ";
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

void debug_rec_with_parts(NodeARN *u, int i, stack<NodeARN*>& parts){
    if(u == nullptr){
        return;
    }
    if(u->dir != nullptr){
        if(u->dir->is_root){
            parts.push(u->dir);
        }
        else{   
            debug_rec_with_parts(u->dir, i+3, parts);
        }
    }

    for(int j=0;j<i;j++){
        cout << " ";
    }
    if(u->is_root){
        cout << " ->";
    }
    else{
        cout << "   ";
    }
    cout << u->key << "(" << u->black_height << "/" << u->depth << ")";
    cout << "  [" << u->min_depth << " " << u->max_depth << "]";
    if(u->preto){
        cout << "   --- Black";
    }
    else{
        cout << "   --- Red";
    }
    cout << endl;
    if(u->esq != nullptr){
        if(u->esq->is_root){
            parts.push(u->esq);
        }
        else{
            debug_rec_with_parts(u->esq, i+3, parts);
        }
    }
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

void check_black_height(NodeARN* raiz, bool& deu_erro){
    NodeARN* r = raiz;
    if(r == nullptr){
        return;
    }
    rec_check_black_height(r, r->black_height, deu_erro);
}

int size_rec(NodeARN* raiz){
    if(raiz == nullptr){
        return 0;
    }
    int e = size_rec(raiz->esq);
    int d = size_rec(raiz->dir); 
    return e + d + 1;
}

int size(NodeARN* raiz){
    return size_rec(raiz);
}

void imprimir(NodeARN* raiz){
    cout << "tamanho da árvore igual a " << size(raiz) << endl;
    debug_rec(raiz);
    cout << "---------------" << endl << endl;
}

void imprimir_em_partes(NodeARN* raiz){
    if(raiz == nullptr){
        return;
    }
    cout << "print individual das árvores" << endl;
    int contador = 1;
    stack<NodeARN*> parts;
    parts.push(raiz);
    while(!parts.empty()){
        cout << "printando árvores de número" << contador << endl;
        NodeARN* debug = parts.top();
        parts.pop();
        debug_rec_with_parts(debug, 0, parts);
        cout << "-------------" << endl;
        contador += 1;
    }
}

NodeARN* add(NodeARN* raiz, int key, bool& deu_erro, int depth = 0){
    raiz = putRN(raiz, key, depth);
    check_black_height(raiz, deu_erro);
    if(deu_erro){
        imprimir(raiz);
    }
    return raiz;
}

NodeARN* Join(NodeARN *u, int k, NodeARN *v){
    if(u == nullptr){
        bool to_be_destroyed = false;            // DELETE THIS IN THE FUTURE!
        return add(v, k, to_be_destroyed);
    }
    if(v == nullptr){
        bool to_be_destroyed = false;            // DELETE THIS IN THE FUTURE!
        return add(u, k, to_be_destroyed);
    }
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

        v->pai = novo;
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
        
        u->pai = novo;
        v_aux->pai = novo;
        novo->pai = x;
    }
    if(novo->pai->preto == false){
        /* Arn fix;
        fix.Fix(novo); */
        Fix(novo);
    }
    while(final_root->pai != nullptr){
        final_root = final_root->pai;
    }
    return final_root;
}

split_data Split(NodeARN *root, int pivot){
    stack<NodeARN*> left_trees;
    stack<NodeARN*> right_trees;

    NodeARN* search = root;
    while(search != nullptr){
        if(search->key > pivot){ //go left
            NodeARN* right = search;
            search = search->esq;
            
            if(right->esq != nullptr){
                right->esq->pai = nullptr;
                right->esq = nullptr;
            }
            right_trees.push(right);

            if(right->dir != nullptr && right->dir->preto == false){
                right->dir->preto = true;
                right->dir->black_height += 1; 
            }
        }
        else{ //go right
            NodeARN* left = search;
            search = search->dir;

            if(left->dir != nullptr){
                left->dir->pai = nullptr;
                left->dir = nullptr;
            }
            left_trees.push(left);

            if(left->esq != nullptr && left->esq->preto == false){
                left->esq->preto = true;
                left->esq->black_height += 1;
            }
        }
    }

    NodeARN* final_left_tree = nullptr;
    if(left_trees.size() > 0){
        final_left_tree = left_trees.top();
        left_trees.pop();

        int final_insertion = final_left_tree->key; //get last 

        if(final_left_tree->esq == nullptr){
            delete final_left_tree;
            final_left_tree = nullptr;
        }
        else{       
            final_left_tree = final_left_tree->esq;     
            final_left_tree->pai->esq = nullptr;
            delete final_left_tree->pai;
            final_left_tree->pai = nullptr;
        }
        
        
        while(left_trees.size() > 0){
            NodeARN* aux_tree = left_trees.top();
            left_trees.pop();

            int aux_index = aux_tree->key;

            if(aux_tree->esq != nullptr){

                aux_tree = aux_tree->esq;
                aux_tree->pai->esq = nullptr;
                delete aux_tree->pai;
                aux_tree->pai = nullptr;
            }
            else{
                delete aux_tree;
                aux_tree = nullptr;
            }

            final_left_tree = Join(aux_tree, aux_index, final_left_tree);
        }

        final_left_tree = Join(final_left_tree, final_insertion, nullptr);
    }

    NodeARN* final_right_tree = nullptr;
    if(right_trees.size() > 0){
        final_right_tree = right_trees.top();
        right_trees.pop();

        int final_insertion = final_right_tree->key;

        if(final_right_tree->dir == nullptr){
            delete final_right_tree;
            final_right_tree = nullptr;
        }
        else{
            final_right_tree = final_right_tree->dir;
            
            final_right_tree->pai->dir = nullptr;
            delete final_right_tree->pai;
            final_right_tree->pai = nullptr;
        }

        while(right_trees.size() > 0){
            NodeARN* aux_tree = right_trees.top();
            right_trees.pop();

            int aux_index = aux_tree->key;

            if(aux_tree->dir != nullptr){

                aux_tree = aux_tree->dir;
                aux_tree->pai->dir = nullptr;
                delete aux_tree->pai;
                aux_tree->pai = nullptr;
            }
            else{
                delete aux_tree;
                aux_tree = nullptr;
            }

            final_right_tree = Join(final_right_tree, aux_index, aux_tree);
        }
        final_right_tree = Join(nullptr, final_insertion, final_right_tree);
    }

    return {final_left_tree, final_right_tree};
}

void update_depths(NodeARN* u, int min, int max, bool root = false){
    if(u == nullptr){
        return;
    }
    if(u->is_root and (!root)){
        return;
    }
    u->min_depth = min;
    u->max_depth = max;
    update_depths(u->esq, min, max);
    update_depths(u->dir, min, max);
}

NodeARN* build_complete_BST(int n, int offset = 0, int depth = 0){
    if(n <= 0){
        return nullptr;
    }
    else if(n == 1){
        return new NodeARN(offset + 1, depth);
    }

    int x = 1;
    while(x*2 - 1 <= n){
        x = x * 2;
    }

    int left, right = 0;
    
    left = n - x + 1;
    
    if(left > x/2){
        right = left - x/2;
        left = x/2;
    }
    
    left += (x - 2)/2;
    right += (x - 2)/2;

    NodeARN* new_node = new NodeARN(left + offset + 1, depth);
    new_node->esq = build_complete_BST(left, offset, depth + 1);
    new_node->dir = build_complete_BST(right, offset + left + 1, depth + 1);
    return new_node;
}

void add_to_tango(NodeARN* root, NodeARN* new_piece){
    if(root == nullptr || new_piece == nullptr){
        return;
    }

    NodeARN* current_node = root;
    while(true){
        if(current_node->key > new_piece->key){
            if(current_node->esq == nullptr){
                current_node->esq = new_piece;
                break;
            }
            else{
                current_node = current_node->esq;
            }
        }
        else{
            if(current_node->dir == nullptr){
                current_node->dir = new_piece;
                break;
            }
            else{
                current_node = current_node->dir;
            }
        }
    }
}

NodeARN* build_tango_tree(NodeARN* u){
    if(u == nullptr){
        return nullptr;
    }
    bool dummy = false;
    stack<NodeARN*> all_nodes;
    all_nodes.push(u);

    NodeARN* tango = nullptr;

    while(!all_nodes.empty()){
        NodeARN* node_in_auxiliary_tree = all_nodes.top();
        all_nodes.pop();
        NodeARN* new_node = new NodeARN(node_in_auxiliary_tree->key,node_in_auxiliary_tree->depth);
        int min_depth = new_node->depth;
        int max_depth = new_node->depth;

        while(node_in_auxiliary_tree->esq != nullptr){
            if(node_in_auxiliary_tree->dir != nullptr){
                all_nodes.push(node_in_auxiliary_tree->dir);
            }
            new_node = add(new_node, node_in_auxiliary_tree->esq->key,dummy,node_in_auxiliary_tree->esq->depth);
            node_in_auxiliary_tree = node_in_auxiliary_tree->esq;
            if(node_in_auxiliary_tree->depth < min_depth){
                min_depth = node_in_auxiliary_tree->depth;
            }
            if(node_in_auxiliary_tree->depth > max_depth){
                max_depth = node_in_auxiliary_tree->depth;
            }
        }

        new_node->is_root = true;
        update_depths(new_node,min_depth, max_depth, true);

        if(tango == nullptr){
            tango = new_node;
        }
        else{
            add_to_tango(tango, new_node);
        }
    }
    imprimir(tango);
    return tango;
}

NodeARN* tango_function(NodeARN* u, int min_depth){
    cout << "Tango no nó com chave  " << u->key << "  , procura-se depth = " << min_depth << endl;
    return u;
}

bool search_in_tango(NodeARN* root, int searched_item){
    if(root == nullptr){
        return false;
    }

    NodeARN* last_root = root;
    NodeARN* current_node = root;

    while(current_node != nullptr){

        if(current_node->is_root and current_node != last_root){
            // it is going to return the new tree
            last_root = tango_function(last_root, current_node->min_depth);
            return true;
        }

        if(current_node->key == searched_item){
            return true;
        }
    
        if(current_node->key > searched_item){
            current_node = current_node->esq;
        }
        else{
            current_node = current_node->dir;
        }
    }
    return false;
}

#endif

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

/* int main(){
    bool deu_erro = false;
    for(unsigned int seed = 1; seed < 150; seed++){
        cout << "SEED = " << seed << endl;
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> distrib(1, 10000);
        std::uniform_int_distribution<> distrib2(20001, 30000);
        vector<int> numeros_adicionados;
        NodeARN* rn = nullptr;
        NodeARN* rn2 = nullptr;

        std::uniform_int_distribution<> escolha(1, 5000);
        int teste1 = escolha(gen);

        for(int i = 1; i < 5000; i++){
            int numero_sorteado1 = distrib(gen);
            int numero_sorteado2 = distrib2(gen);
            while(std::find(numeros_adicionados.begin(), numeros_adicionados.end(), numero_sorteado1) != numeros_adicionados.end()){
                numero_sorteado1 = distrib(gen);
            }
            while(std::find(numeros_adicionados.begin(), numeros_adicionados.end(), numero_sorteado2) != numeros_adicionados.end()){
                numero_sorteado2 = distrib2(gen);
            }
            //cout << "ADICIONANDO NÚMERO  " << numero_sorteado << endl;
            if(i < teste1){
                rn = add(rn, numero_sorteado1, deu_erro);
                numeros_adicionados.push_back(numero_sorteado1);
            }
            else{
                rn2 = add(rn2, numero_sorteado2, deu_erro);
                numeros_adicionados.push_back(numero_sorteado2);
            }
            if(deu_erro){
                cout << "DEU MERDA na seed " << seed << "! :(" << endl;
                break;
            }
        }

        cout << "TESTE DO JOIN!" << endl;
        //imprimir(rn);
        //imprimir(rn2);

        int middle = 1500;
        numeros_adicionados.push_back(middle);

        NodeARN* teste = Join(rn, middle, rn2);
        //imprimir(teste);
        check_black_height(rn2, deu_erro);

        //if(!deu_erro){
        //    sort(numeros_adicionados.begin(),numeros_adicionados.end());
        //    for(int i = 0; i < numeros_adicionados.size(); i++){
        //        cout << numeros_adicionados[i] << " ";
        //    }
        //    cout << endl;
        //}
    }
    if(!deu_erro){
        cout << "DEU CERTO!" << endl;
    }
} */

/* int main(){
    bool deu_erro = false;
    bool deu_erro_contagem = false;

    
    vector<int> entrada = {50, 41, 7, 30, 25, 27, 3, 2, 4, 5, 20, 6, 13, 8, 14, 60, 1, 17, 9, 15, 10, 12};
    //vector<int> entrada = {7, 3, 1, 2, 13, 9, 11, 18};
    
    for(int j = 1; j < entrada.size(); j++){
        NodeARN* new_tree = nullptr;

        for(int i=0; i < entrada.size(); i++){
            new_tree = add(new_tree, entrada[i], deu_erro);
        }
        
        imprimir(new_tree);
        
        
        int searched = entrada[j];
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
        
        
    if(deu_erro){
        cout << "ERRO BLACK HEIGHT" << endl;
    }
    else if(deu_erro_contagem){
        cout << "ERRO CONTAGEM" << endl;
    }
    else{
        cout << "TUDO OK!" << endl;
    }
} */