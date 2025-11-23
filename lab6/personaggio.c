#include <stdio.h>
#include <stdlib.h>
#include "personaggio.h"

void add_pg(pg_list_t* list, pg_t pg) {
    if (list->head == NULL) {
        list->head = malloc(sizeof(pg_elem_t));
        list->tail = list->head;
    } else {
        list->tail->next = malloc(sizeof(pg_elem_t));
        list->tail = list->tail->next;
    }
    list->tail->pg = pg;
    list->tail->next = NULL;
    list->n++;
}

void remove_pg(pg_list_t* list, int i) {
    if (i < 0 || i >= list->n) return;
    pg_elem_t* cur = list->head;

    int j = 0;
    while (j!=i-1) {
        cur = cur->next;
        j++;
    }

    pg_elem_t* tmp = cur->next->next;
    free(cur->next);
    cur->next = tmp;
}

pg_t parse_pg(const char* line){
    pg_t pg;
    sscanf(line, "PG%u %50s %50s %u %u %u %u %u %u", &pg.id, pg.name, pg.class, &pg.stats.hp,
           &pg.stats.mp, &pg.stats.atk, &pg.stats.def, &pg.stats.mag, &pg.stats.spr);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        pg.inv[i] = -1;
    }
    return pg;
}

void parse_pg_file(const char* filename, pg_list_t* list) {
    FILE* fin = fopen(filename, "r");
    char buf[MAX_BUF];

    list->head = NULL;
    list->tail = NULL;
    list->n = 0;

    while (fgets(buf, MAX_BUF, fin)) {
        add_pg(list, parse_pg(buf));
    }

    fclose(fin);
}

void free_pg_list(pg_list_t* list) {
    pg_elem_t* cur = list->head;
    while (cur != NULL) {
        pg_elem_t* tmp = cur->next;
        free(cur);
        cur = tmp;
    }
}

pg_t* get_pg(pg_list_t* list, unsigned int id) {
    pg_elem_t* cur = list->head;
    while (cur != NULL) {
        if (cur->pg.id == id) return &cur->pg;
        cur = cur->next;
    }

    return NULL;
}

void print_pg_info(item_t* items, pg_t* pg) {
    printf("%s\t(ID: PG%04u)\n", pg->name, pg->id);
    print_stats(pg->stats);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (pg->inv[i] != -1) print_item_info(&items[pg->inv[i]]);
    }
}

void print_stats(pg_stats_t stats){
	  printf("\tHP:\t%u\tMP:\t%u\tATK:\t%u\tDEF:\t%u\tMAG:\t%u\tSPR:\t%u\n",
         stats.hp, stats.mp, stats.atk, stats.def, stats.mag, stats.spr);
}

pg_stats_t calculate_stats(item_t* items, pg_t* pg) {
    pg_stats_t stats = pg->stats;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (pg->inv[i] != -1) {
            item_t item = items[pg->inv[i]];

            stats.hp += item.hp;
            stats.mp += item.mp;
            stats.atk += item.atk;
            stats.def += item.def;
            stats.mag += item.mag;
            stats.spr += item.spr;
        }
    }

    if (stats.hp < 0) stats.hp = 0;
    if (stats.mp < 0) stats.mp = 0;
    if (stats.atk < 0) stats.atk = 0;
    if (stats.def < 0) stats.def = 0;
    if (stats.mag < 0) stats.mag = 0;
    if (stats.spr < 0) stats.spr = 0;

    return stats;
}
