

#include <stdio.h>

int countOccurrences(int* A, int candidate, int l, int r) {
    int count = 0;
    for (int i = l; i <= r; i++) {
        if (A[i] == candidate) {
            count++;
        }
    }

    return count;
}

int majorityR(int* A, int l, int r) {
    if (r-l == 0) return A[l]; // se il vettore ha un solo elemento, è necessariamente l'elemento maggioritario

    int mid = l + (r-l)/2; // divido il vettore in due sottovettori e cerco il loro maggioritario
    int a = majorityR(A, l, mid);
    int b = majorityR(A, mid+1, r);
    if (a != b) { // se abbiamo due sottoarray con elementi maggioritari diversi bisogna contare quante volte compaiono
        if (a != -1 && countOccurrences(A, a, l, r) > (r-l)/2) // saltiamo il check se non ci sono maggioritari nel primo sottovettore
            return a;
        if (b != -1 && countOccurrences(A, b, l, r) > (r-l)/2) // idem per il secondo sottovettore
            return b;
        return -1;
    }
    return a; // se sono uguali saltiamo il check e ritorniamo direttamente il maggioritario trovato (che può essere anche nessuno)
}

// trova l'elemento maggioritario (che compare > n/2 volte) nell'array.
// in caso di occorrenza uguale a n/2 volte sto considerando come nessun elemento maggioritario
int majority(int* A, int n){
    int l = 0, r = n-1;
    return majorityR(A, l, r);
}

int main(int argc, char *argv[]) {
    int array[] = {3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 3};
    constexpr int n = sizeof(array)/sizeof(array[0]);

    printf("%d\n", majority(array, n));
}