#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hll.h"

int main(int argc, char **argv)
{
    struct hll *h = hll_init();
    char line[256];

    if(argc < 2) {
        fprintf(stderr, "Please provide a file (one key per line)\n");
        exit(1);
    }

    FILE* file = fopen(argv[1], "r"); 
    if( ! file ) {
        fprintf(stderr, "Unable to read %s\n", argv[1]);
        exit(2);
    }


    while (fgets(line, sizeof(line), file)) {
        hll_add(h, (uint8_t *) line, strlen(line));
    }

    //hll_print(h);
    printf("Unique entries = %llu\n", hll_count(h));
    return 0;
}
