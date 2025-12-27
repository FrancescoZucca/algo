#include "invArray.h"

struct invArray_s {
    int n;
    inv_t* items;
};

/* creatore e disruttore */
invArray_t invArray_init(){
    return calloc(1, sizeof(struct invArray_s));
}

void invArray_free(invArray_t invArray){
    free(invArray);
}

/* lettura e scrittura su file */
void invArray_read(FILE *fp, invArray_t invArray){
    fscanf(fp, " %d ", &invArray->n);
    invArray->items = malloc(invArray->n * sizeof(inv_t));
    for (int i = 0; i < invArray->n; i++)
        inv_read(fp, &invArray->items[i]);
}

void invArray_print(FILE *fp, invArray_t invArray){
    fprintf(fp, "%d\n", invArray->n);
    for(int i = 0; i<invArray->n; i++) {
        printf("%d - ", i);
        inv_print(fp, &invArray->items[i]);
    }
}

/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
    inv_print(fp, &invArray->items[index]);
}
/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t *invArray_getByIndex(invArray_t invArray, int index){
    return &invArray->items[index];
}
/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name){
    for (int i = 0; i < invArray->n; i++){
        if (!strcmp(name, invArray->items[i].nome))
            return i;
    }

    return -1;
}