#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CODE 7
#define MAX_STR 31
#define MAX_CORSE 1000

typedef struct {
	int giorno, mese, anno;
} data_t;

typedef struct {
	int ora, minuto, secondo;
} ora_t;

typedef struct {
	char codice[MAX_CODE], partenza[MAX_STR], arrivo[MAX_STR];
	data_t data;
	ora_t ora_p, ora_a;
	int ritardo;
} corsa_t;

enum comando_e {
	r_date,
	r_partenza,
	r_capolinea,
	r_ritardo,
	r_ritardo_tot,
	r_fine,
	r_order,
	r_invalid
};

void printInfo(corsa_t corsa) {
	printf("\t%7s\t%15s\t%15s\t%04d/%02d/%02d\t%02d:%02d -> %02d:%02d\t%2d minuti di ritardo\n",
		corsa.codice, corsa.partenza, corsa.arrivo, corsa.data.anno, corsa.data.mese, corsa.data.giorno,
		corsa.ora_p.ora, corsa.ora_p.minuto, corsa.ora_a.ora, corsa.ora_a.minuto, corsa.ritardo);
}

int leggiComando(){
	char buf[MAX_STR];
	printf("Inserisci comando:\n> ");
	scanf("%31s", buf);

	if (!strcmp(buf, "date")) return r_date;
	if (!strcmp(buf, "partenza")) return r_partenza;
	if (!strcmp(buf, "capolinea")) return r_capolinea;
	if (!strcmp(buf, "ritardo")) return r_ritardo;
	if (!strcmp(buf, "ritardo_tot")) return r_ritardo_tot;
	if (!strcmp(buf, "fine")) return r_fine;
	if (!strcmp(buf, "order")) return r_order;
	return r_invalid;
}

void parseDate(char* str, data_t* date){
	date->anno = atoi(strtok(str, "/"));
	date->mese = atoi(strtok(NULL, "/"));
	date->giorno = atoi(strtok(NULL, "/"));
}

void parseTime(char* str, ora_t* hour){
	hour->ora = atoi(strtok(str, ":"));
	hour->minuto = atoi(strtok(NULL, ":"));
	hour->secondo = atoi(strtok(NULL, ":"));
}

int parseFile(char* filename, corsa_t* corse){
	char buf[7*MAX_STR];
	FILE* fin = fopen(filename ,"r");
	if (!fin) return -1;

	int n = atoi(fgets(buf, 7*MAX_STR, fin)), i;

	for (i = 0; i < n && !feof(fin); i++) {
		fgets(buf, 7*MAX_STR, fin);
		char str[7][MAX_STR], *b = buf;
		for (int j = 0; j < 7; j++) {
			strncpy(str[j], strtok(b, " "), MAX_STR);
			b = NULL;
		}
		strncpy(corse[i].codice, str[0], MAX_CODE);
		strncpy(corse[i].partenza, str[1], MAX_STR);
		strncpy(corse[i].arrivo, str[2], MAX_STR);
		parseDate(str[3] ,&corse[i].data);
		parseTime(str[4], &corse[i].ora_p);
		parseTime(str[5], &corse[i].ora_a);
		corse[i].ritardo = atoi(str[6]);
	}

	if (i != n) return -2;

	fclose(fin);
	return n;
}

int compareDates(data_t date1, data_t date2) {

	if (date1.anno < date2.anno) return -1;
	if (date1.anno > date2.anno) return 1;

	if (date1.mese < date2.mese) return -1;
	if (date1.mese > date2.mese) return 1;

	if (date1.giorno < date2.giorno) return -1;
	if (date1.giorno > date2.giorno) return 1;

	return 0;
}

void printAllInInterval(corsa_t* corse, int n, bool ritardo) {
	data_t d1, d2;
	char d1s[15], d2s[15];
	printf("-\tInserisci date (yyyy/mm/dd yyyy/mm/dd): ");
	scanf("%15s %15s", d1s, d2s);
	parseDate(d1s, &d1);
	parseDate(d2s, &d2);
	printf("\tCodice\t\tPartenza\tDestinazione\tData\t\tOrario\t\t\tRitardo\n");
	for (int i = 0; i < n; i++) {
		if (ritardo && !corse[i].ritardo) continue;
		corsa_t corsa = corse[i];
		if (compareDates(d1, corsa.data) <= 0 && compareDates(d2, corsa.data) >= 0) {
			printInfo(corsa);
		}
	}
}

void printAllStation(corsa_t* corse, int n, bool from) {
	char st[MAX_STR];
	printf("-\tInserisci stazione: ");
	scanf("%31s", st);
	for (int i = 0; i < n; i++) {
		if (!strcmp(from ? corse[i].partenza : corse[i].arrivo, st))
			printInfo(corse[i]);
	}
}

void printRitardo(corsa_t* corse, int n) {
	char codice[MAX_CODE];
	printf("-\tInserisci codice: ");
	scanf("%7s", codice);

	int ritardo = 0, found = 0;

	for (int i = 0; i < n; i++) {
		if (!strcmp(corse[i].codice, codice)) {
			ritardo += corse[i].ritardo;
			found = 1;
		}
	}

	if (found)
		printf("-\tLa corsa %s ha fatto %d minuti di ritardo.\n", codice, ritardo);
	else
		printf("Corsa %s non trovata.\n", codice);
}

int main(int argc, char** argp){
	if (argc != 2) {
		printf("Usage: %s <corse.txt>\n", argp[0]);
		return -1;
	}

	corsa_t corse[MAX_CORSE];
	int n = parseFile(argp[1], corse);
	if (n == -1) {
		printf("Could not open file %s.\n", argp[1]);
		return 1;
	}
	if (n == -2) {
		printf("Error parsing file %s.\n", argp[1]);
		return 2;
	}

	printf("%d corse caricate.\n", n);

	enum comando_e comando = r_invalid;
	while (comando != r_fine) {
		comando = leggiComando();

		switch(comando) {
			case r_invalid:
				printf("-\tComando non riconosciuto.\n");
				break;
			case r_fine:
				printf("-\tChiudo...\n");
				break;
			case r_date:
				printAllInInterval(corse, n, false);
				break;
			case r_partenza:
				printAllStation(corse, n, true);
				break;
			case r_capolinea:
				printAllStation(corse, n, false);
				break;
			case r_ritardo:
				printAllInInterval(corse, n, true);
				break;
			case r_ritardo_tot:
				printRitardo(corse, n);
				break;
			case r_order:
				printf("-\tComando non riconosciuto.\n");
				break;
		}
	}	

	return 0;
}
