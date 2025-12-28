#pragma once

#include <stdio.h>
#include "date.h"
#include <stdlib.h>

#define MAX_NAME 21

typedef struct titolo_s* titolo_t;
typedef struct lista_titoli_s* lista_titoli_t;

titolo_t titolo_init();
void titolo_parse(FILE* fp, titolo_t titolo);
void titolo_free(titolo_t titolo);
double titolo_value(datetime_t date, titolo_t titolo);

lista_titoli_t listaTitoli_init();
void listaTitoli_free(lista_titoli_t lista);
int listaTitoli_parse(FILE* fp, lista_titoli_t lista);
void listaTitoli_insert(titolo_t titolo, lista_titoli_t lista);
titolo_t listaTitoli_search(char* name, lista_titoli_t lista);

