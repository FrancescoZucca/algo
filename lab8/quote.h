#include "date.h"

typedef struct {
    datetime_t date;
    double price;
} quote_t;

typedef struct quote_list* quote_list_t;
void QL_insert(quote_list_t list, quote_t quote);