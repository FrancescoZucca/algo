#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 256


typedef enum {
    ZAFFIRO,
    RUBINO,
    TOPAZIO,
    SMERALDO
} pietra_e;

static const char lettere[4] = {'Z', 'R', 'T', 'S'};

void check(pietra_e* sol, pietra_e* result, int* max, int n) {
    if (n > *max) {
        *max = n;
        for (int i = 0; i < n; i++) {
            result[i] = sol[i];
        }
    }
}

void lunghezza(pietra_e* collana, int cur, int* max, pietra_e* fin, int pietre[4], int total, int* flag) {

    static const int pa[2] = {ZAFFIRO, SMERALDO};
    static const int pb[2] = {RUBINO, TOPAZIO};

    pietra_e pietra_a = pa[collana[cur-1]%2];
    pietra_e pietra_b = pb[collana[cur-1]%2];

    if (cur == total) {
        *flag = 1;
        *max = total;
        return;
    }

    if (pietre[pietra_a] > 0) {
        pietre[pietra_a]--;
        collana[cur] = pietra_a;
        lunghezza(collana, cur+1, max, fin, pietre, total, flag);
        if (flag) return;
        pietre[pietra_a]++;
    }
    if (pietre[pietra_b] > 0) {
        pietre[pietra_b]--;
        collana[cur] = pietra_b;
        lunghezza(collana, cur+1, max, fin, pietre, total, flag);
        if (flag) return;
        pietre[pietra_b]++;
    }
    if (!pietre[pietra_a] && !pietre[pietra_b]) {
        check(collana, fin, max, cur);
    }
}

int main() {
    int z, r, t, s;
    printf("Inserisci il numero di zaffiri, rubini, topazi e smeraldi:\n(<z> <r> <t> <s>): ");
    scanf("%d %d %d %d", &z, &r, &t, &s);

    int pietre[4] = {z,r,t,s};
    int total = z+r+t+s;
    pietra_e* collana = malloc(total * sizeof(pietra_e));
    pietra_e* result = malloc(total * sizeof(pietra_e));
    int max = 0, flag = 0;

    for (int i = 0; i < 4; i++) {
        if (pietre[i]) {
            pietre[i]--;
            *collana = i;
            lunghezza(collana, 1, &max, result, pietre, total, &flag);
            pietre[i]++;
        }
    }

    printf("%d\n\n", max);
    for (int i = 0; i < max; i++) {
        printf("%c", lettere[collana[i]]);
    }

    printf("\n");

    free(result);
    free(collana);
}
