#define MAX_STR 51
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum comando_e {
    e_invalid,
    e_add,
    e_remove,
    e_search,
    e_print,
    e_quit
};

typedef struct {
    char codice[6], nome[MAX_STR], cognome[MAX_STR], via[MAX_STR], citta[MAX_STR];
    int anno, mese, giorno, cap;
} item_t;

typedef struct entry {
    item_t item;
    struct entry* next;
} entry_t;

void populateEntry(entry_t* new, char codice[6], char nome[MAX_STR], char cognome[MAX_STR], char via[MAX_STR], char citta[MAX_STR], char data[11], int cap) {
    strcpy(new->item.codice, codice);
    strcpy(new->item.nome, nome);
    strcpy(new->item.cognome, cognome);
    strcpy(new->item.via, via);
    strcpy(new->item.citta, citta);
    new->next = NULL;
    new->item.cap = cap;
    sscanf(data, "%d/%d/%d", &new->item.giorno, &new->item.mese, &new->item.anno);
}

void addToList(entry_t** list, entry_t* new) {
    if (*list == NULL) *list = new;
    else {
        entry_t* current = *list, *parent = NULL;
        while (current != NULL && new->item.anno > current->item.anno) {
            parent = current;
            current = current->next;
        }
        while (current != NULL && new->item.anno == current->item.anno && new->item.mese > current->item.mese) {
            parent = current;
            current = current->next;
        }
        while (current != NULL && new->item.anno == current->item.anno && new->item.mese == current->item.mese && new->item.giorno > current->item.giorno) {
            parent = current;
            current = current->next;
        }


        if (parent == NULL) { // metti in testa alla lista
            new->next = current;
            *list = new;
        } else if (current == NULL) { // metti in coda alla lista
            new->next = parent->next;
            parent->next = new;
        } else { // inserisci fra parent e current
            new->next = current;
            parent->next = new;
        }
    }
}

int parseFile(char* filename, entry_t** list) {
    FILE* fin = fopen(filename, "r");
    char codice[6], nome[MAX_STR], cognome[MAX_STR], via[MAX_STR], citta[MAX_STR], data[11];
    int cap;
    if (fin == NULL) return -1;
    while (fscanf(fin, "%5s %50s %50s %10s %50s %50s %05d", codice, nome, cognome, data, via, citta, &cap) == 7) {
        entry_t* new = malloc(sizeof(entry_t));
        populateEntry(new, codice, nome, cognome, via, citta, data, cap);
        addToList(list, new);
    }
    fclose(fin);
    return 0;
}

void freeList(entry_t* list) {
    while (list != NULL) {
        entry_t* temp = list->next;
        free(list);
        list = temp;
    }
}

void printInfo(item_t item, FILE* fout, int raw) {
    if (raw)
        fprintf(fout, "%s %s %s %02d/%02d/%02d %s %s %05d\n", item.codice, item.nome, item.cognome, item.giorno, item.mese, item.anno, item.via, item.citta, item.cap);
    else
        fprintf(fout, "%5s\t%s\t%s\tNato il %02d/%02d/%02d\tResidente in %20s a %s \t%05d\n", item.codice, item.nome, item.cognome, item.giorno, item.mese, item.anno, item.via, item.citta, item.cap);
}

void printAllInfo(entry_t* list, FILE* fout, int raw) {
    while (list != NULL) {
        printInfo(list->item, fout, raw);
        list = list->next;
    }
}

entry_t* searchEntry(entry_t** list, char* codice, bool remove) {
    entry_t* temp = NULL;
    entry_t* current = *list;
    entry_t* previous = *list;
    while (current != NULL && temp == NULL) {

        if (strcmp(current->item.codice, codice) == 0) {

            if (current == *list) {
                temp = current;
                if (remove)
                    *list = temp->next;
            } else {
                temp = current;
                if (remove)
                    previous->next = current->next->next;
            }
        }
        previous = current;
        current = current->next;
    }
    return temp;
}

