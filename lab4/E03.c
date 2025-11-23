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

void powerset(int pos, pietra_e* collana, int pietre[4], int* max_l, pietra_e* max_sol, int total, int* flag){
    // choices[pietra % 2] = scelta a, choices[2 + pietra % 2] scelta b
    static const pietra_e choices[4] = {ZAFFIRO, SMERALDO, RUBINO, TOPAZIO};

    pietra_e pietra_a = choices[collana[pos-1]%2];
    pietra_e pietra_b = choices[2 + collana[pos-1]%2];

    int sum = pos;
    for (int i = 0; i < 4; i++) sum += pietre[i];
    if (sum < *max_l) return;

    if (!pietre[pietra_a] && !pietre[pietra_b]) {
        if (pos > *max_l){
            *max_l = pos;
            for (int i = 0; i < pos; i++) {
                max_sol[i] = collana[i];
            }
            if (pos == total) *flag = 1;
        }
        return;
    }

    if(pietre[pietra_a] && !*flag) {
        collana[pos] = pietra_a;
        pietre[pietra_a]--;
        powerset(pos+1, collana, pietre, max_l, max_sol, total, flag);
        pietre[pietra_a]++;
    }

    if(pietre[pietra_b] && !*flag) {
        collana[pos] = pietra_b;
        pietre[pietra_b]--;
        powerset(pos+1, collana, pietre, max_l, max_sol, total, flag);
        pietre[pietra_b]++;
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
            powerset(1, collana, pietre, &max, result, total, &flag);
            pietre[i]++;
        }
    }

    printf("%d\n\n", max);
    for (int i = 0; i < max; i++) {
        printf("%c", lettere[result[i]]);
    }

    printf("\n");

    free(result);
    free(collana);
}
