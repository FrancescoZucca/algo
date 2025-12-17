#include "rete.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 31

struct elaboratore {
    char nome[MAX_STR], subnet[MAX_STR];
};

struct node{
    struct elaboratore elab;
    struct node* next;
};

struct matrice {
    int n;
    int** data;
};

struct rete {
    int n;
    struct matrice matrice;
    int** lista;
    struct node* elaboratori;
};

void setMatrix(rete_t rete, int i, int j, int flusso) {
    if (i < j) {int tmp = i; i = j; j = tmp;}
    if (i==j) return;

    if (rete->matrice.n < i) {
        int** new = malloc(sizeof(int*)*i);
        for (int k=0; k<i; k++) {
            if (k < rete->matrice.n) new[k] = rete->matrice.data[k];
            else new[k] = calloc(k+1, sizeof(int));
        }
        if (rete->matrice.data)
            free(rete->matrice.data);
        rete->matrice.data = new;
        rete->matrice.n = i;
    }

    rete->matrice.data[i-1][j] = flusso;
}

int getMatrix(rete_t rete, int i, int j) {
    if (i < j) {int tmp = i; i = j; j = tmp;}

    if (i==j) return 0;
    if (i > rete->matrice.n) return 0;
    return rete->matrice.data[i-1][j];
}

void updateMatrix(rete_t rete, int i) {
    for (int j= i>rete->matrice.n?i:rete->matrice.n; j>i; j--) {
        for (int k = 0; k < j; k++) {
            if (k < i)
                setMatrix(rete, j+1, k, getMatrix(rete, j, k));
            else if (k > i)
                setMatrix(rete, j+1, k+1, getMatrix(rete, j, k));
            else
                setMatrix(rete, j+1, k, 0);
        }
    }
    for (int k = 0; k < i; k++)
        setMatrix(rete, i, k, 0);
}

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

    //printf("idx = %d ret = %d\n", idx, ret);

    if (!cur || ret) {
        printf("Aggiungo %s...\n", elab.nome);
        rete->n++;
        struct node* new = malloc(sizeof(struct node));
        strcpy(new->elab.nome, elab.nome);
        strcpy(new->elab.subnet, elab.subnet);
        
        if (idx){
            last->next = new;
            new->next = cur;
        } else {
            printf("Il nodo %s è il primo della lista.\n", elab.nome);
            new->next = rete->elaboratori;
            rete->elaboratori = new;
        }

        updateMatrix(rete, idx);
    }

    return idx;
}

int getNode(rete_t rete, const char* nome) {
    struct node* cur = rete->elaboratori;
    int ret = 0;
    int idx = 0;
    while (cur && (ret = strcmp(nome, cur->elab.nome)) > 0) {
        cur = cur->next;
        idx++;
    }

    if (ret) return -1;
    return idx;
}

char* getName(rete_t rete, int i){
    struct node* cur = rete->elaboratori;

    for (int r = 0; cur && r <= i; r++) {
        if (r == i) {
            return cur -> elab.nome;
        }

        cur = cur -> next;
    }

    return NULL;
}

int** generateList(rete_t rete) {
    int ** ret = malloc(rete->n * sizeof(int*));
    for (int i = 0; i < rete->n; i++) {
        ret[i] = malloc(rete->n * sizeof(int));
        int idx = 0;
        for (int j = 0; i < rete->n; i++) {
            ret[i][j] = -1;
            if (getMatrix(rete, i, j)) ret[i][idx++] = j;
        }
    }

    rete->lista = ret;
    return ret;
}

void printNetworkInfo(FILE* fout, rete_t rete, int full) {

    for (int i = 0; i < rete->n; i++) {
        for (int j = 0; j < i; j++)
		fprintf(fout, "%2d ", getMatrix(rete, i, j));
	fprintf(fout, "\n");
    }

    if (full) {
        struct node* cur = rete->elaboratori;
        int i = 0, ret;
        while (cur) {
            fprintf(fout, "- %s\n", cur->elab.nome);
            for (int j = 0; j < rete->n; j++) {
                if ((ret = getMatrix(rete, i, j)))
                    fprintf(fout, "\t - %s: Flusso %d\n", getName(rete, j), ret);
            }
            i++;
            cur = cur->next;
        }
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

    while(fscanf(fin, " %30s %30s %30s %30s %d ", a.nome, a.subnet, b.nome, b.subnet, &flusso) == 5){
        printf("Aggiunta arco %s - %s con flusso %d\n", a.nome, b.nome, flusso);
        addOrGetNode(rete, a);
        addOrGetNode(rete, b);
        setMatrix(rete, getNode(rete, a.nome), getNode(rete, b.nome), flusso);
        printNetworkInfo(stdout, rete, 0);
    }
    printf("%d nodi caricati.\n", rete->n);

    fclose(fin);
    return rete;
}

int main() {
    rete_t rete = parseFile("lab8/E02/grafo.txt");
    if (!rete) {
        printf("File non trovato. \n");
        return -1;
    }
    printNetworkInfo(stdout, rete, 1);
}
