#ifndef __HLL_H
#define __HLL_H

#include <stdint.h>

#define HLL_SEED			0xadc83b19ULL
#define HLL_REGISTERS		(1 << 14)


#define HLL_INDEX_BITS		14
#define HLL_VALUE_BITS		50
#define HLL_INDEX_MASK		0xFFFC000000000000
#define HLL_VALUE_MASK		0x0003FFFFFFFFFFFF


#define HLL_GET_INDEX(x)	((x & HLL_INDEX_MASK) >> (64-HLL_INDEX_BITS))
#define HLL_GET_VALUE(x)	(x & HLL_VALUE_MASK) 

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))


#define HLL_OK      1
#define HLL_ERROR   0

struct hll {
    uint8_t registers[HLL_REGISTERS];
    double PE[64];
};


struct hll* hll_init(void);
uint8_t  	hll_add(struct hll *h, uint8_t *data, uint32_t datalen);
void 		hll_print(struct hll *h);
uint64_t 	hll_count(struct hll *h);


#endif

