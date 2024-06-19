#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define SEED    0x12345678
#include "../include/avl.h"

int hash_insere(thash *h, void *bucket)
{
    int key = h->get_key(bucket);
    int pos = key % h->max;
    // Check if the table is full
    if (h->max == (h->size + 1))
    {
        free(bucket);
        return EXIT_FAILURE;
    } else {
        // Insert the bucket
        while (h->table[pos] != 0)
        {
            if (h->table[pos] == h->deleted)
                break;
            pos = (pos + 1) % h->max;
        }
        h->table[pos] = (uintptr_t)bucket;
        h->size += 1;
    }
    return EXIT_SUCCESS;
}

int hash_constroi(thash *h, int nbuckets, int (*get_key)(void *))
{
    h->table = calloc(sizeof(void *), nbuckets + 1);
    if (h->table == NULL)
    {
        return EXIT_FAILURE;
    }
    h->max = nbuckets + 1;
    h->size = 0;
    h->deleted = (uintptr_t)&(h->size);
    h->get_key = get_key;
    return EXIT_SUCCESS;
}

void *hash_busca(thash h, int key)
{
    int pos = key % h.max;
    void *ret = NULL;
    
    while (h.table[pos] != 0 && h.table[pos] != h.deleted)
    {
        if (h.get_key((void*)h.table[pos]) == key)
        {
            ret = (void*)h.table[pos];
        }
        pos = (pos + 1) % h.max;
    }

    return ret;
}

int hash_remove(thash *h, int key)
{
    
    int pos = key % h->max;
    while (h->table[pos] != 0)
    {
        if (h->get_key((void *)h->table[pos]) == key)
        { // If key matches, remove
            free((void *)h->table[pos]);
            h->table[pos] = h->deleted;
            h->size -= 1;
            return EXIT_SUCCESS;
        } else {
            pos = (pos + 1) % h->max;
        }
    }
    return EXIT_FAILURE;
}

void hash_apaga(thash *h)
{
    int pos;
    for (pos = 0; pos < h->max; pos++)
    {
        if (h->table[pos] != 0)
        {
            if (h->table[pos] != h->deleted)
            {
                free((void *)h->table[pos]);
            }
        }
    }
    free(h->table);
}

