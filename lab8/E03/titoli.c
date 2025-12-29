#include "titoli.h"
#include "quote.h"
#include <string.h>

struct titolo_s{
    char name[MAX_NAME];
    quote_list_t quotazioni;
};

struct node{
    titolo_t titolo;
    struct node* next;
};

struct lista_titoli_s{
    struct node* head;
};

titolo_t titolo_init(){
    titolo_t ret = malloc(sizeof(struct titolo_s));
    ret->quotazioni = QL_init();
    return ret;
}

void titolo_parse(FILE* fp, titolo_t titolo){
    int q_n;
    fscanf(fp, " %20s %d", titolo->name, &q_n);
    for (int i = 0; i < q_n; i++){
        quote_t quote;
        quote_parse(fp, &quote);
        QL_insert(titolo->quotazioni, quote);
    }
}

double titolo_value(datetime_t date, titolo_t titolo){
    quote_t* quote = QL_search(date, titolo->quotazioni);
    return (quote ? quote->price : 0.0);
}

void titolo_free(titolo_t titolo){
    QL_free(titolo->quotazioni);
    free(titolo);
}

lista_titoli_t listaTitoli_init(){
    lista_titoli_t ret = malloc(sizeof(struct lista_titoli_s));
    ret->head = NULL;
    return ret;
}

void listaTitoli_free(lista_titoli_t lista){
    struct node* cur = lista->head;
    while (cur) {
        struct node* last = cur;
        cur = cur->next;
        titolo_free(last->titolo);
        free(last);
    }

    free(lista);
}

void listaTitoli_insert(titolo_t titolo, lista_titoli_t lista){
    struct node* cur = lista->head;
    struct node* last = NULL;
    int ret = -1;
    while (cur && (ret = strcmp(cur->titolo->name, titolo->name)) < 0){
        last = cur;
        cur = cur->next;
    }

    if (!ret) return; // titolo già presente in lista, ignoriamo l'inserimento.

    struct node* new = malloc(sizeof(struct node));
    if (!new) return;

    new->titolo = titolo;
    if (last) {
        last->next = new;
        new->next = cur;
    } else {
        new->next = lista->head;
        lista->head = new;
    }
}

titolo_t listaTitoli_search(char* name, lista_titoli_t lista){
    struct node* cur = lista->head;
    int ret = -1;
    while (cur && (ret = strcmp(cur->titolo->name, name)) < 0){
        cur = cur->next;
    }

    if (!ret)
        return cur->titolo;
    else
        return NULL;
}

int listaTitoli_parse(FILE* fp, lista_titoli_t lista){
    int n;
    fscanf(fp, " %d", &n);
    for (int i = 0; i < n; i++){
        titolo_t titolo = titolo_init();
        titolo_parse(fp, titolo);
        listaTitoli_insert(titolo, lista);
    }

    return n;
}

void titolo_interval_price(datetime_t date_start, datetime_t date_finish, titolo_t titolo, double* min, double* max){
    QL_interval_price(date_start, date_finish, titolo->quotazioni, min, max);
}
void titolo_minmax_price(titolo_t titolo, double* min, double* max){
    QL_minmax_price(titolo->quotazioni, min, max);
}
void titolo_balance(titolo_t titolo, double S){
    QL_balance(titolo->quotazioni, S);
}