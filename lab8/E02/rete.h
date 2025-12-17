#ifndef RETE_H
#define RETE_H

#include <stdio.h>

typedef struct rete* rete_t;

rete_t parseFile(const char* filename);
int** generateList(rete_t rete);
void printNetworkInfo(FILE* fout, rete_t rete, int full);
void isSubgraphComplete(rete_t rete, int useList);
void freeNet(rete_t rete);

#endif