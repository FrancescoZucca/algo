#pragma once

#include "date.h"
#include <stdlib.h>

typedef struct {
    datetime_t date;
    double price;
    int weight;
} quote_t;

void quote_parse(FILE* fp, quote_t* quote);

typedef struct quote_list* quote_list_t;
quote_list_t QL_init();
void QL_free(quote_list_t list);
quote_t* QL_search(datetime_t date, quote_list_t list);
void QL_insert(quote_list_t list, quote_t quote);