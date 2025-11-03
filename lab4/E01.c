#include <stdio.h>
#include <stdlib.h>

typedef struct cache_entry {
    unsigned long long int mask;
    struct cache_entry* next;
} cache_entry_t;

// controlla se abbiamo già un'entry per la mask, se no la aggiungiamo
int checkAndAddEntry(struct cache_entry* list, unsigned long long int mask) {

    if (list->mask == 0)
        list->mask = mask;
    else {
        while (list->next) {
            if (list->next->mask == mask)
                return 1;
            list = list->next;
        }
        list->next = malloc(sizeof(cache_entry_t));
        list->next->mask = mask;
        list->next->next = NULL;
    }

    return 0;
}

int getBit(unsigned long long int mask, int occurrence) {
    int count = 0;
    while (occurrence > -1 && mask != 0) {
        while(!(mask & 1)) {
            mask >>= 1;
            count++;
        }
        mask >>= 1;
        count++;
        occurrence--;
    }
    return count-1;
}

unsigned long long int getMask(int vec) {
    return 1ull << vec;
}

int parseFile(unsigned long long int** vec, const char* filename, int* n, int* e){
    FILE *fin = fopen(filename, "r");
    if(fin == NULL){
      printf("Errore nell'apertura del file %s\n", filename);
      return -1;
    }

    fscanf(fin, "%d %d", n, e);
    *vec = calloc(*e, sizeof(unsigned long long int));

    for (int i = 0; i < *e; i++) {
        int a,b;
        fscanf(fin, "%d %d", &a, &b);
        (*vec)[i] |= getMask(a) | getMask(b);
    }

    fclose(fin);
    return 0;
}

int countEdges(cache_entry_t* cache, unsigned long long int* vec, unsigned long long int mask, int v, int e){

    int count = 0;

    if (checkAndAddEntry(cache, mask)) { // evitiamo di calcolare duplicati
        return 0;
    }

    if (v!=1) {
        for (int j = 0; j < v; j++) { // rimuovi un vertice per ottenere tutte le combinazioni di v-1 vertici
            unsigned long long int submask = mask ^ (1 << getBit(mask, j));
            int c = countEdges(cache, vec, submask, v-1, e);
            count = c > count ? count : c; // salva il massimo del count dei sottovettori
        }
    }

    if (count != e) { // non serve controllare, se un sottovettore è già vertex cover
        count = 0;
        for (int i = 0; i < e; i++)
            if ((vec[i] & mask) != 0) count++;
    }

    if (count == e) { // se tutti gli archi sono toccati dai vertici considerati, i.e. se abbiamo una vertex cover
        printf("(");
        for (int i = 0; i < v-1; i++) {
            printf("%d,", getBit(mask, i));
        }
        printf("%d) ", getBit(mask, v-1));
    }

    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename.txt>\n", argv[0]);
        return -1;
    }

    unsigned long long int* vec = NULL;
    int n, e;
    if (parseFile(&vec, argv[1], &n, &e))
        return -2;

    if (n > sizeof(unsigned long long int) * 8) {
        printf("Troppi vertici! Il massimo è %lu\n", sizeof(unsigned long long int) * 8);
    }

    // utilizzo ogni bit come mask per un vertice. così otteniamo, nel caso di 5 vertici
    // (1 << 5) - 1 = 11111, operando quindi su tutti i vertici.
    unsigned long long int mask = (1 << n) - 1;

    // necessaria per evitare di calcolare e stampare inutilmente sottovettori già considerati
    cache_entry_t head;
    head.mask = 0;
    head.next = NULL;

    countEdges(&head, vec, mask, n, e);
    printf("\n");

    // free di tutte le strutture
    cache_entry_t* list = head.next;
    while (list != NULL) {
        cache_entry_t* tmp = list;
        list = list->next;
        free(tmp);
    }
    free(vec);

    return 0;
}