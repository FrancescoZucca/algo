#include <stdio.h>
#include "rete.h"

#define FILENAME "lab8/E02/grafo.txt"

int main(int argc, char** argp) {
    rete_t rete = parseFile(FILENAME);
    if (!rete) {
        printf("File non trovato. \n");
        return -1;
    }
    printNetworkInfo(stdout, rete);
}