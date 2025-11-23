//
// Created by francesco on 11/21/25.
//

#ifndef PERSONAGGIO_H
#define PERSONAGGIO_H

#include "equipaggiamento.h"

#define MAX_BUF 256

typedef struct {
    int hp, mp, atk, def, mag, spr;
} pg_stats_t;

typedef struct {
    char name[MAX_STR], class[MAX_STR];
    unsigned int id;
    pg_stats_t stats;
    int inv[INVENTORY_SIZE];
} pg_t;

typedef struct pg_e{
   pg_t pg;
   struct pg_e* next;
} pg_elem_t;

typedef struct {
  pg_elem_t* head;
  pg_elem_t* tail;
  int n;
} pg_list_t;

void add_pg(pg_list_t* list, pg_t pg);

void remove_pg(pg_list_t* list, int id);

pg_t parse_pg(const char* line);

void parse_pg_file(const char* filename, pg_list_t* list);

void free_pg_list(pg_list_t* list);

pg_t* get_pg(pg_list_t* list, unsigned int id);

void print_pg_info(item_t* items, pg_t* pg);

void print_stats(pg_stats_t stats);

pg_stats_t calculate_stats(item_t* items, pg_t* pg);

#endif //PERSONAGGIO_H
