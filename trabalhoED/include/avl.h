#ifndef __AVL__
#define  __AVL__

typedef struct _item
{
    int key;
    void * registro;
    struct _item * prox;
} titem;

typedef struct {
    int codIBGE;
    char nome[1024];
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    unsigned int siafi_id;
    unsigned int ddd;
    char fuso_horario[1024];

}tmunicipio;


typedef struct 
{
    int codIBGE;
    int ddd;   
}tddd;

typedef struct 
{
    int codIBGE;
    int latitude; // arrendondei floats...
}tlatitude;

typedef struct 
{
    int codIBGE;
    int longitude;
}tlongitude;

typedef struct 
{
    int codIBGE;
    char nome[1024];
}tnome;

typedef struct _node{
    titem item;
    struct _node *pai;
    struct _node *esq;
    struct _node *dir;
    int h;
}tnode;

#include<stdint.h>

typedef struct {
        uintptr_t * table;
        int size;
        int max;
        uintptr_t deleted;
        int (*get_key)(void *);
}thash;

uint32_t hashf(const char* str, uint32_t h);

int hash_insere(thash * h, void * bucket);
int hash_constroi(thash * h,int nbuckets, int (*get_key)(void *) );
void * hash_busca(thash h, int key);
int hash_remove(thash * h, int key);
void hash_apaga(thash *h);

void avl_insere(tnode ** parv,titem *reg);
void avl_remove(tnode ** parv,titem reg);
void avl_destroi(tnode * parv);
void processa_avl(tnode *** h, thash * t);
void _rd(tnode ** pparv);
void _re(tnode ** pparv);
void _avl_rebalancear(tnode ** pparv);
tnode ** sucessor_avl(tnode ** parv, titem reg);
titem * constroi_item(tmunicipio * registro, int tipo);
tnode ** procuraNode(tnode ** parv, titem reg);
int avl_busca(tnode ** parv, titem * item);
int* query_avl(tnode *parv, int *result_list, int *result_size, int inf, int sup);

#define TAM_QUERY 2000

#endif