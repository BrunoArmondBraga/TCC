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
        int is_double_black = 0; //0 = false, 1 = true, 2 = to be deleted/null double black
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
    u->max_depth = -1;
    u->min_depth = 100000;
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
    return aux;
}

NodeARN* predecessor(NodeARN* root){
    if(root->esq == nullptr){
        return root;
    }
    return predecessor(root->esq);
}

NodeARN* sucessor(NodeARN* root){
    if(root->dir == nullptr){
        return root;
    }
    return sucessor(root->dir);
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

NodeARN *putRN(NodeARN *raiz, int key, NodeARN* to_be_positioned, int depth = 0){
    if(raiz == nullptr){
        if(to_be_positioned != nullptr){
            NodeARN* insert = reset_node(to_be_positioned);
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
    NodeARN* p = raiz;
    bool found = false;
    while(!found){
        if((p->key > key) && (p->esq != nullptr)){
            p = p->esq;
        }
        else if((p->key > key) && (p->esq == nullptr)){
            found = true;
        }
        else if((p->key < key) && (p->dir != nullptr)){
            p = p->dir;
        }
        else if((p->key < key) && (p->dir == nullptr)){
            found = true;
        }
        else{
            found = true;
        }
    }
    
    NodeARN *filho;
    if(to_be_positioned == nullptr){
        filho = new NodeARN(p,key,depth);
    }
    else{
        filho = reset_node(to_be_positioned);
    }

    if((p->key > key) && (p->esq == nullptr)){
        p->esq = filho;
    }
    else{
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
        NodeARN* suc = sucessor(root);
        root->depth = suc->depth; //ESTAMOS PERDENDO INFORMAÇÃO DA DEPTH!!
        root->key = suc->key;
        suc->key = root->key;
        deleteRN(suc);
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
    raiz->is_root = true;
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

NodeARN* Join(NodeARN *u, NodeARN* k, NodeARN *v){
    if(u == nullptr){
        NodeARN* to_be_returned = add(v, k->key, k, k->depth);
        return to_be_returned;
    }
    if(v == nullptr){
        NodeARN* to_be_returned = add(u, k->key, k, k->depth);
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
        novo->is_root = true;
        return novo;
    }

    NodeARN *x;
    NodeARN *final_root;
    v->is_root = false;
    u->is_root = false;

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
        Fix(novo);
    }
    while(final_root->pai != nullptr){
        final_root = final_root->pai;
    }
    final_root->is_root = true;
    return final_root;
}

split_data Split(NodeARN *root, float pivot){
    stack<NodeARN*> left_trees;
    stack<NodeARN*> right_trees;

    NodeARN* search = root;
    search->is_root = false;
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

        NodeARN* final_insertion = final_left_tree; //get last 
        //final_insertion->is_root = false;

        if(final_left_tree->esq != nullptr){
            final_left_tree = final_left_tree->esq;
            if(final_left_tree->pai != nullptr){
                final_left_tree->pai->esq = nullptr;
            }
            final_left_tree->pai = nullptr;
        }
        else{
            final_left_tree = nullptr;
        }


        /* if(final_left_tree->esq == nullptr){
            //delete final_left_tree;
            //final_left_tree = nullptr;
        }
        else{       
            final_left_tree = final_left_tree->esq;
            if(final_left_tree->pai != nullptr){
                final_left_tree->pai->esq = nullptr;
            } 
            //delete final_left_tree->pai;
            //final_left_tree->pai = nullptr;
        } */
        
        
        while(left_trees.size() > 0){
            NodeARN* aux_tree = left_trees.top();
            left_trees.pop();

            //int aux_index = aux_tree->key;

            NodeARN* insertion_node = aux_tree;
            //insertion_node->is_root = false;

            /* if(aux_tree->esq != nullptr){

                aux_tree = aux_tree->esq;
                if(aux_tree->pai != nullptr){
                    aux_tree->pai->esq = nullptr;
                }
                delete aux_tree->pai;
                aux_tree->pai = nullptr;
            }
            else{
                delete aux_tree;
                aux_tree = nullptr;
            } */

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

            final_left_tree = Join(aux_tree, insertion_node, final_left_tree);
        }

        final_left_tree = Join(final_left_tree, final_insertion, nullptr);
    }

    NodeARN* final_right_tree = nullptr;
    if(right_trees.size() > 0){
        final_right_tree = right_trees.top();
        right_trees.pop();

        NodeARN* final_insertion = final_right_tree;
        //final_insertion->is_root = false;

        /* if(final_right_tree->dir == nullptr){
            delete final_right_tree;
            final_right_tree = nullptr;
        }
        else{
            final_right_tree = final_right_tree->dir;
            
            final_right_tree->pai->dir = nullptr;
            delete final_right_tree->pai;
            final_right_tree->pai = nullptr;
        } */

        if(final_right_tree->dir != nullptr){
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
            //insertion_node->is_root = false;

            /* int aux_index = aux_tree->key;

            if(aux_tree->dir != nullptr){

                aux_tree = aux_tree->dir;
                if(aux_tree->pai != nullptr){
                    aux_tree->pai->dir = nullptr;
                }
                delete aux_tree->pai;
                aux_tree->pai = nullptr;
            }
            else{
                delete aux_tree;
                aux_tree = nullptr;
            } */

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

            final_right_tree = Join(final_right_tree, insertion_node, aux_tree);
        }
        final_right_tree = Join(nullptr, final_insertion, final_right_tree);
    }
    if(final_left_tree != nullptr){
        final_left_tree->is_root = true;
    }
    if(final_right_tree != nullptr){
        final_right_tree->is_root = true;
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

NodeARN* bring_to_front(NodeARN* root, int item){
    if(root == nullptr){
        return nullptr;
    }

    split_data first_split = Split(root, item - 0.5);
    split_data second_split = Split(first_split.right_tree, item + 0.5);

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

    if(single_node->esq != nullptr){
        single_node->esq->pai = nullptr;
        add_to_tango(first_fragment, single_node->esq);
    }
    if(single_node->dir != nullptr){
        single_node->dir->pai = nullptr;
        add_to_tango(second_fragment, single_node->dir);
    }
    single_node->esq = first_fragment;
    single_node->dir = second_fragment;

    if(first_fragment != nullptr){
        first_fragment->is_root = false;
    }
    if(second_fragment != nullptr){
        second_fragment->is_root = false;
    }

    single_node->preto = true;
    
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

    NodeARN* single_node = root;
    NodeARN* first_fragment = root->esq;
    NodeARN* second_fragment = root->dir;
    root->esq = nullptr;
    root->dir = nullptr;
    return Join(first_fragment, reset_node(root), second_fragment);
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