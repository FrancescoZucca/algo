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
	r_fine,
  	r_print,
	r_search,
	r_invalid,
};

void printInfo(corsa_t corsa) {
	printf("\t%7s\t%15s\t%15s\t%04d/%02d/%02d\t%02d:%02d -> %02d:%02d\t%2d minuti di ritardo\n",
		corsa.codice, corsa.partenza, corsa.arrivo, corsa.data.anno, corsa.data.mese, corsa.data.giorno,
		corsa.ora_p.ora, corsa.ora_p.minuto, corsa.ora_a.ora, corsa.ora_a.minuto, corsa.ritardo);
}

int leggiComando(){
	char buf[MAX_STR];
	printf("Comandi disponibili:\n-\tprint, search, fine\nInserisci comando:\n> ");
	scanf("%30s", buf);
	if (!strcmp(buf, "fine")) return r_fine;
    if (!strcmp(buf, "print")) return r_print;
	if (!strcmp(buf, "search")) return r_search;
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

int getCurrentStep(corsa_t* corsa, int step) { // funzione helper per estrarre l'int corretto
	switch (step) {
		case 0:
			return corsa->ora_p.secondo;
		case 1:
			return corsa->ora_p.minuto;
		case 2:
			return corsa->ora_p.ora;
		case 3:
			return corsa->data.giorno;
		case 4:
			return corsa->data.mese;
		case 5:
			return corsa->data.anno;
		default:
			printf("Impossible case!\n");
			return -1;
	}
}

char* getStringToCompare(corsa_t* corsa, int type) { // funzione helper per estrarre la stringa corretta
	switch (type) {
		case 2:
			return corsa->codice;
		case 3:
			return corsa->partenza;
		case 4:
			return corsa->arrivo;
		default:
			printf("Impossible case!\n");
		return NULL;
	}
}

void radixSort(corsa_t** corse, int n, int step) {

	// Usando il counting sort, assicuro la stabilità necessaria con un algoritmo lineare. Si assumono anni di 4 cifre.

	corsa_t** result = malloc(n*sizeof(corsa_t *));
	int digits = step == 6 ? 10000 : 100; // se stiamo ordinando tutto tranne gli anni, sono numeri a due cifre e bisogna solo iterare sino a 10^2. Per gli anni, si itera sino a 10^4.

	for (int j = 1; j < digits; j*=10) {

		int i, count[10] = {0};
		for (i = 0; i < n; i++)
			count[(getCurrentStep(corse[i], step)/j)%10]++; // Vettore occorrenze semplici

		for (i = 1; i < 10; i++)
			count[i] += count[i-1]; // Vettore occorrenze multiple

		for (i = n-1; i >= 0; i--) {
			result[count[(getCurrentStep(corse[i], step)/j)%10] -1] = corse[i]; // sort nel vettore result
			count[(getCurrentStep(corse[i], step)/j)%10]--;
		}

		for (i = 0; i < n; i++)
			corse[i] = result[i]; // copio il risultato
	}
	free(result);
}

void orderByDate(corsa_t** corse, int n) { // wrapper per il radix sort
	for (int step = 0; step < 6; step++) {
		radixSort(corse, n, step);
	}
}

void merge(corsa_t** A, corsa_t** B, int l, int q, int r, int type) { // ricombina gli array suddivisi ricorsivamente
	int i = l, j = q+1;
	for (int k = l; k <= r; k++)
		if (i > q)
			B[k] = A[j++];
		else if (j > r)
			B[k] = A[i++];
		else if (strcmp(getStringToCompare(A[i], type), getStringToCompare(A[j], type)) < 0)
			B[k] = A[i++];
		else
			B[k] = A[j++];

	for (int k = l; k <= r; k++)
		A[k] = B[k];
}

void mergeSortR(corsa_t** A, corsa_t** B, int l, int r, int type) { // discendi e ricombina ricorsivamente
	if (l >= r)
		return;
	const int q = (l + r) / 2;
	mergeSortR(A, B, l, q, type);
	mergeSortR(A, B, q+1, r, type);
	merge(A, B, l, q, r, type);
}

void orderByString(corsa_t** corse, int n, int type) { // wrapper per il merge sort
	corsa_t** B = malloc(n*sizeof(corsa_t));
	if (B == NULL) return;
	mergeSortR(corse, B, 0, n-1, type);
	free(B);
}

corsa_t* cercaCodiceLineare(corsa_t* corse, int n, char* codice) { // cerca il codice iterando su tutti gli elementi del vettore, ritornando la prima occorrenza
	for (int i = 0; i < n; i++)
		if (strcmp(corse[i].codice, codice) == 0)
			return &corse[i];

	return NULL;
}

corsa_t* cercaCodiceDicotomica(corsa_t** corse, int n, char* codice) { // cerca il codice con una ricerca binaria su un array ordinato di elementi.
	int i, k = n/2, l = 0, r = n-1;
	while ((i = strcmp(codice, corse[k]->codice)) && l<=r)
		if (i > 0) {
			l = k+1;
			k = l + (r-l)/2;
		} else {
			r = k-1;
			k = l + (r-l)/2;
		}

	if (l>r) return NULL;

	return corse[k];
}

int partialCompare(char* p1, char* p2) { // come strcmp, ma itera solo sui primi caratteri sino a quando non esaurisce p2
	while (*p2++ == *p1++)
		if (*p2 == 0) return 0;

	if (*--p2 > *--p1) return -1;
	return 1;
}

int* cercaStazioneLineare(corsa_t* corse, int n, char* codice, int* results) {
	int j = 0;
	for (int i = 0; i < n; i++) {
		if (partialCompare(corse[i].partenza, codice) == 0) results[j++] = i;
	}

	results[j] = -1;
	return results;
}

int binSearchR(corsa_t** corse, int l, int r, char* codice, int lower) {
	if (l > r) return l;

	int q = l + (r-l)/2;

	if ((lower && partialCompare(corse[q]->partenza, codice) >= 0) || (!lower && partialCompare(corse[q]->partenza, codice) > 0))
		return binSearchR(corse, l, q-1, codice, lower);

	return binSearchR(corse, q+1, r, codice, lower);
}

corsa_t** cercaStazioneDicotomica(corsa_t** corse, int n, char* codice, int* out)
{
	int l = 0, r = n-1;

	int low = binSearchR(corse, l, r, codice, 1);
	int high = binSearchR(corse, l, r, codice, 0);

	if (l>r) return NULL;

	*out = high - low;
	return &corse[low];
}

void orderPointers(int n, corsa_t **corse_date, corsa_t **corse_codice, corsa_t **corse_partenza, corsa_t **corse_arrivo) {
	orderByDate(corse_date, n);
	orderByString(corse_codice, n, 2);
	orderByString(corse_partenza, n, 3);
	orderByString(corse_arrivo, n, 4);
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

	corsa_t **corse_date = malloc(n * sizeof(corsa_t *));
	corsa_t **corse_codice = malloc(n * sizeof(corsa_t *));
	corsa_t **corse_partenza = malloc(n * sizeof(corsa_t *));
	corsa_t **corse_arrivo = malloc(n * sizeof(corsa_t *));

    for (int i = 0; i < n; i++) {
      	corse_date[i] = corse+i;
    	corse_codice[i] = corse+i;
    	corse_partenza[i] = corse+i;
    	corse_arrivo[i] = corse+i;
    }

	orderPointers(n, corse_date, corse_codice, corse_partenza, corse_arrivo);

	printf("%d corse caricate.\n", n);

	enum comando_e comando = r_invalid;
	while (comando != r_fine) {
		comando = leggiComando();
		int option;

		switch(comando) {
			case r_invalid:
				printf("-\tComando non riconosciuto.\n");
				break;
			case r_print:
                printf("Stampare su video (1) o su file (2)?> ");
                option = -1;
				int c;
                scanf("%d", &option);
                FILE* fin = fopen(argp[1], "r");
                FILE* out;
                if (option == 1) out = stdout;
                else if (option == 2) {
                  printf("Inserisci il nome del file su cui stampare: ");
                  char filename[MAX_STR];
                  scanf("%30s", filename);
                  out = fopen(filename, "w");
                } else {
                	printf("Opzione non riconosciuta.\n");
                    break;
                }
				while((c = getc(fin)) != EOF) putc(c, out);
				if (out != stdout) fclose(out);
				break;
			case r_fine:
				printf("-\tChiudo...\n");
				break;case r_search:
				printf("-\tCosa vuoi cercare?\n- 1)\tCodice\n- 2)\tStazione di Partenza\n> ");
				scanf("%d", &option);
				int lineare;
				printf("Vuoi cercare dicotomicamente (1) o linearmente (2)? \n> ");
				scanf("%d", &lineare);
				lineare--;
				if (lineare != 0 && lineare != 1) {
					printf("Opzione non riconosciuta. Cerco linearmente...\n");
					lineare = 1;
				}

				char input[MAX_STR];
				printf("Inserisci la stringa da cercare: ");
				scanf("%30s", input);

				switch (option) {
					case 1:
						corsa_t* result = lineare ? cercaCodiceLineare(corse, n, input) : cercaCodiceDicotomica(corse_codice, n, input);
						if (result != NULL)
							printInfo(*result);
						else
							printf("Nessun risultato trovato.\n");
						break;
					case 2:
						if (lineare) {
							int* indexes = malloc(sizeof(int) * (n+1));
							if (indexes == NULL) {
								printf("Errore allocazione memoria.\n");
								break;
							}
							cercaStazioneLineare(corse, n, input, indexes);
							if (*indexes == -1)
								printf("Nessun risultato trovato.\n");
							else {
								for (int i = 0; indexes[i] != -1; i++)
									printInfo(corse[indexes[i]]);
							}
							free(indexes);
						} else {
							int result_num;
							corsa_t** result = cercaStazioneDicotomica(corse_partenza, n, input, &result_num);
							if (result == NULL || result_num < 1)
								printf("Nessun risultato trovato.\n");
							else
								for (int k = 0; k < result_num; k++)
									printInfo(*result[k]);
						}
						break;
					default:
						printf("Opzione non riconosciuta.\n");
				}

				break;
		}
	}

    free(corse_date);
	free(corse_codice);
	free(corse_partenza);
	free(corse_arrivo);

	return 0;
}
