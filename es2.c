#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define MAX_CODE 7
#define MAX_STR 31
#define MAX_CORSE 1000

typedef struct {
	int giorno, mese, anno;
} data_t;

typedef struct {
	int ora, minuto;
} ora_t;

typedef struct {
	char codice[MAX_CODE], partenza[MAX_STR], arrivo[MAX_STR];
	data_t data;
	ora_t ora_p, ora_a;
	int ritardo;
} corsa_t;

enum {
	r_date,
	r_partenza,
	r_capolinea,
	r_ritardo,
	r_ritardo_tot,
	r_fine,
	r_invalid
} comando_e;

int leggiComando(){
	char buf[MAX_STR];
	printf("Inserisci comando:\n> ")
	fgets(buf, MAX_STR, stdin);

	if (!strcmp(buf, "date")) return r_date;
	if (!strcmp(buf, "partenza")) return r_partenza;
	if (!strcmp(buf, "capolinea")) return r_capolinea;
	if (!strcmp(buf, "ritardo")) return r_ritardo;
	if (!strcmp(buf, "ritardo_tot")) return r_ritardo_tot;
	if (!strcmp(buf, "fine")) return r_fine;
	return r_invalid;
}

void parseDate(char* str, data_t* date){
	date->giorno = atoi(strtok(str, "/"));
	date->mese = atoi(strtok(NULL, "/"));
	date->anno = atoi(strtok(NULL, "/"));
}

void parseTime(char* str, ora_t* hour){
	hour->ora = atoi(strtok(str, ":"));
	hour->minuto = atoi(strtok(NULL, ":"));
}

int parseFile(char* filename, corsa_t* corse){
	char buf[7*MAX_STR];
	FILE* fin = fopen(filename ,"r");
	if (!fin) return -1;

	int n = atoi(fgets(buf, 7*MAX_STR, fin)), i;

	for (i = 0; i < n && !feof(fin); i++) {
		fgets(buf, 7*MAX_STR, fin);
		strncpy(corse[i].codice, strtok(buf, " "), MAX_CODE);
		strncpy(corse[i].partenza, strtok(NULL, " "), MAX_STR);
		strncpy(corse[i].arrivo, strtok(NULL, " "), MAX_STR);
		parseDate(strtok(NULL, " ") ,&(corse[i].data));
		parseTime(strtok(NULL, " "), &(corse[i].ora_p));
		parseTime(strtok(NULL, " "), &(corse[i].ora_a));
		corse[i].ritardo = atoi(strtok(NULL, " "));
	}

	if (i != n) return -2;

	fclose(fin);
	return n;
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
				printf("\tComando non riconosciuto.\n");
			case r_fine:
				break;
			case r_date:
				
		}
	}	

	return 0;
}
