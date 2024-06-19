#include"../include/cJSON.h"
#include"../include/avl.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

int get_key(void *reg)
{
    return ((tmunicipio *)reg)->codIBGE;
}

void processa_avl(tnode *** h, thash * t)
{
    FILE *fp = fopen("../municipios.json", "r");
    printf("Abrindo Arquivo\n");
    
    char buffer[1024];

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    printf("Tamanho de arquivo encontrado:  %ld\n", size);
    char *jsonData =(char *)malloc(size + 1);
    long bytes = fread(jsonData, 1, size, fp);
    
    jsonData[bytes] = '\0';

    fclose(fp);

    cJSON *root = cJSON_Parse(jsonData);

    
    *h = malloc(sizeof(tnode *) * 4);
    for (int i = 0; i < 4; i++)
    {
        (*h)[i] = NULL;
    }

    if (hash_constroi(t, 6133, get_key) == EXIT_FAILURE)
    {
        perror("Failed to construct hash table");
        free(jsonData);
        return;
    }

    cJSON *item = root->child;

    while(item != NULL)
    {

        cJSON *codigo_ibge = cJSON_GetObjectItemCaseSensitive(item, "codigo_ibge");
        cJSON *nome = cJSON_GetObjectItemCaseSensitive(item, "nome"); 
        cJSON *latitude = cJSON_GetObjectItemCaseSensitive(item, "latitude");
        cJSON *longitude = cJSON_GetObjectItemCaseSensitive(item, "longitude");
        cJSON *capital = cJSON_GetObjectItemCaseSensitive(item, "capital");
        cJSON *codigo_uf = cJSON_GetObjectItemCaseSensitive(item, "codigo_uf");
        cJSON *siafi_id = cJSON_GetObjectItemCaseSensitive(item, "siafi_id");
        cJSON *ddd = cJSON_GetObjectItemCaseSensitive(item, "ddd");
        cJSON *fuso_horario = cJSON_GetObjectItemCaseSensitive(item, "fuso_horario");

        tmunicipio *novo_municipio = (tmunicipio *)malloc(sizeof(tmunicipio));
        
        novo_municipio->codigo_uf = codigo_uf->valueint;
        strcpy(novo_municipio->nome, nome->valuestring);
        strcpy(novo_municipio->fuso_horario, fuso_horario->valuestring);
        novo_municipio->latitude = latitude->valuedouble;
        novo_municipio->longitude = longitude->valuedouble;
        novo_municipio->capital = capital->valueint;
        novo_municipio->siafi_id = siafi_id->valueint;
        novo_municipio->ddd = ddd->valueint;
        novo_municipio->codIBGE = codigo_ibge->valueint;
        
        for(int i = 0; i < 4; i++) // laço itera sobre todos as árvores, para mais detalhes, observar 'titem.c'
        {
            titem * it;
            it = constroi_item(novo_municipio, i);
            avl_insere(&((*h)[i]), it);
        }
        
        hash_insere(t, novo_municipio);
        item = item->next;
    }
    
    free(jsonData);
    cJSON_Delete(root);
    printf("Encerrando parsing . . .\n");

}
