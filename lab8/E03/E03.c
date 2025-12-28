#include "titoli.h"
#include "date.h"
#include "quote.h"

#define N_OPZIONI 7

int main(int argc, char** argp){
    char* opzioni[N_OPZIONI] = {
        "Esci.",
        "Acquisisci contenuto da file.",
        "Seleziona un titolo.",
        "Ricerca la quotazione del titolo in una data.",
        "Ricerca la quotazione minima e massima in un intervallo di date.",
        "Ricerca la quotazione minima e massima del titolo.",
        "Bilancia l'albero di quotazioni se necessario."
    };

    titolo_t selezionato = NULL;
    int opt = -1;
    lista_titoli_t lista = listaTitoli_init();
    while(opt){
        printf("Scegli un'opzione:\n");
        for (int i = 0; i < (selezionato ? N_OPZIONI : 3); i++){
            printf("%d - %s\n", i, opzioni[i]);
        }
        scanf(" %d", &opt);
        switch(opt){
            case 0 : break;
            case 1 : {
                printf("Inserisci il nome del file: \n");
                char buffer[256];
                scanf("%255s", buffer);
                FILE* fp = fopen(buffer, "r");
                if (fp) {
                    printf("Sono stati caricati %d titoli.\n", listaTitoli_parse(fp, lista));
                    fclose(fp);
                } else
                    printf("File non trovato.\n");
            } break;
            case 2 : {
                printf("Inserisci il nome del titolo.\n");
                char buffer[MAX_NAME];
                scanf(" %20s", buffer);
                selezionato = listaTitoli_search(buffer, lista);
                if (selezionato) printf("Titolo selezionato.\n");
                else printf("Titolo non trovato.");
            } break;
            case 3 : {
                if (!selezionato) {printf("Necessario selezionare un titolo.\n"); break;}
                printf("Inserisci data: <aaaa/mm/gg>\n");
                datetime_t date;
                parse_date(stdin, &date, 1); 
                printf("Il valore e' %.02lf\n", titolo_value(date, selezionato));
            } break;
            default : {
                printf("Opzione non valida.");
            }
        }
    }

    listaTitoli_free(lista);
    
    return 0;
}