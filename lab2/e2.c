#include <stdlib.h>
#include <stdio.h>

int** malloc2dR(int r, int c) {
	int** ret = malloc(r * sizeof(int*));
	for (int i = 0; i < r; i++)
		ret[i] = malloc(c * sizeof(int));

	return ret;
}

void malloc2dP(int*** ret, int r, int c) {
	*ret = malloc2dR(r, c);
}

int readFile(int*** mat, const char* filename, int* r, int* c) {
	int i, j = 0, n = 0;

	FILE* fin;
	fopen_s(&fin, filename, "r");
	if (fin == NULL) return -1;

	fscanf_s(fin, "%d %d", r, c);
	printf("Matrice %dx%d\n", *r, *c);

	malloc2dP(mat, *r, *c);
	if (*mat == NULL) return -2;

	for (i = 0; i < *r; i++) {
		while (j < *c) {
			fscanf_s(fin, "%d", &((*mat)[i][j]));
			j++;
		}
		j = 0;
	}

	return 0;
}

void separa(int** mat, int r, int c, int** bianche, int** nere, int* b, int* n) {
	*bianche = malloc((r*c/2 + (r*c) % 2) * sizeof(int));
	*nere = malloc(r*c / 2 * sizeof(int));

	*n = 0; *b = 0;
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++) {
			if ((i + j) % 2)
				(*nere)[(*n)++] = mat[i][j];
			else
				(*bianche)[(*b)++] = mat[i][j];
		}
}

int main() {
	int** mat, r, c, *bianche, *nere;
	if(readFile(&mat, "mat.txt", &r, &c)) return -1;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++)
			printf("%d ", mat[i][j]);
		printf("\n");
	}

	int b, n;
	separa(mat, r, c, &bianche, &nere, &b, &n);

	printf("Vettore 'bianco':\t");
	for (int i = 0; i < b; i++)
		printf("%d ", bianche[i]);
	printf("\n");

	printf("Vettore 'nero':  \t");
	for (int i = 0; i < n; i++)
		printf("%d ", nere[i]);
	printf("\n");

	for (int i = 0; i < r; i++)
		free(mat[i]);
	free(mat);

	free(bianche);
	free(nere);

	return 0;
}