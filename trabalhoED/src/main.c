#include"../include/avl.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

void arquivaResultado(int *lista, int t, thash *h, const char *filename)
{
    char filepath[200] = "../out/";
    strcat(filepath, filename);
    FILE *file = fopen(filepath, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file %s for writing.\n", filename);
        return;
    }

    for (int i = 0; i < t; i++)
    {
        tmunicipio *out = (tmunicipio *)hash_busca(*h, lista[i]);
        if (out != NULL)
        {
            fprintf(file, "%s\nlatitude: %f\nlongitude: %f\ncodigo IBGE: %d\nDDD: %d\n\n", out->nome, out->latitude, out->longitude, out->codIBGE, out->ddd);
        } else {
            fprintf(file, "Municipio com codigo IBGE %d nao encontrado.\n\n", lista[i]);
        }
    }

    fclose(file);
}

void hash_imprime(thash *h)
{
    int pos;
    for (pos = 0; pos < h->max; pos++)
    {
        if (h->table[pos] != 0)
        { 
            if (h->table[pos] != h->deleted)
            {  
                printf("Position %d: %ld\n", pos, h->table[pos]); 
                printf("%d\n", ((tmunicipio*)h->table[pos])->codIBGE);
            }
        }
    }
}

int contem(int *set, int size, int element)
{
    for(int i = 0; i < size; i++)
    {
        if(set[i] == element)
        {
            return 1;
        }
    }
    return 0;
}


int* retornaInterseccao(int *lista1, int t1, int *lista2, int t2, int *lista3, int t3, int *t)
{
    
    int * lista = malloc(sizeof(int)*TAM_QUERY);
    
    for(int i = 0; i < t1; i++)
    {
        int found1 = 0;
        int found2 = 0;
        found1 = contem(lista2, t2, lista1[i]);
        found2 = contem(lista3, t3, lista1[i]);
        
        if(found1 && found2)
        {
            lista[(*t)++] = lista1[i];
        }
    }
    return lista;

}

void imprimeLista(int * lista, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf(" %d", lista[i]);
        
    }
    printf("\n");
}


int main(void)
{
    
    thash *h = malloc(sizeof(thash));
    tnode ** avl;
    processa_avl(&avl, h);
    printf("Hash de tamanho %d\n", h->size);
    printf("Trabalho pratico 2 --- Estrutura de dados\n");

    int inf, sup;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int *lista1 = malloc(sizeof(int) * TAM_QUERY);
    int *lista2 = malloc(sizeof(int) * TAM_QUERY);
    int *lista3 = malloc(sizeof(int) * TAM_QUERY);
    int  * t = malloc(sizeof(int));
    int i =0;
    while (1)
    {
        *t =0;
    
        printf("Digite o limitante inferior e superior do DDD\n(inferior) (superior)\n");
        scanf("%d %d", &inf, &sup);
        getchar();
        query_avl(avl[0], lista1, &t1, inf, sup);
        printf("%d DDD's encontrados\n", t1);
        //imprimeLista(lista1, t1);

        printf("Digite o limitante inferior e superior da longitude\n(inferior) (superior)\n");
        scanf("%d %d", &inf, &sup);
        getchar();
        query_avl(avl[2], lista2, &t2, inf, sup);
        printf("%d longitudes encontradas\n", t2);
        //imprimeLista(lista2, t2);

        printf("Digite o limitante inferior e superior da latitude\n(inferior) (superior)\n");
        scanf("%d %d", &inf, &sup);
        getchar();
        query_avl(avl[3], lista3, &t3, inf, sup);
        printf("%d latitudes encontradas\n", t3);
        //imprimeLista(lista3, t3);

        int *lista = retornaInterseccao(lista1, t1, lista2, t2, lista3, t3, t);

        printf("%d resultados encontrados.\n", *t);

        
        char filename[100];
        snprintf(filename, sizeof(filename), "query%d.txt", i++);
        arquivaResultado(lista, *t, h, filename);

        free(lista);

        printf("Digite 1 para realizar outra query, 0 para encerrar (limpeza de memoria)\n");
        scanf("%d", &inf);
        if (!inf) break;

        t1 = 0;
        t2 = 0;
        t3 = 0;
    }

    hash_apaga(h);
    free(h);
    free(lista1);
    free(lista2);
    free(lista3);
    for (int i = 0; i < 4; i++)
    {
        avl_destroi(avl[i]);
    }
    return EXIT_SUCCESS;
}