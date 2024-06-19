#include<stdio.h>
#include<stdlib.h>
#include "../include/avl.h"

int max(int a,int b){
    return a>b?a:b;
}

int altura(tnode *arv){
    int ret;
    if (arv==NULL){
        ret = -1;
    }else{
        ret = arv->h;
    }
    return ret;
}

void _avl_insere(tnode ** parv, tnode * pai, titem *item){
    if (*parv == NULL){
        *parv = (tnode *) malloc(sizeof(tnode));
        (*parv)->item = *item;
        (*parv)->pai = pai;
        (*parv)->esq = NULL;
        (*parv)->dir = NULL;
        (*parv)->h = 0;

    }else if(item->key == (*parv)->item.key)  // inserção na lista encadeada
    {
        titem * aux = &((*parv)->item);
        while(aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = (titem *) malloc(sizeof(titem));
        *(aux->prox) = *item;
        aux->prox->prox = NULL;

    } else if(((*parv)->item.key - item->key)>0){
        _avl_insere(&(*parv)->esq,*parv, item);
    }else{                                      // recursão agora mantém um ponteiro do pai
        _avl_insere(&(*parv)->dir,*parv, item);
    }
    (*parv)->h = max(altura((*parv)->esq),altura((*parv)->dir)) + 1;
    _avl_rebalancear(parv);
}


void avl_insere(tnode ** parv, titem * item)
{
    _avl_insere(parv, NULL, item);           //driver para inserção recursiva
}


void _rd(tnode **parv){
    tnode * y = *parv; 
    tnode * x = y->esq;
    tnode * A = x->esq;
    tnode * B = x->dir;
    tnode * C = y->dir;
    tnode * aux = y->pai;

    x->pai = aux;   // alterações * pai
    y->pai = x;
    y->esq = B; 
    x->dir = y;
    *parv  = x;
    y->h = max(altura(B),altura(C)) + 1;
    x->h = max(altura(A),altura(y)) + 1;
}

void _re(tnode **parv){
    tnode * x = *parv; 
    tnode * y = x->dir;
    tnode * A = x->esq;
    tnode * B = y->esq;
    tnode * C = y->dir;
    tnode * aux = x->pai;
    x->pai = y;     // alterações * pai
    y->pai = aux;
    x->dir = B;
    y->esq = x;
    *parv  = y;
    x->h = max(altura(A),altura(B)) + 1;
    y->h = max(altura(x),altura(C)) + 1;
}


void _avl_rebalancear(tnode **parv){
    int fb;
    int fbf;
    tnode * filho;
    fb = altura((*parv)->esq) - altura((*parv)->dir);

    if (fb  == -2){
        filho = (*parv)->dir;
        fbf = altura(filho->esq) - altura(filho->dir);
        if (fbf <= 0){ /* Caso 1  --> ->*/
            _re(parv);
        }else{   /* Caso 2  --> <-*/
            _rd(&(*parv)->dir);
            _re(parv);
        }
    }else if (fb == 2){  
        filho = (*parv)->esq;
        fbf = altura(filho->esq) - altura(filho->dir);
        if (fbf >=0){ /* Caso 3  <-- <-*/
            _rd(parv);
        }else{  /* Caso 4  <-- ->*/
            _re(&(*parv)->esq);
            _rd(parv);
        }
    }
}

tnode ** percorre_esq(tnode ** arv){
    tnode * aux = *arv;
    if (aux->esq  == NULL){
        return arv;
    }else{
        while (aux->esq->esq != NULL)
            aux = aux->esq;
        return &(aux->esq);
    }
}
void avl_remove(tnode **parv, titem reg){
    int cmp;
    tnode *aux;
    tnode **sucessor;
    if (*parv != NULL){
        cmp  = (*parv)->item.key  - reg.key;
        if (cmp > 0){ /* ir esquerda*/
            avl_remove(&((*parv)->esq), reg);
        }else if (cmp < 0){ /*ir direita*/
            avl_remove(&((*parv)->dir), reg);
        }else{ /* ACHOU  */
            if ((*parv)->esq == NULL && (*parv)->dir == NULL){   /* no folha */
                free(*parv);
                *parv = NULL;
            }else if ((*parv)->esq == NULL || (*parv)->dir == NULL){ /* tem um filho*/
                aux = *parv;
                if ((*parv)->esq == NULL){
                    *parv = (*parv)->dir;
                }else{
                    *parv = (*parv)->esq;
                }
                free(aux);
            }else{ /* tem dois filhos */
                sucessor = percorre_esq(&(*parv)->dir);
                (*parv)->item = (*sucessor)->item;
                avl_remove(&(*parv)->dir,(*sucessor)->item);
            }
        }
        if (*parv != NULL){
            (*parv)->h = max(altura((*parv)->esq),altura((*parv)->dir)) + 1;
            _avl_rebalancear(parv);
        }
    }
}

void avl_destroi(tnode *parv){
    if (parv!=NULL){
        avl_destroi(parv->esq);
        avl_destroi(parv->dir);
        
        titem *aux = parv->item.prox;
        while (aux != NULL)
        {
            titem *temp = aux;
            aux = aux->prox;
           
            free(temp);
        }
        free(parv);
    }
}

tnode ** procuraNode(tnode ** parv, titem reg)
{
    if(*parv == NULL || (*parv)->item.key == reg.key)
    {
        return parv; 
    }
    
    if((*parv)->item.key > reg.key)
    {
        return procuraNode(&((*parv)->esq), reg); 
    } else {
        return procuraNode(&((*parv)->dir), reg); 
    }
}


tnode ** sucessor_avl(tnode ** parv, titem reg)
{
    tnode ** alvo = procuraNode(parv, reg);
    tnode ** ret;
    if((*alvo)->dir != NULL)
    {
        ret = percorre_esq(&((*parv)->dir)); // procura node mais à esquerda no nó da direita
    } else {
        while((*alvo)->pai != NULL)
        {
            if((*alvo)->pai->esq == *parv)
            {
                break;                       // encontrai pai com filho à esquerda ( devolve o mantém o ponteiro no filho à esquerda )
            }
            (*alvo) = (*alvo)->pai;
        }
        ret = alvo;
        
    }
    return ret;
}


int *query_avl(tnode *parv, int *lista, int *t, int inf, int sup)
{
    if (parv == NULL)
    {
        return lista;
    }

    if (parv->item.key > sup)
    {
        return query_avl(parv->esq, lista, t, inf, sup);
    }

    if (parv->item.key < inf)
    {
        return query_avl(parv->dir, lista, t, inf, sup);
    }

    if (parv->item.key >= inf && parv->item.key <= sup)
    {
        
        titem *current_item = &(parv->item);
        while (current_item->prox != NULL)
        {
            if (*t < TAM_QUERY)
            {
                lista[(*t)++] = ((tmunicipio *)(current_item->registro))->codIBGE;
            }

    
            current_item = current_item->prox;
        }
    }

    query_avl(parv->esq, lista, t, inf, sup);
    query_avl(parv->dir, lista, t, inf, sup);

    return lista;
}

int avl_busca(tnode ** parv, titem * item)
{
    if(((*parv)->item.key - item->key)>0){
        avl_busca(&(*parv)->esq, item);
    }else if(((*parv)->item.key - item->key)<0){                                     
        avl_busca(&(*parv)->dir, item);
    } else if(*parv == NULL)
    {
        printf("item não encontrado\n");
    } else {
        printf("item encontrado, chave: %d\n", item->key);
    }
}

