#include <stdio.h>
#include <stdlib.h>

#define FILENAME_B "lab5/board.txt"
#define FILENAME_T "lab5/tiles.txt"

typedef struct {
    char colore_o, colore_v;
    int punti_o, punti_v;
    char ruotata;
} tessera_t;

int punteggio(tessera_t* tessere, int** scacchiera, int R, int C) {
    int totale = 0;
    for (int r = 0; r < R; r++) {
        tessera_t cur = tessere[scacchiera[r][0]];
        char colore = cur.ruotata ? cur.colore_v : cur.colore_o;
        int punti = cur.ruotata ? cur.punti_v : cur.punti_o, c;
        for (c = 1; c < C && ((tessere[scacchiera[r][c]].ruotata ? tessere[scacchiera[r][c]].colore_v : tessere[scacchiera[r][c]].colore_o) == colore); c++) {
            punti += tessere[scacchiera[r][c]].ruotata ? tessere[scacchiera[r][c]].punti_v : tessere[scacchiera[r][c]].punti_o;
        }

        if (c == C)
            totale += punti;
    }

    for (int c = 0; c < C; c++) {
        tessera_t cur = tessere[scacchiera[0][c]];
        char colore = cur.ruotata ? cur.colore_o : cur.colore_v;
        int punti = cur.ruotata ? cur.punti_o : cur.punti_v;
        int r;
        for (r = 1; r < R && ((tessere[scacchiera[r][c]].ruotata ? tessere[scacchiera[r][c]].colore_o : tessere[scacchiera[r][c]].colore_v) == colore); r++) {
            punti += tessere[scacchiera[r][c]].ruotata ? tessere[scacchiera[r][c]].punti_o : tessere[scacchiera[r][c]].punti_v;
        }

        if (r == R)
            totale += punti;
    }


    return totale;
}

void powerset(int pos, int** scacchiera, int T, int R, int C, tessera_t* tessere, int* mark, int* max, int* sol, char* rotations) {
    if (pos >= R*C) {
        int punti = punteggio(tessere, scacchiera, R, C);
        if (punti > *max) {
            *max = punti;
            for (int i = 0; i < R; i++) {
                for (int j = 0; j < C; j++) {
                    sol[i*C + j] = scacchiera[i][j];
                    rotations[i*C + j] = tessere[scacchiera[i][j]].ruotata;
                }
            }
        }
        return;
    }

    if (scacchiera[pos/C][pos % C] != -1) {
        powerset(pos+1, scacchiera, T, R, C, tessere, mark, max, sol, rotations);
    }
    else {
        for (int i = 0; i < T; i++) {
            if (!mark[i]) {
                mark[i] = 1;
                scacchiera[pos/C][pos % C] = i;
                tessere[i].ruotata = 1;
                powerset(pos+1, scacchiera, T, R, C, tessere, mark, max, sol, rotations);
                tessere[i].ruotata = 0;
                powerset(pos+1, scacchiera, T, R, C, tessere, mark, max, sol, rotations);
                mark[i] = 0;
                scacchiera[pos/C][pos % C] = -1;
            }
        }
    }
}

void find_opt(int** scacchiera, tessera_t* tessere, int* mark, int T, int R, int C) {
    int max = -1;
    int* sol = malloc(sizeof(int) * R*C);
    char* rotations = malloc(sizeof(char)*R*C);

    powerset(0, scacchiera, T, R, C, tessere, mark, &max, sol, rotations);

    printf("%d\n", max);

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            printf("%d/%d ", sol[i*C + j], rotations[i*C + j]);
        }
        printf("\n");
    }

    free(sol);
}

tessera_t* parseTiles(const char* filename, int* T) {
    FILE* f = fopen(filename, "r");

    fscanf(f, "%d", T);

    tessera_t* tessere = malloc(*T * sizeof(tessera_t));

    for (int i = 0; i < *T; i++)
        fscanf(f, " %c %d %c %d ", &tessere[i].colore_o, &tessere[i].punti_o, &tessere[i].colore_v, &tessere[i].punti_v);

    fclose(f);
    return tessere;
}

int** parseBoard(const char* filename, int* R, int* C, tessera_t* tessere, int* mark) {
    FILE* f = fopen(filename, "r");

    fscanf(f, "%d %d", R, C);

    int** board = malloc(*R * sizeof(int*));

    for (int i = 0; i < *R; i++) {
        board[i] = malloc(*C * sizeof(int));
        for (int j = 0; j < *C; j++) {
            int ruotata;
            fscanf(f, "%d/%d", &board[i][j], &ruotata);
            if (board[i][j] != -1) {
                tessere[board[i][j]].ruotata = ruotata;
                mark[board[i][j]] = 1;
            }
        }
    }

    fclose(f);
    return board;
}

#define TILES_FILE "lab5/tiles.txt"
#define BOARD_FILE "lab5/board.txt"

int main() {
    int T, R, C;
    tessera_t* tessere = parseTiles(TILES_FILE, &T);
    int* mark = calloc(T, sizeof(int));
    int** board = parseBoard(BOARD_FILE, &R, &C, tessere, mark);

    find_opt(board, tessere, mark, T, R, C);

    free(tessere);
    for (int i = 0; i < R; i++) free(board[i]);
    free(board);
    free(mark);
}
