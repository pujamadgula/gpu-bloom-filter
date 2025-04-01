#include "bloom_filter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Still in Progress with new hash function


// static hash function using djb2 algorithm and seed
static unsigned int simple_hash(const char* str, int seed) {
    unsigned int hash = 5381 + seed;
    while (*str) {
        // hash * 33 + current character 
        hash = ((hash << 5) + hash) + *str;
        str++;
    }
    return hash; 
}

// Initializes the Bloom Filer 
// -m: number of bits in the bit array 
// -k: number of hash functions to use 
void bloom_init(Bloomfilter* bf, int m, int k){
    bf->m = m;
    bf->k = k; 

    // Allocate memory for the bit array and set all bits to 0
    // (each bit is a byte here for simplicity; this can be optimized later)
    bf->bit_array = (uint8_t*)calloc(m, sizeof(uint8_t));

}

void bloom_insert(Bloomfilter* bf, const char* key) {
    for (int i = 0; i < bf->k; i++) {
        unsigned int hash = simple_hash(key, i);
        int bit_index = hash % bf->m;
        bf->bit_array[bit_index] = 1; // Sets the corresponding bit to 1 


    }
}