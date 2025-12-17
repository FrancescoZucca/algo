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

struct arco {
    char a[MAX_STR], b[MAX_STR];
    int flusso;
    struct arco* next;
};


struct rete {
    int n;
    int** matrice;
    int** lista;
    struct node* elaboratori;
    struct arco* archi;
};

int getMatrix(rete_t rete, int i, int j) {
    if (i < j) {int tmp = j; j = i; i = tmp;}
    if (i == j) return 0;

    return rete->matrice[i-1][j];
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

    if (!cur || ret) {
        printf("\t - Aggiungo %s...\n", elab.nome);
        rete->n++;
        struct node* new = malloc(sizeof(struct node));
        strcpy(new->elab.nome, elab.nome);
        strcpy(new->elab.subnet, elab.subnet);
        
        if (idx){
            last->next = new;
            new->next = cur;
        } else {
            //printf("Il nodo %s è il primo della lista.\n", elab.nome);
            new->next = rete->elaboratori;
            rete->elaboratori = new;
        }
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
        for (int j = 0; j < rete->n; j++) {
            ret[i][j] = -1;
            if (getMatrix(rete, i, j)) ret[i][idx++] = j;
        }
    }

    rete->lista = ret;
    return ret;
}

void printNetworkInfo(FILE* fout, rete_t rete, int full) {

    if (full) {
        for (int i = 0; i < rete->n; i++) {
            for (int j = 0; j < i; j++)
            fprintf(fout, "%2d ", getMatrix(rete, i, j));
        fprintf(fout, "\n");
        }
    }

    struct node* cur = rete->elaboratori;
    int i = 0, ret;
    while (cur) {
        fprintf(fout, "- %s (%d)\n", cur->elab.nome, i);
            for (int j = 0; j < rete->n; j++) {
            if ((ret = getMatrix(rete, i, j)))
                fprintf(fout, "\t - %s: Flusso %d\n", getName(rete, j), ret);
        }
        i++;
        cur = cur->next;
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
    rete -> archi = NULL;
    struct arco* cur = rete -> archi;

    while(fscanf(fin, " %30s %30s %30s %30s %d ", a.nome, a.subnet, b.nome, b.subnet, &flusso) == 5){
        printf("Aggiunta arco %s - %s con flusso %d\n", a.nome, b.nome, flusso);
        addOrGetNode(rete, a);
        addOrGetNode(rete, b);
        
        if (!cur) {
            rete -> archi = malloc(sizeof(struct arco));
            cur = rete->archi;
        } else {
            cur -> next = malloc(sizeof(struct arco));
            cur = cur -> next;
        }
        strcpy(cur->a, a.nome);
        strcpy(cur->b, b.nome);
        cur->flusso = flusso;
        cur -> next = NULL;
        
    }
    printf("%d nodi caricati.\n", rete->n);

    rete -> matrice = malloc(rete->n * sizeof(int*));
    for (int i = 1; i <= rete->n; i++)
        rete->matrice[i-1] = calloc(i, sizeof(int));

    cur = rete->archi;
    int n = 0;
    while (cur) {
        int i = getNode(rete, cur->a);
        int j = getNode(rete, cur->b);

        if (i < j) {int tmp = j; j = i; i = tmp;}

        rete->matrice[i-1][j] = cur->flusso;

        cur = cur->next;
        n++;
    }

    printf("%d archi caricati.\n", n);

    fclose(fin);
    return rete;
}

void isSubgraphComplete(rete_t rete, int useList) {
    int flag = 1;
    int a, b, c;
    char sa[MAX_STR], sb[MAX_STR], sc[MAX_STR];
    while(flag) {
        printf("Inserire i nomi dei tre elaboratori: \n");
        scanf(" %s %s %s", sa, sb, sc);

        a = getNode(rete, sa);
        b = getNode(rete, sb);
        c = getNode(rete, sc);

        if (a == -1 || b == -1 || c == -1) printf("Riprovare.\n");
        else flag = 0;
    }

    if (!useList) {
        if (getMatrix(rete,a,b) && getMatrix(rete,b,c) && getMatrix(rete,c,a))
            printf("%s, %s e %s formano un sottografo completo.\n", sa, sb, sc);
        else
            printf("%s, %s e %s non formano un sottografo completo.\n", sa, sb, sc);
    } else {
        if (!rete->lista) generateList(rete);

        int flag = 0;
        for (int i = 0; i < rete->n && flag != 2; i++) {
            if (rete->lista[a][i] == -1) break; // Abbiamo iterato sino alla fine della lista.

            if (rete->lista[a][i] == b || rete -> lista[a][i] == c) flag++; 
        }

        if (flag == 2) {
            flag = 0;
            for (int i = 0; i < rete->n && flag != 2; i++) {
                if (rete->lista[c][i] == -1) break; // Abbiamo iterato sino alla fine della lista.

                if (rete->lista[c][i] == a || rete -> lista[c][i] == b) flag++; 
            }
        }

        if (flag == 2)
            printf("%s, %s e %s formano un sottografo completo.\n", sa, sb, sc);
        else
            printf("%s, %s e %s non formano un sottografo completo.\n", sa, sb, sc);
    }
}

void freeNet(rete_t rete) {
    for (int i = 0; i < rete->n; i++){
        free(rete->matrice[i]);
        free(rete->lista[i]);
    }
    free(rete->matrice);
    free(rete->lista);

    struct node* cur_node = rete->elaboratori;
    struct node* last_node = rete->elaboratori;
    while(cur_node) {
        cur_node = last_node->next;
        free(last_node);
        last_node = cur_node;
    }

    struct arco* cur_arc = rete->archi;
    struct arco* last_arc = rete->archi;
    while(cur_arc) {
        cur_arc = last_arc->next;
        free(last_arc);
        last_arc = cur_arc;
    }

    free(rete);
}
