#include <stdio.h>
#include "date.h"

int date_compare(datetime_t d1, datetime_t d2, int onlyCompareDay){
    if (d1.year > d2.year) return 1;
    if (d1.year < d2.year) return -1;

    if (d1.month > d2.month) return 1;
    if (d1.month < d2.month) return -1;

    if (d1.day > d2.day) return 1;
    if (d1.day < d2.day) return -1;

    if (onlyCompareDay) return 0;

    if (d1.hour > d2.hour) return 1;
    if (d1.hour < d2.hour) return -1;

    if (d1.minute > d2.minute) return 1;
    if (d1.minute < d2.minute) return -1;

    return 0;
}

void parse_date(FILE* fp, datetime_t* date, int onlyDate){
    if (onlyDate) {
        fscanf(fp, " %d/%d/%d", &date->year, &date->month, &date->day);
        date->hour = -1;
        date->minute = -1;
    } else
        fscanf(fp, " %d/%d/%d %d:%d", &date->year, &date->month, &date->day, &date->hour, &date->minute);
}