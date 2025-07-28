#ifndef ARN_H
#define ARN_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <stack>
#include <climits>

using namespace std;

class NodeARN{
    private:
        
    public:
        int key;
        bool preto;
        int black_height = 1;
        bool is_root = false;
        int is_double_black = 0; //0 = false, 1 = true, 2 = to be deleted/null double black
        int depth = 0;
        int min_depth = INT_MAX;
        int max_depth = INT_MIN;
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
            min_depth = depth;
            max_depth = depth;
        }
        NodeARN(NodeARN* pai,int key,int depth){
            preto = false;
            esq = nullptr;
            dir = nullptr;
            this->depth = depth;
            this->max_depth = depth;
            this->min_depth = depth;
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

/* void delete_with_caution(NodeARN* u){
    if(u == nullptr){
        return;
    }
    
    bool has_right_child = false;
    bool has_left_child = false;
    bool is_right_child = true;
    bool is_root = false;

    if(u->pai == nullptr){
        cout << "IM NOT SURE HOW TO HANDLE THIS DELETION!" << endl;
        cout << "deletion of node with key " << u->key << endl;
        is_root = true;
    }
    else{
        if(u->pai->esq == u){
            is_right_child = false;
        }
    }
    if(u->esq != nullptr){
        has_left_child = true;
    }
    if(u->dir != nullptr){
        has_right_child = true;
    }


    if()

    
    
} */

struct split_data
{
    NodeARN* left_tree;
    NodeARN* right_tree;
};

int max(int a, int b, int c){
    if(a >= b && a >= c){
        return a;
    }
    if(b >= a && b >= c){
        return b;
    }
    return c;
}

int min(int a, int b, int c){
    if(a <= b && a <= c){
        return a;
    }
    if(b <= a && b <= c){
        return b;
    }
    return c;
}

void fix_depths(NodeARN* u){
    if(u == nullptr){
        return;
    }
    int left_min_depth, left_max_depth, right_min_depth, right_max_depth;
    left_min_depth = left_max_depth = right_min_depth = right_max_depth = u->depth;

    if(u->esq != nullptr && (!u->esq->is_root)){
        left_min_depth = u->esq->min_depth;
        left_max_depth = u->esq->max_depth;
    }
    if(u->dir != nullptr && (!u->dir->is_root)){
        right_min_depth = u->dir->min_depth;
        right_max_depth = u->dir->max_depth;
    }

    u->min_depth = min(left_min_depth, u->depth, right_min_depth);
    u->max_depth = max(left_max_depth, u->depth, right_max_depth);
}

void reset_depths(NodeARN* u){
    if(u == nullptr){
        return;
    }
    u->min_depth = u->depth;
    u->max_depth = u->depth;
}

NodeARN* reset_node(NodeARN* u){
    if(u == nullptr){
        return nullptr;
    }
    if(u->esq != nullptr){
        u->esq->pai = nullptr;
        u->esq = nullptr;
    }
    if(u->dir != nullptr){
        u->dir->pai = nullptr;
        u->dir = nullptr;
    }
    if(u->pai != nullptr){
        if(u->pai->esq != nullptr && u->pai->esq == u){
            u->pai->esq = nullptr;
        }
        else{
            u->pai->dir = nullptr;
        }
        u->pai = nullptr;
    }
    u->is_double_black = false;
    u->is_root = false;
    u->max_depth = u->depth;
    u->min_depth = u->depth;
    u->pai = nullptr;
    u->black_height = 1;
    u->preto = false;
    return u;
}

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
    if(r->is_root){
        r->is_root = false;
        aux->is_root = true;
    }
    fix_depths(r);
    fix_depths(aux);
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
    if(r->is_root){
        r->is_root = false;
        aux->is_root = true;
    }
    fix_depths(r);
    fix_depths(aux);
    return aux;
}

