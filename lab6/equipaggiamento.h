//
// Created by francesco on 11/21/25.
//

#ifndef EQUIPAGGIAMENTO_H
#define EQUIPAGGIAMENTO_H

#define MAX_STR 51
#define INVENTORY_SIZE 8

typedef struct {
    char nome[MAX_STR], tipo[MAX_STR];
    int hp, mp, atk, def, mag, spr;
} item_t;

item_t* load_items(const char* filename, int* N);
int search_item(item_t* items, int N, const char* nome);
void print_item_info(item_t* item);

#endif //EQUIPAGGIAMENTO_H
