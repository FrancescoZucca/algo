#include <stdio.h>
#include <stdlib.h>

#define FILENAME "lab5/att.txt"

typedef struct{
    int i, f;
} attivita_t;

void check(int* max, int* res, int* sel, attivita_t* vec, int N){
    int somma = 0;
    for(int i = 0; i < N; i++) {
        if (sel[i])
            somma += vec[i].f - vec[i].i;
    }
    if(somma > *max) {
        *max = somma;
        for(int i = 0; i < N; i++) {
            res[i] = sel[i];
        }
    }
}

int promising(int N, attivita_t* vec, int* sel, attivita_t a) {
    for (int i = 0; i < N; i++) {
        if (sel[i] && vec[i].i < a.f && a.i < vec[i].f) {
            return 0;
        }
    }
    return 1;
}

void attSelR(int N, attivita_t* V, int pos, int* res, int* sel, int* max){
    if (pos >= N) {
        check(max, res, sel, V, N);
        return;
    }

    sel[pos] = 0;
    attSelR(N, V, pos+1, res, sel, max);
    if (promising(pos, V, sel, V[pos])) {
        sel[pos] = 1;
        attSelR(N, V, pos+1, res, sel, max);
    }
}

void attSel(int N, attivita_t* V) {
    int max = 0;
    int* res = malloc(N * sizeof(int));
    int* sel = malloc(N * sizeof(int)); // 1 = selected, 0 = not selected

    attSelR(N, V, 0, res, sel, &max);

    printf("Somma durate: %d\n", max);
    for(int i = 0; i < N; i++) {
        if (res[i])
            printf("(%d, %d) ", V[i].i, V[i].f);
    }

    free(res);
    free(sel);
}

int main(){
    FILE* fin = fopen(FILENAME, "r");

    int N = 0;
    fscanf(fin, "%d", &N);
    attivita_t* V = malloc(N * sizeof(attivita_t));

    for(int i = 0; i < N; i++) {
        fscanf(fin, "%d %d", &V[i].i, &V[i].f);
    }

    fclose(fin);

    attSel(N, V);

    free(V);
}