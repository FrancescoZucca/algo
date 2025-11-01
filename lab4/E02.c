#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int n;
    char songs[5][256];
} amico_t;

int parseFile(amico_t** amici, const char* filename) {
    FILE* fin = fopen(filename, "r");

    int A;
    fscanf(fin, "%d", &A);

    *amici = malloc(A * sizeof(amico_t));
    for (int i = 0; i < A; i++) {
        fscanf(fin, "%d", &(*amici)[i].n);
        for (int j = 0; j < (*amici)[i].n; j++) {
            fscanf(fin, "%255s", (*amici)[i].songs[j]);
        }
    }

    fclose(fin);
    return A;
}

int findSong(int pos, amico_t* vec, int* scelte, int n, int cnt) {

    if (pos >= n) {
        for (int i = 0; i < n; i++)
            printf("\t%s\n", vec[i].songs[scelte[i]]);
        printf("\n");
        return cnt + 1;
    }

    for (int i = 0; i < vec[pos].n; i++) {
        scelte[pos] = i;
        cnt = findSong(pos + 1, vec, scelte, n, cnt);
    }

    return cnt;
}

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        printf("Usage: %s <brani.txt>\n", argv[0]);
        return -1;
    }

    amico_t* amici;
    int A = parseFile(&amici, argv[1]);

    int* scelte = malloc(A * sizeof(int));
    printf("%d playlist trovate.\n", findSong(0, amici, scelte, A, 0));

    free(amici);
    free(scelte);
}