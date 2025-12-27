#include "pgList.h"

struct node {
    pg_t pg;
    struct node* next;
};

struct pgList_s {
    int n;
    struct node* head;
    struct node* tail;
};

/* creatore e distruttore */
pgList_t pgList_init(){
    pgList_t ret = malloc(sizeof(struct pgList_s));
    ret->head = NULL;
    ret->tail = NULL;
    ret->n = 0;
}

void pgList_free(pgList_t pgList){
    struct node* cur = pgList->head;
    while (cur){
        struct node* last = cur;
        cur = cur->next;
        free(last);
    }

    free(pgList);
}

/* lettura e scrittura su file */
void pgList_read(FILE *fp, pgList_t pgList){
    pg_t pg;
    while (!pg_read(fp, &pg))
        pgList_insert(pgList, pg);
    
}
void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray){
    struct node* cur = pgList->head;
    while (cur) {
        pg_print(fp, &cur->pg, invArray);
        cur = cur->next;
    }
}

/* inserimento di un nuovo personaggio */
void pgList_insert(pgList_t pgList, pg_t pg){
    struct node* new = malloc(sizeof(struct node));
    new->pg = pg;
    new->next = NULL;
    if (pgList->tail == NULL){
        pgList->head = new;
        pgList->tail = new;
    } else {
        pgList->tail->next = new;
        pgList->tail = new;
    }
    pgList->n++;
}
/* cancellazione con rimozione */
void pgList_remove(pgList_t pgList, char* cod){
    struct node* cur = pgList->head;
    struct node* last = pgList->head;
    while (cur && strcmp(cur->pg.cod, cod)){
        last = cur;
        cur = cur->next;
    }

    if (cur) {
        if (cur == pgList->head) pgList->head = cur->next;
        else last->next = cur->next;
        pgList->n--;
        free(cur);
    }
}
/* ricerca per codice, ritornando il puntatore */
pg_t *pgList_searchByCode(pgList_t pgList, char* cod){
    struct node* cur = pgList->head;
    while (cur && strcmp(cur->pg.cod, cod)){
        cur = cur->next;
    }

    if (cur) {
        return &cur->pg;
    }

    return NULL;
}