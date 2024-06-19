#include"../include/avl.h"
#include<stdio.h>
#include<stdlib.h>
#define SEED    0x12345678
#include<stdint.h>
#include<string.h>

uint32_t hashf(const char* str, uint32_t h){
    /* One-byte-at-a-time Murmur hash 
    Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}
titem *constroi_item(tmunicipio *registro, int tipo)
{
    titem *t = malloc(sizeof(titem));
    if (!t)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    t->prox = NULL;

    switch (tipo)
    {
        case 0:
            t->registro = malloc(sizeof(tddd));
            ((tddd*)(t->registro))->codIBGE = registro->codIBGE;
            ((tddd*)(t->registro))->ddd = registro->ddd;
            t->key = registro->ddd;
            break;

        case 1:
            t->registro = malloc(sizeof(tnome));
            
            ((tnome*)t->registro)->codIBGE = registro->codIBGE;
            strcpy(((tnome*)t->registro)->nome, registro->nome);
            t->key = hashf(registro->nome, SEED);
            break;

        case 2:
            t->registro = malloc(sizeof(tlongitude));
            
            ((tlongitude*)t->registro)->codIBGE = registro->codIBGE;
            ((tlongitude*)t->registro)->longitude = registro->longitude;
            t->key = registro->longitude;
            break;

        case 3:
            t->registro = malloc(sizeof(tlatitude));
            
            ((tlatitude*)t->registro)->codIBGE = registro->codIBGE;
            ((tlatitude*)t->registro)->latitude = registro->latitude;
            t->key = registro->latitude;
            break;

        default:
            free(t);
            return NULL;
    }

    return t;
}