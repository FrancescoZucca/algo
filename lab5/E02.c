#include <stdio.h>
#include <stdlib.h>

#define FILENAME_B "lab5/board.txt"
#define FILENAME_T "lab5/tiles.txt"

typedef struct {
    char colore_o, colore_v;
    int punti_o, punti_v, usabile;
} tessera_t;

void ruota(tessera_t* t) {
    int p = t-> punti_o;
    char c = t -> colore_o;

    t -> colore_o = t -> colore_v;
    t -> colore_v = c;

    t -> punti_o = t -> punti_v;
    t -> punti_v = p;
}

void check(tessera_t* pedine ,int** scacchiera, tessera_t* res, int R, int C, int *max) {
    int totale = 0;
    char *colori_v = malloc(C * sizeof(char));
    int *punti_v = malloc(C * sizeof(int));

    for (int i = 0; i < R; i++) {
        char colore_o = pedine[scacchiera[i][0]].colore_o;
        int subtotale_o = 0;
        for (int j = 0; j < C; j++) {

            if (i == 0) { // inizializzazione del vettore colori verticali
                colori_v[j] = pedine[scacchiera[i][j]].colore_v;
                punti_v[j] = pedine[scacchiera[i][j]].punti_v;
            } else if (colori_v[j] != 0) {
                if (pedine[scacchiera[i][j]].colore_v != colori_v[j]) {
                    colori_v[j] = 0;
                    punti_v[j] = 0;
                } else {
                    punti_v[j] += pedine[scacchiera[i][j]].punti_v;
                }
            }

            if (i == R-1) {
                totale += punti_v[j];
            }

            if (pedine[scacchiera[i][j]].colore_o != colore_o) {
                subtotale_o = 0;
                colore_o = 0;
            } else if (colore_o != 0)
                subtotale_o += pedine[scacchiera[i][j]].punti_o;
        }

        totale += subtotale_o;
    }

    if (totale > *max) {
        *max = totale;
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                res[i*C+j] = pedine[scacchiera[i][j]];
            }
        }
    }

    free(colori_v);
    free(punti_v);
}

void chooseR(tessera_t* pedine ,int** scacchiera, tessera_t* res, int pos, int R, int C, int T, int *max) {
    if (pos == R*C) {
        check(pedine, scacchiera, res, R, C, max);
        return;
    }

    if (scacchiera[pos/C][pos%C] != -1) { // se questo spazio è già stato riempito
        chooseR(pedine, scacchiera, res, pos+1, R, C, T, max);
    } else {
        for (int i = 0; i < T; i++) {
            if (pedine[i].usabile) {
                pedine[i].usabile = 0;
                scacchiera[pos/C][pos%C] = i;
                chooseR(pedine, scacchiera, res, pos+1, R, C, T, max);
                ruota(&pedine[i]);
                chooseR(pedine, scacchiera, res, pos+1, R, C, T, max);
                pedine[i].usabile = 1;
            }
        }
    }
}

void choose(tessera_t* pedine, int T, int** scacchiera, int R, int C) {
    tessera_t* res = malloc(R*C * sizeof(tessera_t));
    int max = 0;

    chooseR(pedine, scacchiera, res, 0, R, C, T, &max);

    printf("Massimo punti ottenibili: %d\n", max);
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            printf("%c%d%c%d ", res[i*C+j].colore_o, res[i*C+j].punti_o, res[i*C+j].colore_v, res[i*C+j].punti_v);
        }
        printf("\n");
    }

    free(res);
}

int main() {
    FILE* t_fin = fopen(FILENAME_T, "r");

    int T;
    fscanf(t_fin, "%d", &T);

    tessera_t* pedine = malloc(T * sizeof(tessera_t));
    for (int i = 0; i < T; i++) {
        fscanf(t_fin, " %c %d %c %d", &pedine[i].colore_o, &pedine[i].punti_o, &pedine[i].colore_v, &pedine[i].punti_v);
        pedine[i].usabile = 1;
    }

    FILE* b_fin = fopen(FILENAME_B, "r");

    int R, C, ruotato;
    fscanf(b_fin, "%d %d", &R, &C);

    int** scacchiera = malloc(R * sizeof(int*));
    for (int i = 0; i < R; i++) {
        scacchiera[i] = malloc(C * sizeof(int));
        for (int j = 0; j < C; j++) {
            fscanf(b_fin, "%d/%d", &scacchiera[i][j], &ruotato);
            if (scacchiera[i][j] != -1)
                pedine[scacchiera[i][j]].usabile = 0;
            if (ruotato)
                ruota(&pedine[scacchiera[i][j]]);
        }
    }

    free(t_fin);
    free(b_fin);

    choose(pedine, T, scacchiera, R, C);

    free(pedine);
    for (int i = 0; i < R; i++) {
        free(scacchiera[i]);
    }
    free(scacchiera);
}
