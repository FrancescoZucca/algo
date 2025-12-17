#include <stdio.h>
#include "rete.h"

#define FILENAME "lab8/E02/grafo.txt"

int main(int argc, char** argp) {

    char* fname = FILENAME;

    if (argc == 2) fname = argp[1];

    rete_t rete = parseFile(fname);
    if (!rete) {
        printf("File non trovato. \n");
        return -1;
    }
    printNetworkInfo(stdout, rete);
}
