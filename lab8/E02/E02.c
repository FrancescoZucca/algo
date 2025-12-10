#include "rete.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 31

// Matrice triangolare inferiore rappresentante le adiacenze pesate.
// Viene salvata solo la parte necessaria, ovvero il triangolo inferiore
// shiftato di uno verso l'alto, in quanto non sono ammessi cappi.
// matrice[i][j] rappresenta l'arco fra i nodi di indici i e j+1 
struct row {
    int* row;
    struct matrix* next;
};

struct node{
    elaboratore_t elab;
    struct node* next;
};

struct rete {
    int n;
    struct row* matrice;
    int** lista;
    struct node* elaboratori;
};

struct elaboratore {
    char nome[MAX_STR], subnet[MAX_STR];
};

int addOrGetNode(rete_t* rete, elaboratore_t elab){
    struct node* cur = rete->elaboratori;
    struct node* last = rete->elaboratori;
    int ret;
    int idx = 0;
    while (last && cur->next && (ret = strcmp(elab.nome, cur->elab.nome)) > 0) {
        last = cur;
        cur = cur->next;
        idx++;
    }

    if (!last || ret) {
        rete->n++;
        struct node* new = malloc(sizeof(struct node));
        new -> elab = elab;
        
        if (idx){
            last->next = new;
            new->next = cur;
        } else {
            new->next = rete->elaboratori;
            rete->elaboratori = new;
        }
    }

    return idx;
}

void setMatrix(struct row* matrix, int i, int j) {
    if (i > j) {int tmp = i; i = j; j = tmp;}
    if (i == j) return;

    struct row* cur = matrix;
    struct row* last = matrix;

    for (int r = 1; r <= i; i++) {
        if (!cur) {
            last->next = malloc(r * sizeof(int));
            cur = last->next;
        }

        
    }
}

int parseFile(const char* filename, rete_t* rete) {
    FILE* fin = fopen(filename, "r");

    if(!fin) return 1;

    int flusso;

    elaboratore_t a, b;

    rete -> n = 0;
    rete -> elaboratori = NULL;
    rete -> lista = NULL;
    rete -> matrice = malloc(sizeof(int));

    while(fscanf(fin, " %30s %30s %30s %30s %d ", a.nome, a.subnet, b.nome, b.subnet, &flusso) == 4){
        int i = addOrGetNode(rete, a);
        int j = addOrGetNode(rete, b);


    }

    fclose(fin);
    return 0;
}