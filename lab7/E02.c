#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DD 12
#define DP 28
#define FILENAME "elementi.txt"

typedef struct {
    char nome[101];
    char tipo, ingresso, uscita, precedenza, finale, difficolta; // non è necessario avere 4 byte per salvare questi numeri, si assume che difficolta < 256
    double valore;
} elemento_t;

typedef struct {
    elemento_t elementi[5];
    char n;
} diagonale_t;

elemento_t* parseFile(const char* filename, int* N) {
    FILE* fin = fopen(filename, "r");

    if (!fin){
        printf("File %s non trovato.\n", filename);
        return NULL;
    }

    fscanf(fin, "%d", N);

    elemento_t* ret = malloc(*N * sizeof(elemento_t));

    for (int i = 0; i < *N; i++) {
        fscanf(fin, " %100s %hhd %hhd %hhd %hhd %hhd %lf %hhd ", ret[i].nome, &ret[i].tipo, &ret[i].ingresso, &ret[i].uscita, 
                                &ret[i].precedenza, &ret[i].finale, &ret[i].valore, &ret[i].difficolta);
    }

    fclose(fin);

    return ret;
}

int check(diagonale_t diagonali[3], double* max, diagonale_t soluzione[3], int dd, int dp){

    double valore = 0;
    int difficolta = 0;
    for (int i = 0; i < 3; i++){
        double valore_d = 0;
        int difficolta_d = 0;
        for (char j = 0; j < diagonali[i].n; j++){
            valore_d += diagonali[i].elementi[j].valore;
            difficolta_d += diagonali[i].elementi[j].difficolta;
            if (i == 2 && j == diagonali[i].n-1 && diagonali[i].elementi[j].difficolta >= 8) valore_d *= 1.5;
        }

        if (difficolta_d > dd) return 1;

        valore += valore_d;
        difficolta += difficolta_d;
    }

    if (difficolta > dp) return 1;
    if (valore < *max) return 0;

    //printf("Trovato potenziale punteggio %lf maggiore del massimo corrente %lf\n", valore, *max);
    
    char seq = 0;
    char flag[3] = {0};
    for(int i = 0; i < 3; i++)
        for (int j = 0; j < diagonali[i].n; j++){
           if (j!= 0 && !seq && diagonali[i].elementi[j-1].tipo && diagonali[i].elementi[j].tipo) seq = 1;
           flag[i] |= diagonali[i].elementi[j].tipo;
        }
    if(!flag[0] || !flag[1] || !flag[2] || ((flag[0]|flag[1]|flag[2]) != 3)) return 0;
    if (!seq) return 0;

    *max = valore;
    for (int i = 0; i < 3; i++) {
        soluzione[i].n = diagonali[i].n;
        for (char j = 0; j < diagonali[i].n; j++)
            soluzione[i].elementi[j] = diagonali[i].elementi[j];
    }

    return 0;
}

void findopt_element(int pos, elemento_t* elementi, diagonale_t diagonali[3], int diag, int N, int difficolta, char ingresso,
                            int difficolta_tot, int dd, int dp, double* max, diagonale_t soluzione[3]){

    if (difficolta > dd || difficolta_tot > dp) return;
    
    if (diag == 2 && pos)
        if (check(diagonali, max, soluzione, dd, dp)) return; // check ritorna 1 solo in caso di errore di difficoltà troppo alta, impossibile da sistemare aggiungendo altri elementi
    if (diagonali[diag].n == 5 && diag == 2) return;

    if ((pos == 0 || !diagonali[diag].elementi[pos-1].finale) && diagonali[diag].n < 5)
        for (int i = 0; i < N; i++){
            //printf("Trying element %d...\n", i);
            if (elementi[i].ingresso == ingresso && (pos || !elementi[i].precedenza) && elementi[i].difficolta <= dd) {
                diagonali[diag].elementi[pos] = elementi[i];
                diagonali[diag].n++;
                //if (elementi[i].ingresso != elementi[i].uscita) printf("Mi giro con un/a %s\n", elementi[i].nome);
                findopt_element(pos+1, elementi, diagonali, diag, N, difficolta+elementi[i].difficolta, elementi[i].uscita, difficolta_tot+elementi[i].difficolta,
                                        dd, dp, max, soluzione);

                diagonali[diag].n--;
            }
        }

    if (diag < 2 && pos) findopt_element(0, elementi, diagonali, diag+1, N, 0, 1, difficolta_tot, dd, dp, max, soluzione);
}

void findopt(elemento_t* elementi, diagonale_t soluzione[3], int N, int dd, int dp) {
    diagonale_t diagonali[3];
    double max = -1;

    for (int i = 0; i < 3; i++){
        diagonali[i].n = 0;
        soluzione[i].n = 0;
    }
    
    findopt_element(0, elementi, diagonali, 0, N, 0, 1, 0, dd, dp, &max, soluzione);

    printf("Punteggio massimo: %lf\n", max);
}

int main() {
    printf("Inizio.. DD = %d; DP = %d\n", DD, DP);
    int N;
    elemento_t* elementi = parseFile(FILENAME, &N);

    if (!elementi) return -1;

    diagonale_t diagonali[3]; // 3 diagonali di massimo 5 elementi

    printf("Caricati %d elementi.\n", N);
    //for (int i = 0; i < N; i++) printf("%s %hhd %hhd %hhd %hhd %hhd %hhd %lf\n", elementi[i].nome, elementi[i].tipo, elementi[i].ingresso, elementi[i].uscita, 
    //                            elementi[i].precedenza, elementi[i].finale, elementi[i].difficolta, elementi[i].valore);
    findopt(elementi, diagonali, N, DD, DP);

    for (int i = 0; i < 3; i++){
        printf("Diagonale %d (%hhd elementi): \n", i+1, diagonali[i].n);
        for (char j = 0; j < diagonali[i].n; j++) {
            elemento_t e = diagonali[i].elementi[j];
            printf("-\t%s %hhd %hhd %hhd %hhd %hhd %hhd %lf\n", e.nome, e.tipo, e.ingresso, e.uscita, 
                                e.precedenza, e.finale, e.difficolta, e.valore);
        }
    }

    free(elementi);
}