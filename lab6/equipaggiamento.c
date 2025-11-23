//
// Created by francesco on 11/21/25.
//

#include "equipaggiamento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

item_t* load_items(const char* filename, int* N) {
    FILE* f = fopen(filename, "r");

    fscanf(f, "%u", N);
    item_t* items = malloc(sizeof(item_t) * *N);

    for (int i = 0; i < *N; i++) {
        fscanf(f, "%50s %50s %d %d %d %d %d %d", items[i].nome, items[i].tipo, &items[i].hp,
             &items[i].mp, &items[i].atk, &items[i].def, &items[i].mag, &items[i].spr);
    }

    fclose(f);
    return items;
}

int search_item(item_t* items, int N, const char* nome) {
    for (int i = 0; i < N; i++) {
    	if (!strcmp(items[i].nome, nome)) return i;
    }

    return -1;
}

void print_item_info(item_t* item) {

    printf("%s - %s\n\t- HP: %d MP: %d ATK: %d DEF: %d MAG: %d SPR: %d\n",
         item->nome, item->tipo, item->hp, item->mp, item->atk, item->def, item->mag, item->spr);
}
