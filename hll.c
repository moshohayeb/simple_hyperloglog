#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "hll.h"


static inline uint8_t hll_get_zeros(uint64_t value) 
{
    uint8_t i;
    uint64_t mask = 1;
    for(i = 0 ; i < HLL_VALUE_BITS ; i++) {
        if( (mask & value) != 0 )
            break;
        mask <<= 1;
    }

    return i;
}


static void write_u64(uint64_t x) 
{
    int i;
    for(i = 0; i < 64 ; ++i) {
        putchar(x & ((uint64_t) 1 << (63 - i)) ? '1' : '0');
    }
    printf("\n");
}


static uint64_t hll_murmur64(const void * key, int len) 
{
    const uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;
    uint64_t h = HLL_SEED ^ (len * m);
    const uint8_t *data = (const uint8_t *)key;
    const uint8_t *end = data + (len-(len&7));

    while(data != end) {
        uint64_t k;
        k = *((uint64_t*)data);
        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
        data += 8;
    }

    switch(len & 7) {
        case 7: h ^= (uint64_t)data[6] << 48;
        case 6: h ^= (uint64_t)data[5] << 40;
        case 5: h ^= (uint64_t)data[4] << 32;
        case 4: h ^= (uint64_t)data[3] << 24;
        case 3: h ^= (uint64_t)data[2] << 16;
        case 2: h ^= (uint64_t)data[1] << 8;
        case 1: h ^= (uint64_t)data[0];
        h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
}


struct hll* hll_init(void)
{
    uint32_t i;
    struct hll *h;
    h = (struct hll *) calloc(1, sizeof(struct hll));
    if( ! h ) return HLL_ERROR;

    h->PE[0] = 1; 
    for (i = 1; i < 64; i++) {
        h->PE[i] = 1.0/(1ULL << i);
    }
    return h;
}


uint8_t  hll_add(struct hll *h, uint8_t *data, uint32_t datalen)
{
    uint64_t hash, value;
    uint16_t index;
    uint8_t zero;

    hash = hll_murmur64(data, datalen);
    index = (uint16_t) HLL_GET_INDEX(hash);
    value = HLL_GET_VALUE(hash);
    zero = hll_get_zeros(value);
    h->registers[index] = MAX(zero, h->registers[index]);

#if 0
    printf("HASH = ");  write_u64(hash);
    printf("INDEX = "); write_u64(HLL_GET_INDEX(hash));
    printf("VALUE = "); write_u64(HLL_GET_VALUE(hash));
    printf("index = %d\n", index); 
    printf("value = %llu\n", value); 
    printf("number of cons zeros = %d\n", hll_get_zeros(value));
#endif

    return HLL_OK;
}


uint64_t hll_count(struct hll *h)
{
    double m = HLL_REGISTERS;
    double alpha = 0.7213/(1+1.079/m);
    double E = 0;
    int32_t ez = 0; /* Number of registers equal to 0. */
    int32_t i;


   for (i = 0; i < HLL_REGISTERS; i++) {
        uint8_t reg = h->registers[i];
        if (reg == 0) {
            ez++;
            E += 1; 
        } else {
            E += h->PE[reg];
        }
    }

     E = (1/E)*alpha*m*m;

     if (E < m*2.5 && ez != 0) {
        E = m*log(m/ez); 
     } else if (m == 16384 && E < 72000) {
        double bias = 5.9119*1.0e-18*(E*E*E*E)
        -1.4253*1.0e-12*(E*E*E)+
        1.2940*1.0e-7*(E*E)
        -5.2921*1.0e-3*E+
        83.3216;
        E -= E*(bias/100);
     }

    return (uint64_t) E*2;
}


void hll_print(struct hll *h)
{
    (void)write_u64;
    uint32_t i;
    for(i = 0 ; i < HLL_REGISTERS ; i++) {
        printf("R %d : %d\n", i, h->registers[i]);
    }
}