entry_t* removeInterval(entry_t** list, int giorno1, int mese1, int anno1, int giorno2, int mese2, int anno2) {
    entry_t* current = *list, *temp = NULL;
    while (anno1 > current->item.anno && current->next != NULL)
        current = current->next;
    while (mese1 > current->item.mese && current->next != NULL)
        current = current->next;
    while (giorno1 > current->item.giorno && current->next != NULL)
        current = current->next;

    if (current->next == NULL) {
        return NULL;
    }

    if (current->item.anno < anno2) {
        if (current == *list) {
            temp = current;
            *list = temp->next;
        } else {
            temp = current->next;
            current->next = current->next->next;
        }
        return temp;
    }
    if (current->item.mese < mese2) {
        if (current == *list) {
            temp = current;
            *list = temp->next;
        } else {
            temp = current->next;
            current->next = current->next->next;
        }
        return temp;
    }
    if (current->item.giorno < giorno2) {
        if (current == *list) {
            temp = current;
            *list = temp->next;
        } else {
            temp = current->next;
            current->next = current->next->next;
        }
        return temp;
    }

    return temp;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <anagrafica.txt>\n", argv[0]);
    }

    entry_t* list = NULL;
    parseFile(argv[1], &list);
    printAllInfo(list, stdout, false);

    int option = e_invalid, opt;
    entry_t* ret;
    while (option != e_quit) {
        printf("Inserisci comando: \n- 1)\tAggiungi entrata\n- 2)\tRimuovi Entrata\n- 3)\tRicerca Entrata\n- 4)\tStampa Entrate\n- 5)\tEsci\n> ");
        scanf("%d", &option);

        switch (option) {
            case e_invalid:
                printf("Comando non riconosciuto.\n");
                scanf("%s");
            break;
            case e_add:
                opt = -1;
            printf("Vuoi acquisire da tastiera (1), o da file (2)?");
            scanf("%d", &opt);
            switch (opt) {
                case 1:
                    printf("Inserisci i dati e premi invio:\n<codice> <nome> <cognome> <data_di_nascita gg/mm/yyyy> <via> <citta'> <cap>\n> ");
                    char codice[6], nome[MAX_STR], cognome[MAX_STR], via[MAX_STR], citta[MAX_STR], data[11];
                    int cap;
                    scanf("%5s %50s %50s %10s %50s %50s %05d", codice, nome, cognome, data, via, citta, &cap);
                    entry_t* new = malloc(sizeof(entry_t));
                    populateEntry(new, codice, nome, cognome, via, citta, data, cap);
                    addToList(&list, new);
                    printf("- Entrata aggiunta. \n");
                break;
                case 2:
                    char input[128];
                    printf("Inserisci il nome del file: ");
                    scanf("%127s", input);
                    if (!parseFile(input, &list))
                        printf("Entrate aggiunte.\n");
                    else
                        printf("Errore nell'apertura del file %s", input);
                break;
            }
            break;
            case e_search:
                char input[6];
                printf("Inserisci il codice da cercare: ");
                scanf("%5s", input);
                ret = searchEntry(&list, input, false);
                if (ret == NULL)
                    printf("Nessun entrata trovata.\n");
                else {
                    printInfo(ret->item, stdout, false);
                }
            break;
            case e_remove:
                opt = -1;
                printf("Vuoi rimuovere un'entrata (1), o piu' entrate per intervallo di date (2)?");
                scanf("%d", &opt);
                switch (opt) {
                    case 1:
                        char input[6];
                        printf("Inserisci il codice da cercare: ");
                        scanf("%5s", input);
                        ret = searchEntry(&list, input, true);
                        if (ret == NULL)
                            printf("Nessun entrata trovata.\n");
                        else {
                            printInfo(ret->item, stdout, false);
                            free(ret);
                        }
                    break;
                    case 2:
                        int anno1, anno2, mese1, mese2, giorno1, giorno2;
                        printf("Inserisci l'intervallo di date (gg/mm/yyyy gg/mm/yyyy): ");
                        scanf("%d/%d/%d %d/%d/%d", &giorno1, &mese1, &anno1, &giorno2, &mese2, &anno2);
                        while ((ret = removeInterval(&list, giorno1, mese1, anno1, giorno2, mese2, anno2)) != NULL) {
                            printInfo(ret->item, stdout, false);
                            free(ret);
                        }
                    break;
                    default:
                        printf("Comando non riconosciuto.\n");
                        option = e_invalid;
                    break;
                }
            break;
            case e_print:
                opt = -1;
                printf("Vuoi stampare a video (1), o su file (2)?");
                scanf("%d", &opt);
                switch (opt) {
                    case 1:
                        printAllInfo(list, stdout, false);
                    break;
                    case 2:
                        char input[128];
                        printf("Inserisci il nome del file: ");
                        scanf("%127s", input);
                        FILE* fout = fopen(input, "w+");
                        if (fout == NULL)
                            printf("Errore nell'apertura del file %s", input);
                        printAllInfo(list, fout, true);
                        fclose(fout);
                    break;
                }
            break;
        }
        option = option == e_quit ? e_quit : e_invalid;
    }

    freeList(list);
}