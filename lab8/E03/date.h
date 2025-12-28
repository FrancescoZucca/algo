#pragma once

#include <stdio.h>

typedef struct {
    int year, month, day, hour, minute;
} datetime_t;

int date_compare(datetime_t d1, datetime_t d2, int onlyCompareDay);
void parse_date(FILE* fp, datetime_t* date, int onlyDate);