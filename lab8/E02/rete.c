#include "rete.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 31

// Matrice triangolare inferiore rappresentante le adiacenze pesate.
// Viene salvata solo la parte necessaria, ovvero il triangolo inferiore
// shiftato di uno verso l'alto, in quanto non sono ammessi cappi.
// matrice[i][j] rappresenta l'arco fra i nodi di indici i+1 e j.
struct row {
    int* row;
    struct row* next;
};

struct elaboratore {
    char nome[MAX_STR], subnet[MAX_STR];
};

struct node{
    struct elaboratore elab;
    struct node* next;
};

struct rete {
    int n;
    struct row* matrice;
    int** lista;
    struct node* elaboratori;
};


int addOrGetNode(rete_t rete, struct elaboratore elab){
    struct node* cur = rete->elaboratori;
    struct node* last = rete->elaboratori;
    int ret;
    int idx = 0;
    while (cur && (ret = strcmp(elab.nome, cur->elab.nome)) > 0) {
        last = cur;
        cur = cur->next;
        idx++;
    }

    printf("idx = %d ret = %d\n", idx, ret);

    if (!cur || ret) {
        printf("Aggiungo %s...\n", elab.nome);
        rete->n++;
        struct node* new = malloc(sizeof(struct node));
        printf("a");
        strcpy(new->elab.nome, elab.nome);
        printf("b");
        strcpy(new->elab.subnet, elab.subnet);
        
        if (idx){
            last->next = new;
            new->next = cur;
        } else {
            printf("Il nodo %s è il primo della lista.\n", elab.nome);
            new->next = rete->elaboratori;
            rete->elaboratori = new;
        }
    }

    return idx;
}

int getNode(rete_t rete, const char* nome) {
    struct node* cur = rete->elaboratori;
    struct node* last = rete->elaboratori;
    int ret;
    int idx = 0;
    while (cur && (ret = strcmp(nome, cur->elab.nome)) > 0) {
        last = cur;
        cur = cur->next;
        idx++;
    }

    if (ret) return -1;
    return idx;
}

char* getName(rete_t rete, int i){
    struct node* cur = rete->elaboratori;

    for (int r = 0; r <= i; r++) {
        if (r == i) {
            return cur -> elab.nome;
        }
    }

    return 0;
}

void setMatrix(struct row* matrix, int i, int j, int flusso) {
    if (i < j) {int tmp = i; i = j; j = tmp;}
    if (i == j) return;

    struct row* cur = matrix;
    struct row* last = matrix;

    for (int r = 0; r < i; r++) {
        if (!cur) {
            last->next = malloc(sizeof(struct row*));
            last -> next -> row = calloc(r, sizeof(int));
            cur = last->next;
            cur->next = NULL;
        }

        if (r == i-1)
            cur -> row[j] = flusso;

        cur = cur->next;
    }
}

int getMatrix(rete_t rete, int i, int j) {
    if (i < j) {int tmp = i; i = j; j = tmp;}
    if (i == j) return 0;

    struct row* cur = rete->matrice;

    for (int r = 0; r <= i && cur; r++) {
        if (r == i) {
            return cur -> row[j];
        }

        cur = cur->next;
    }

    return 0;
}

int** generateList(rete_t rete) {
    int ** ret = malloc(rete->n * sizeof(int*));
    for (int i = 0; i < rete->n; i++) {
        ret[i] = malloc(rete->n * sizeof(int));
        int idx = 0;
        for (int j = 0; i < rete->n; i++) {
            ret[i][j] = -1;
            int k;
            if (getMatrix(rete, i, j)) ret[i][idx++] = j;
        }
    }

    rete->lista = ret;
    return ret;
}

void printNetworkInfo(FILE* fout, rete_t rete) {
    struct node* cur = rete->elaboratori;
    int i = 0, ret;
    while (cur) {
        printf("- %30s\n", cur->elab.nome);
        for (int j = 0; j < rete->n; j++) {
            if ((ret = getMatrix(rete, i, j)))
                printf("\t - %30s: Flusso %d\n", getName(rete, j), ret);
        }
        i++;
    }
}

rete_t parseFile(const char* filename) {
    FILE* fin = fopen(filename, "r");

    if(!fin) return NULL;

    int flusso;

    struct elaboratore a, b;

    rete_t rete = malloc(sizeof(struct rete));

    rete -> n = 0;
    rete -> elaboratori = NULL;
    rete -> lista = NULL;
    rete -> matrice = malloc(sizeof(struct row*));
    rete -> matrice -> row = malloc(sizeof(int));

    while(fscanf(fin, " %30s %30s %30s %30s %d ", a.nome, a.subnet, b.nome, b.subnet, &flusso) == 5){
        printf("Aggiunta arco %s - %s con flusso %d\n", a.nome, b.nome, flusso);
        int i = addOrGetNode(rete, a);
        printf("\t Aggiunto nodo %s\n", a.nome);
        int j = addOrGetNode(rete, b);
        printf("\t Aggiunto nodo %s\n", b.nome);

        setMatrix(rete->matrice, i, j, flusso);
    }
    printf("%d nodi caricati.\n", rete->n);

    fclose(fin);
    return rete;
}