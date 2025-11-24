typedef struct {
    int year, month, day, hour, minute;
} datetime_t;

inline int compare_dates(datetime_t d1, datetime_t d2){
    if (d1.year > d2.year) return 1;
    if (d1.year < d2.year) return -1;

    if (d1.month > d2.month) return 1;
    if (d1.month < d2.month) return -1;

    if (d1.day > d2.day) return 1;
    if (d1.day < d2.day) return -1;

    if (d1.hour > d2.hour) return 1;
    if (d1.hour < d2.hour) return -1;

    if (d1.minute > d2.minute) return 1;
    if (d1.minute < d2.minute) return -1;

    return 0;
}