int predecessor(NodeARN* root, int index){
    int pred = -1;
    NodeARN* current_node = root;

    while(current_node != nullptr && (!current_node->is_root)){
        if(current_node->key < index){ //go right
            if(current_node->key > pred){
                pred = current_node->key;
            }
            current_node = current_node->dir;
        }
        else{
            current_node = current_node->esq;
        }
    }
    return pred;
}

int sucessor(NodeARN* root, int index){
    int suc = -1;
    NodeARN* current_node = root;

    while(current_node != nullptr && (!current_node->is_root)){
        if(current_node->key > index){ //go left
            if(current_node->key < suc || suc == -1){
                suc = current_node->key;
            }
            current_node = current_node->esq;
        }
        else{
            current_node = current_node->dir;
        }
    }
    return suc;
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
            if(p == nullptr){
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

NodeARN *putRN(NodeARN *raiz, int key, NodeARN* to_be_positioned, int depth = 0){
    if(raiz == nullptr){
        if(to_be_positioned != nullptr){
            NodeARN* insert = to_be_positioned;
            insert->preto = true;
            insert->black_height = 2;
            insert->is_root = true;
        }
        NodeARN *insert = new NodeARN(nullptr,key,depth);
        insert->preto = true;
        insert->black_height = 2;
        insert->is_root = true;
        return insert;
    }

    NodeARN* upper_nodes = raiz->pai;
    if(raiz->pai != nullptr){
        raiz->pai = nullptr;
    }

    int actual_depth = depth;
    if(to_be_positioned != nullptr){
        actual_depth = to_be_positioned->depth;
    }

    NodeARN* p = raiz;
    bool found = false;

    while(!found){
        if((p->key > key) && ((p->esq == nullptr) || (p->esq->is_root == true))){
            found = true;
        }
        else if((p->key < key) && ((p->dir == nullptr)  || (p->dir->is_root == true))){
            found = true;
        }
        else if((p->key > key)){
            p = p->esq;
        }
        else if((p->key < key)){
            p = p->dir;
        }
        else{
            cout << "NÃO É PRA EXISTIR" << endl;
            found = true;
        }
        if(p->min_depth > actual_depth){
            p->min_depth = actual_depth;
        }
        if(p->max_depth < actual_depth){
            p->max_depth = actual_depth;
        }
    }

    NodeARN *filho;
    if(to_be_positioned == nullptr){
        filho = new NodeARN(p,key,depth);
    }
    else{
        filho = to_be_positioned;
    }

    /* if((p->key > key) && (p->esq == nullptr)){
        p->esq = filho;
    }
    else{
        p->dir = filho;
    } */

    if(p->key > key){
        if(p->esq != nullptr){
            NodeARN* other_root = p->esq;
            if(other_root->key < filho->key){
                filho->esq = other_root;
                other_root->pai = filho;
            }
            else{
                filho->dir = other_root;
                other_root->pai = filho;
            }
        }
        p->esq = filho;
    }
    else{
        if(p->dir != nullptr){
            NodeARN* other_root = p->dir;
            if(other_root->key < filho->key){
                filho->esq = other_root;
                other_root->pai = filho;
            }
            else{
                filho->dir = other_root;
                other_root->pai = filho;
            }
        }
        p->dir = filho;
    }
    filho->pai = p;

    if(p->preto){
        filho->black_height = p->black_height - 1; //new_node is red, father must be black
    }

    Fix(filho);

    NodeARN* new_root = raiz;
    while(new_root->pai != nullptr){
        new_root = new_root->pai;
    }
    if(upper_nodes != nullptr){
        new_root->pai = upper_nodes;
    }
    return new_root;
}

void fixDoubleBlack(NodeARN* root){
    if(root->pai == nullptr){
        root->is_double_black = 0;
        return;
    }

    bool root_is_right_child = false;
    if(root->pai->dir == root){
        root_is_right_child = true;
    }

    NodeARN* sibling = nullptr;
    if(root_is_right_child){
        sibling = root->pai->esq;
    }
    else{
        sibling = root->pai->dir;
    }

    
    bool siblings_right_son_is_black = true;
    bool siblings_left_son_is_black = true;
    if(sibling->dir != nullptr && sibling->dir->preto == false){
        siblings_right_son_is_black = false;
    }
    if(sibling->esq != nullptr && sibling->esq->preto == false){
        siblings_left_son_is_black = false;
    }

    if(sibling->preto == false){ //case s is red
        if(root_is_right_child){
            NodeARN* p = rodaDir(root->pai);
        }
        else{
            NodeARN* p = rodaEsq(root->pai);
        }
        fixDoubleBlack(root);
        return;
    }


    if((!siblings_left_son_is_black) or (!siblings_right_son_is_black)){ //case s is black with at least 1 son is red 
        if(root_is_right_child){
            if(!siblings_left_son_is_black){
                NodeARN* p = rodaDir(root->pai);
                p->esq->preto = true;
            }
            else{
                NodeARN* p = rodaEsq(sibling);
                NodeARN* q = rodaDir(p->pai);
                q->preto = true;
            }
        }
        else{
            if(!siblings_right_son_is_black){
                NodeARN* p = rodaEsq(root->pai);
                p->dir->preto = true;
            }
            else{
                NodeARN* p = rodaDir(sibling);
                NodeARN* q = rodaEsq(p->pai);
                q->preto = true;
            }

        }
        if(root->is_double_black == 2){
            if(root->pai->esq == root){
                root->pai->esq = nullptr;
            }
            else{
                root->pai->dir = nullptr;
            }
            delete root;
        }
        return;
    }
    else{ //case s is black with only black children
        if(sibling->preto == false){
            sibling->preto = true;
            root->pai->preto = false;
            if(root->is_double_black){
                delete root;
            }
        }
        else{

        }
    }
    


}

void deleteRN(NodeARN* root){
    if(root->dir != nullptr && root->esq != nullptr){
        //NodeARN* suc = sucessor(root);
        //root->depth = suc->depth; //ESTAMOS PERDENDO INFORMAÇÃO DA DEPTH!!
        //root->key = suc->key;
        //suc->key = root->key;
        //deleteRN(suc);
    }

    bool is_leaf = false;
    bool has_right_child = false;
    bool is_right_child = false;

    if(root->esq == nullptr && root->dir == nullptr){
        is_leaf = true;
    }
    else if(root->dir != nullptr){
        has_right_child = true;
    }

    if(!is_leaf){
        if(root->pai != nullptr && root->pai->dir == root){
            is_right_child = true;
        }
    }

    if(root->preto == false){ //case v == red
        if(!is_leaf){
            if(is_right_child){
                if(has_right_child){
                    root->dir->preto = true;
                    if(root->pai != nullptr){
                        root->pai->dir = root->dir;
                    }
                    root->dir->pai = root->pai;
                }
                else{
                    root->esq->preto = true;
                    if(root->pai != nullptr){
                        root->pai->dir = root->esq;
                    }
                    root->esq->pai = root->pai;
                }
            }
            else{
                if(has_right_child){
                    root->dir->preto = true;
                    if(root->pai != nullptr){
                        root->pai->esq = root->dir;
                    }
                    root->dir->pai = root->pai;
                }
                else{
                    root->esq->preto = true;
                    if(root->pai != nullptr){
                        root->pai->esq = root->esq;
                    }
                    root->esq->pai = root->pai;
                }
            }
        }
        root->dir = nullptr;
        root->esq = nullptr;
        delete root;
        return;
    }

    if(!is_leaf){ //case u == red
        if(has_right_child && root->dir != nullptr && root->dir->preto == false){
            if(is_right_child){
                root->dir->preto = true;
                if(root->pai != nullptr){
                    root->pai->dir = root->dir;
                }
                root->dir->pai = root->pai;
            }
            else{
                root->dir->preto = true;
                if(root->pai != nullptr){
                    root->pai->esq = root->dir;
                }
                root->dir->pai = root->pai;
            }
        }
        else if(!has_right_child && root->esq != nullptr && root->esq->preto == false){
            if(is_right_child){
                root->esq->preto = true;
                if(root->pai != nullptr){
                    root->pai->dir = root->esq;
                }
                root->esq->pai = root->pai;
            }
            else{
                root->esq->preto = true;
                if(root->pai != nullptr){
                    root->pai->esq = root->esq;
                }
                root->esq->pai = root->pai;
            }
        }
        delete root;
        return;
    }

    if(is_leaf){
        root->is_double_black = 2;
        fixDoubleBlack(root);
        return;
    }

    NodeARN* child = nullptr;

    if(!has_right_child){
        child = root->esq;
        if(is_right_child){
            if(root->pai != nullptr){
                root->pai->dir = root->esq;
            }
        }
        else{
            if(root->pai != nullptr){
                root->pai->esq = root->esq;
            }
        }
        root->esq->pai = root->pai;
    }
    else{
        child = root->dir;
        if(is_right_child){
            if(root->pai != nullptr){
                root->pai->dir = root->dir;
            }
        }
        else{
            if(root->pai != nullptr){
                root->pai->esq = root->dir;
            }
        }
        root->dir->pai = root->pai;
    }
    child->is_double_black = 1;
    root->dir = nullptr;
    root->esq = nullptr;
    delete root;
    fixDoubleBlack(child);
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
    cout << "  [" << u->min_depth << " " << u->max_depth << "]";
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
    return;
    if(u == nullptr){
        if(bh != 1){
            cout << "ACHEI UMA FOLHA COM BH > 1" << endl;
            deu_erro = true;
        }
        return;
    }
    cout << "olhando o nó " << u->key << endl;
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

void imprimir_em_partes(NodeARN* raiz, bool all = true){
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
        if(!all){
            break;
        }
    }
}

NodeARN* add(NodeARN* raiz, int key, NodeARN* to_be_positioned, int depth = 0){
    bool first_node = false;
    if(raiz == nullptr){
        first_node = true;
    }
    raiz = putRN(raiz, key, to_be_positioned, depth);
    bool deu_erro = false;
    check_black_height(raiz, deu_erro);
    if(deu_erro){
        cout << "deu erro na adição da chave " << key;
        //imprimir(raiz);
    }
    if(raiz != nullptr){
        if(!raiz->preto){
            raiz->black_height++;
            raiz->preto = true;
        }
    }
    if(first_node){
        raiz->is_root = true;
    }
    return raiz;
}

NodeARN* remove(NodeARN* raiz, int key){
    if(raiz == nullptr){
        return nullptr;
    }
    NodeARN* old_root = raiz;
    if(raiz->key == key){
        if(raiz->esq != nullptr){
            old_root = old_root->dir;
        }
        else if(raiz->dir != nullptr){
            old_root = old_root->esq;
        }
        else{
            delete raiz;
            return nullptr;
        }
    }
    NodeARN* current_node = raiz;
    while(current_node != nullptr){
        if(current_node->key == key){
            deleteRN(current_node);
            while(old_root->pai != nullptr){
                old_root = old_root->pai;
            }
            break;
        }
        if(current_node->key > key){
            current_node = current_node->esq;
        }
        else{
            current_node = current_node->dir;
        }
    }
    return old_root;
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

NodeARN* Join(NodeARN *u, NodeARN* k, NodeARN *v){
    fix_depths(k);
    if(u == nullptr && v == nullptr){
        return k;
    }
    else{
        //reseta o k
        k->black_height = 1;
        k->preto = false;
    }
    if(u == nullptr || u->is_root){
        if(u != nullptr){
            k->esq = u;
        }
        NodeARN* to_be_returned = add(v, k->key, k, k->depth);
        /* if(u != nullptr){
            cout << "ADD TO TANGO MAL USADO!" << endl;
            add_to_tango(to_be_returned, u);
        } */
        return to_be_returned;
    }
    if(v == nullptr || v->is_root){
        if(v != nullptr){
            k->dir = v;
        }
        NodeARN* to_be_returned = add(u, k->key, k, k->depth);
        /* if(v != nullptr){
            cout << "ADD TO TANGO MAL USADO!" << endl;
            add_to_tango(to_be_returned, v);
        } */
        return to_be_returned;
    }
    NodeARN *novo = k;
    novo->preto = false;

    if(u->black_height == v->black_height){
        novo->preto = true;
        novo->black_height = u->black_height + 1;
        u->pai = novo;
        v->pai = novo;
        novo->esq = u;
        novo->dir = v;
        novo->esq->is_root = false;
        novo->dir->is_root = false;
        /* novo->is_root = true; */
        fix_depths(novo);
        return novo;
    }

    NodeARN *x;
    NodeARN *final_root;
    /* v->is_root = false;
    u->is_root = false; */

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
    fix_depths(novo);
    if(novo->pai->preto == false){
        Fix(novo);
    }
    while(final_root->pai != nullptr){
        final_root = final_root->pai;
    }
    //final_root->is_root = true;
    return final_root;
}

split_data Split(NodeARN *root, float pivot){
    stack<NodeARN*> left_trees;
    stack<NodeARN*> right_trees;

    NodeARN* search = root;
    search->is_root = false;

    bool is_there_remaning_node = false;
    NodeARN* remaining_node = nullptr;

    while(search != nullptr){
        if(search->key > pivot){ //go left
            NodeARN* right = search;
            search = search->esq;
            right_trees.push(right);
            
            
            if(right->dir != nullptr && right->dir->preto == false && (!right->dir->is_root)){
                right->dir->preto = true;
                right->dir->black_height += 1; 
            }
            if(right->esq != nullptr){
                if(right->esq->is_root){ //got to a node that is a new root
                    if(left_trees.size() != 0 && right->esq->key < pivot){ //nodes in between need to be moved
                        remaining_node = right->esq;
                        is_there_remaning_node = true;
                        right->esq->pai = nullptr;
                        right->esq = nullptr;
                    }
                    break;
                }
                right->esq->pai = nullptr;
                right->esq = nullptr;
            }
            else{
                break;
            }
        }
        else{ //go right
            NodeARN* left = search;
            search = search->dir;
            left_trees.push(left);

            if(left->esq != nullptr && left->esq->preto == false){
                left->esq->preto = true;
                left->esq->black_height += 1;
            }
            if(left->dir != nullptr){
                if(left->dir->is_root){
                    if(right_trees.size() != 0 && left->dir->key > pivot){ //put the remaining nodes in the other side
                        remaining_node = left->dir;
                        is_there_remaning_node = true;
                        left->dir->pai = nullptr;
                        left->dir = nullptr;
                    }
                    break;
                }
                left->dir->pai = nullptr;
                left->dir = nullptr;
            }
            else{
                break;
            }
        }
    }

    NodeARN* final_left_tree = nullptr;
    if(left_trees.size() > 0){
        final_left_tree = left_trees.top();
        left_trees.pop();

        NodeARN* final_insertion = final_left_tree; //get last
        if(is_there_remaning_node && remaining_node->key < pivot){
            final_insertion->dir = remaining_node;
            remaining_node->pai = final_insertion;
            is_there_remaning_node = false;
        }
        fix_depths(final_insertion);

        //isso faz sentido?
        if(final_left_tree->esq != nullptr && (!final_left_tree->esq->is_root)){
            final_left_tree = final_left_tree->esq;
            if(final_left_tree->pai != nullptr){
                final_left_tree->pai->esq = nullptr;
            }
            final_left_tree->pai = nullptr;
        }
        else{
            final_left_tree = nullptr;
        }        
        
        while(left_trees.size() > 0){
            NodeARN* aux_tree = left_trees.top();
            left_trees.pop();

            NodeARN* insertion_node = aux_tree;
            fix_depths(insertion_node);

            if(aux_tree->esq != nullptr){
                aux_tree = aux_tree->esq;
                if(aux_tree->pai != nullptr){
                    aux_tree->pai->esq = nullptr;
                }
                aux_tree->pai = nullptr;
            }
            else{
                aux_tree = nullptr;
            }

            if(aux_tree != nullptr && aux_tree->is_root){
                insertion_node->esq = aux_tree;
                aux_tree->pai = insertion_node;
                aux_tree = nullptr;
            }

            final_left_tree = Join(aux_tree, insertion_node, final_left_tree);
        }

        final_left_tree = Join(final_left_tree, final_insertion, nullptr);
    }

    NodeARN* final_right_tree = nullptr;
    if(right_trees.size() > 0){
        final_right_tree = right_trees.top();
        right_trees.pop();

        NodeARN* final_insertion = final_right_tree;
        if(is_there_remaning_node && remaining_node->key > pivot){
            final_insertion->esq = remaining_node;
            remaining_node->pai = final_insertion;
            is_there_remaning_node = false;
        }
        fix_depths(final_insertion);

        if(final_right_tree->dir != nullptr && (!final_right_tree->dir->is_root)){
            final_right_tree = final_right_tree->dir;
            if(final_right_tree->pai != nullptr){
                final_right_tree->pai->dir = nullptr;
            }
            final_right_tree->pai = nullptr;
        }
        else{
            final_right_tree = nullptr;
        }
        
        while(right_trees.size() > 0){
            NodeARN* aux_tree = right_trees.top();
            right_trees.pop();

            NodeARN* insertion_node = aux_tree;
            fix_depths(insertion_node);

            if(aux_tree->dir != nullptr){
                aux_tree = aux_tree->dir;
                if(aux_tree->pai != nullptr){
                    aux_tree->pai->dir = nullptr;
                }
                aux_tree->pai = nullptr;
            }
            else{
                aux_tree = nullptr;
            }

            if(aux_tree != nullptr && aux_tree->is_root){
                insertion_node->dir = aux_tree;
                aux_tree->pai = insertion_node;
                aux_tree = nullptr;
            }

            final_right_tree = Join(final_right_tree, insertion_node, aux_tree);
        }
        final_right_tree = Join(nullptr, final_insertion, final_right_tree);
    }
    /* if(final_left_tree != nullptr){
        final_left_tree->is_root = true;
    }
    if(final_right_tree != nullptr){
        final_right_tree->is_root = true;
    } */

    //cases when one of the trees is made of only one node and needs to be black
    if(final_left_tree != nullptr && !final_left_tree->preto){
        final_left_tree->preto = true;
        final_left_tree->black_height = 2;
    }
    if(final_right_tree != nullptr && !final_right_tree){
        final_left_tree->preto = true;
        final_right_tree->black_height = 2;
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

NodeARN* build_tango_tree(NodeARN* u){
    if(u == nullptr){
        return nullptr;
    }
    stack<NodeARN*> all_nodes;
    all_nodes.push(u);

    NodeARN* tango = nullptr;

    while(!all_nodes.empty()){
        NodeARN* node_in_auxiliary_tree = all_nodes.top();
        all_nodes.pop();
        NodeARN* new_node = add(nullptr, node_in_auxiliary_tree->key, nullptr, node_in_auxiliary_tree->depth);
        //new NodeARN(node_in_auxiliary_tree->key,node_in_auxiliary_tree->depth);
        int min_depth = new_node->depth;
        int max_depth = new_node->depth;

        while(node_in_auxiliary_tree->esq != nullptr){
            if(node_in_auxiliary_tree->dir != nullptr){
                all_nodes.push(node_in_auxiliary_tree->dir);
            }
            new_node = add(new_node, node_in_auxiliary_tree->esq->key,nullptr,node_in_auxiliary_tree->esq->depth);
            node_in_auxiliary_tree = node_in_auxiliary_tree->esq;
            if(node_in_auxiliary_tree->depth < min_depth){
                min_depth = node_in_auxiliary_tree->depth;
            }
            if(node_in_auxiliary_tree->depth > max_depth){
                max_depth = node_in_auxiliary_tree->depth;
            }
        }

        new_node->is_root = true;
        //update_depths(new_node,min_depth, max_depth, true);

        if(tango == nullptr){
            tango = new_node;
        }
        else{
            add_to_tango(tango, new_node);
        }
    }
    return tango;
}

NodeARN* bring_to_front(NodeARN* root, int item){
    if(root == nullptr){
        return nullptr;
    }
    root->is_root = false;
    //imprimir(root);
    split_data first_split = Split(root, item - 0.5);
    //imprimir(first_split.left_tree);
    //imprimir(first_split.right_tree);
    split_data second_split = Split(first_split.right_tree, item + 0.5);
    //imprimir(second_split.left_tree);
    //imprimir(second_split.right_tree);

    NodeARN* first_fragment = first_split.left_tree;
    NodeARN* single_node = second_split.left_tree;
    NodeARN* second_fragment = second_split.right_tree;

    /* cout << "Primeiro------------------------------" << endl;
    imprimir(first_fragment);
    cout << "Segundo-------------------------------" << endl;
    imprimir(second_fragment);
    cout << "single node---------------------------" << endl;
    imprimir(single_node); */

    if(single_node->esq != nullptr || single_node->dir != nullptr){
        cout << "TA CERTO ISSO?============================================================================" << endl;
    }

    /* if(single_node->esq != nullptr){
        single_node->esq->pai = nullptr;
        add_to_tango(first_fragment, single_node->esq);
    }
    if(single_node->dir != nullptr){
        single_node->dir->pai = nullptr;
        add_to_tango(second_fragment, single_node->dir);
    } */
    if(first_fragment != nullptr){
        single_node->esq = first_fragment;
    }
    if(second_fragment != nullptr){
        single_node->dir = second_fragment;
    }

    if(first_fragment != nullptr){
        /* first_fragment->is_root = false; */
        first_fragment->pai = single_node;
    }
    if(second_fragment != nullptr){
        /* second_fragment->is_root = false; */
        second_fragment->pai = single_node;
    }

    single_node->preto = true;
    /* single_node->is_root = true; */
    
    if(first_fragment != nullptr){
        single_node->black_height = first_fragment->black_height + 1;
    }
    else if(second_fragment != nullptr){
        single_node->black_height = second_fragment->black_height + 1;
    }
    else{
        single_node->black_height = 2;
    }

    //corrigir min/max depth!
    
    return single_node;
}

NodeARN* concatenate(NodeARN* root){
    if(root == nullptr){
        return nullptr;
    }
    root->is_root = false;
    
    NodeARN* first_fragment = nullptr;
    NodeARN* second_fragment = nullptr;
    NodeARN* single_node = root;

    if(single_node->esq != nullptr && (!single_node->esq->is_root)){
        first_fragment = single_node->esq;
        single_node->esq = nullptr;
        first_fragment->pai = nullptr;
    }
    if(single_node->dir != nullptr && (!single_node->dir->is_root)){
        second_fragment = single_node->dir;
        single_node->dir = nullptr;
        second_fragment->pai = nullptr;
    }
    NodeARN* final = Join(first_fragment, single_node, second_fragment);
    //final->is_root = true;
    return final;
}

NodeARN* cut(NodeARN* u, int min_depth){
    NodeARN* current_node = u;
    u->is_root = false;
    
    int l, r;
    l = r = -1;
    
    while(current_node != nullptr){ //find l
        if(current_node->is_root){
            break;
        }
        if(current_node->depth > min_depth && (current_node->key < l || l == -1)){
            l = current_node->key;
        }
        if(current_node->esq != nullptr && current_node->esq->max_depth > min_depth && (!current_node->esq->is_root)){
            current_node = current_node->esq;
        }
        else{
            current_node = current_node->dir;
        }
    }

    current_node = u; //reset current_node
    
    while(current_node != nullptr){ //find r
        if(current_node->is_root){
            break;
        }
        if(current_node->depth > min_depth && current_node->key > r){
            r = current_node->key;
        }
        if(current_node->dir != nullptr && current_node->dir->max_depth > min_depth && (!current_node->dir->is_root)){
            current_node = current_node->dir;
        }
        else{
            current_node = current_node->esq;
        }
    }

    int l_prime = predecessor(u, l); //calculate l'
    int r_prime = sucessor(u, r); //calculate r'


    NodeARN* root = u;    
    if(l_prime != -1 && r_prime != -1){ //bigger case

        root = bring_to_front(root, l_prime);
        root->dir = bring_to_front(root->dir, r_prime);
        if(root->dir != nullptr){
            root->dir->pai = root;
        }

        
        root->dir->esq->is_root = true;
        

        root->dir = concatenate(root->dir);
        if(root->dir != nullptr){
            root->dir->pai = root;
        }
        root = concatenate(root);
    }
    else if(l_prime == -1 && r_prime != -1){ //smaller case
        root = bring_to_front(root, r_prime);
        

        root->esq->is_root = true;
        

        root = concatenate(root);
    }
    else{
        root = bring_to_front(root, l_prime);
        root->dir->is_root = true;

        root = concatenate(root);
    }
    return root;
}

NodeARN* glue(NodeARN* u, int searched_item){
    int l_prime, r_prime;
    l_prime = r_prime = -1;

    NodeARN* current_node = u;

    while(!current_node->is_root){
        if(current_node->key > searched_item){ //go left
            r_prime = current_node->key;
            current_node = current_node->esq;
        }
        else{ //go right
            l_prime = current_node->key;
            current_node = current_node->dir;
        }
    }

    NodeARN* root = u;
    if(l_prime != -1 && r_prime != -1){ //bigger case

        root = bring_to_front(root, l_prime);
        root->dir = bring_to_front(root->dir, r_prime);
        if(root->dir != nullptr){
            root->dir->pai = root;
        }

        root->dir->esq->is_root = false;
        imprimir(root);

        root->dir = concatenate(root->dir);
        if(root->dir != nullptr){
            root->dir->pai = root;
        }
        root = concatenate(root);
    }
    else if(l_prime == -1 && r_prime != -1){ //l_prime == -1
        root = bring_to_front(root, r_prime);
        root->esq->is_root = false;
        
        root = concatenate(root);
    }
    else{ //r_prime == -1
        root = bring_to_front(root, l_prime);
        
        root->dir->is_root = false;
        root = concatenate(root);
    }
    root->is_root = true;
    return root;
}

NodeARN* tango(NodeARN* u, int min_depth, int searched_item){
    NodeARN* new_root = cut(u, min_depth);
    imprimir(new_root);
    new_root = glue(new_root, searched_item);
    imprimir(new_root);
    return new_root;
}

NodeARN* search_in_tango(NodeARN* root, int searched_item, bool& found){
    if(root == nullptr){
        return nullptr;
    }

    NodeARN* last_root = root;
    NodeARN* current_node = root;

    while(current_node != nullptr && !found){
        if(current_node->is_root && current_node != last_root){
            last_root = tango(last_root, current_node->min_depth - 1, searched_item);
            current_node = last_root;
        }

        if(current_node->key == searched_item){
            found = true;
        }
        else if(current_node->key > searched_item){ //go left
            current_node = current_node->esq;
        }
        else{ //go right
            current_node = current_node->dir;
        }
    }
    return last_root;
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