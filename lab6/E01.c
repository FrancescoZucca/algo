#include <stdio.h>
#include <stdlib.h>

#define FILENAME "lab6/input files/E1/att3.txt"

typedef struct {
    int i, f;
} att_t;

int parseFile(const char* filename, att_t** list) {
    FILE* fin = fopen(filename, "r");

    int n;
    fscanf(fin, "%d", &n);

    *list = malloc(sizeof(att_t) * n);

    for (int i = 0; i < n; i++) {
        fscanf(fin, "%d %d", &(*list)[i].i, &(*list)[i].f);
    }

    fclose(fin);
    return n;
}

void countingSort(att_t* list, const int N, att_t* result, int exp) {
    int c[10] = {0};
    for (int i = 0; i < N; i++)
        c[list[i].i / exp % 10]++;
    for (int i = 1; i < 10; i++) c[i] += c[i-1];

    for (int i = N-1; i >= 0; i--) result[--c[list[i].i / exp % 10]] = list[i];
}

void orderList(att_t* list, const int N) {
    int exp = 1, max = list[0].i;
    for (int i = 1; i < N; i++) if (list[i].i > max) max = list[i].i;
    while (exp <= max) exp *= 10;
    att_t* tmp = malloc(sizeof(att_t) * N);

    for (int i = 1; i < exp; i*=10) {
        countingSort(list, N, tmp, i);
        for (int j = 0; j < N; j++) list[j] = tmp[j];
    }
    free(tmp);
}

void attSelPrint(att_t* list, int* P, int i) {
    if (P[i] == -1) {
        printf("[(%d %d), ", list[i].i, list[i].f);
        return;
    }

    attSelPrint(list, P, P[i]);
    printf("(%d %d), ", list[i].i, list[i].f);
}

void attSelDP(att_t* list, const int N) {
    int* L = malloc(sizeof(int) * N);
    int* P = malloc(sizeof(int) * N);
    int max = 1, last = 1;

    L[0] = list->f - list->i; P[0] = -1;
    for (int i = 1; i < N; i++) {
        L[i] = list[i].f - list[i].i; P[i] = -1;
        for (int j = 0; j < i; j++) {
            if (list[j].f <= list[i].i && L[i] < L[j] + list[i].f - list[i].i) {
                L[i] = L[j] + list[i].f - list[i].i;
                P[i] = j;
            }
        }
        if (L[i] > max) {
            max = L[i];
            last = i;
        }
    }

    attSelPrint(list, P, last);
    printf("]\nDurata: %d", max);

    free(L);
    free(P);
}

int main() {
    att_t* list;

    int n = parseFile(FILENAME, &list);
    orderList(list, n);

    attSelDP(list, n);

    free(list);
}