#ifndef RETE_H
#define RETE_H

#include <stdio.h>

typedef struct rete* rete_t;

rete_t parseFile(const char* filename);
int getMatrix(rete_t rete, int i, int j);
int getNode(rete_t rete, const char* nome);
int** generateList(rete_t rete);
void printNetworkInfo(FILE* fout, rete_t rete);

#endif