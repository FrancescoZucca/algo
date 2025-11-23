#include <stdio.h>

#include "personaggio.h"
#include "equipaggiamento.h"

#include <stdlib.h>

#define FN_INV "lab6/input files/E3/inventario.txt"
#define FN_PGS "lab6/input files/E3/pg.txt"

int ask_id(pg_list_t list, item_t* items) {
    int id;
    pg_elem_t* cur = list.head;
    while (cur) {
        print_pg_info(items, &cur->pg);
        cur = cur->next;
    }
    printf("Inserisci l'ID del personaggio: PG<id>\n");
    scanf("%d", &id);
    return id;
}

int main(){
    int N;
    item_t* items = load_items(FN_INV, &N);

    pg_list_t list;
    parse_pg_file(FN_PGS, &list);

    int exit = 0, opt;
    while(!exit) {
        printf("Cosa vuoi fare?\n\t1- Aggiungi un personaggio\n\t2- Rimuovi un personaggio\n\t"
            "3- Modifica equipaggiamento a un personaggio\n\t4- Calcola le statistiche di un personaggio\n\t5- Esci\n> ");
        scanf("%d", &opt);
        int id;
        switch(opt) {
            case 1:
                printf("Scrivi le caratteristiche del personaggio:\n<PGXXXX> <Nome> <Classe> <hp> <mp> <atk> <def> <mag> <spr>\n");
                pg_t pg;
                scanf("%u %50s %50s %u %u %u %u %u %u", &pg.id, pg.name, pg.class, &pg.stats.hp,
                       &pg.stats.mp, &pg.stats.atk, &pg.stats.def, &pg.stats.mag, &pg.stats.spr);
                for (int i = 0; i < INVENTORY_SIZE; i++)
                    pg.inv[i] = -1;
                add_pg(&list, pg);
                break;
            case 2:
                remove_pg(&list, ask_id(list, items));
                break;
            case 3:
                id = ask_id(list, items);
                int item, slot;
                print_pg_info(items, get_pg(&list, id));
                printf("Inserisci lo slot da modificare, seguito dall'id dell'oggetto (-1 per svuotarlo):\n");
                scanf("%d %d", &slot, &item);
                if (slot > INVENTORY_SIZE || slot < 0) {printf("Invalid slot.\n"); break;}
                get_pg(&list, id)->inv[slot] = item;
                break;
            case 4:
                id = ask_id(list, items);
                print_stats(calculate_stats(items, get_pg(&list, id)));
                break;
            case 5:
                exit = 1;
                break;
            default:
                printf("Opzione invalida.");
        }
    }

    free_pg_list(&list);
    free(items);
}