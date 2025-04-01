#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stdint.h>

typedef struct {
    uint8_t* bit_array;
    int m;
    int k;
} Bloomfilter;

void bloom_init(Bloomfilter* bf, int m, int k);
void bloom_insert(Bloomfilter* bf, const char* key);
int bloom_contains(Bloomfilter* bf, const char *key);
void bloom_free(Bloomfilter* bf);

#endif


