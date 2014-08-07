#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "hll.h"

int main(int argc, char **argv)
{
    struct hll *h = hll_init();
    char line[256];

    if(argc != 2) {
        fprintf(stderr, "Please provide a file (one key per line)\n");
        fprintf(stderr, "usage : %s file.txt\n", argv[0]);
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

    printf("Unique entries = %" PRIu64 "\n", hll_count(h));
    return 0;
}
