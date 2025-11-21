#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int i, f;
} attivita_t;

void check(attivita_t* V, int* sel, int* res, int pos, int* max, int* p_pos) {
    int ret = 0;
    for (int i = 0; i < pos; i++) {
        ret += V[i].f - V[i].i;
    }
    if (ret > *max) {
        *max = ret;
        *p_pos = pos;
        for (int i = 0; i < pos; i++) {
            res[i] = sel[i];
        }
    }
}

int compatible(attivita_t* V, int* sel, int pos, attivita_t a) {
    for (int i = 0; i < pos; i++) {
        if (!(V[sel[i]].i < a.f && a.i < V[sel[i]].f)) return 0;
    }

    return 1;
}

void attSelR(int N, attivita_t* V, int pos, int* sel, int* res, int* max, int* p_pos) {
    if (N == 0)
        return check(V, sel, res, pos, max, p_pos);

    int flag = 1;
    for (int i = 0; i < N; i++) {
        if (compatible(V, sel, pos, V[i])) {
            sel[pos] = i;
            attSelR(N-1, V, pos, sel, res, max, p_pos);
            flag = 0;
        }
    }

    if (!flag) check(V, sel, res, pos, max, p_pos);
}

void attSel(int N, attivita_t* V) {
    int* sel = malloc(sizeof(int) * N);
    int* res = malloc(sizeof(int) * N);
    int max = 0, pos = 0;
    for (int i = 0; i < N; i++) {
        *sel = i;
        attSelR(N-1, V, 1, sel, res, &max, &pos);
    }
    printf("(");
    for (int i = 0; i < pos; i++) {
        printf("(%d,%d)", V[sel[i]].i, V[sel[i]].f);
    }
    free(sel);
    free(res);
}

int main() {
    const int inizi[] = {1, 2, 2, 3, 5, 6};
    const int fini[] = {2, 4, 4, 5, 7, 8};
    attivita_t V[6];
    for (int i = 0; i < 6; i++) {
        V[i].i = inizi[i];
        V[i].f = fini[i];
    }
    attSel(6, V);
    return 0;
}