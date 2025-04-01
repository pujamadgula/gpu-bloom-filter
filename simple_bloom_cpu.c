#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//// CPU ONLY - did not do GPU yet 

typedef struct {
    int m;                  // size of bit vector
    int k;                  // number of hash functions
    int* vector;            // bit vector
    char** keys;            // store actual keys for verification
    int data_size;          // number of inserted keys

    int false_positive;     // counter for incorrect "might be present"
} Bloom;



// Simple hash (modify later) -- explore different hashing functions later 
unsigned int simple_hash(const char* str, int seed) {
    unsigned int hash = seed;
    while (*str) {
        hash = hash * 101 + *str++; // Multiply hash and add next characterx
    }
    return hash;
}

// Creat a new Bloom filter
Bloom* create_bloom(int m, int k) {
    Bloom* b = malloc(sizeof(Bloom)); // Allocate memory for bloom filter structure
    (*b).m = m;
    (*b).k = k;
    (*b).vector = calloc(m, sizeof(int)); // Initialize bit vector to all 0s
    (*b).keys = malloc(1000 * sizeof(char*)); // Preallocate up to 1000 keys
    (*b).data_size = 0;
    (*b).false_positive = 0;
    return b;
    }

// Inserting a key into Bloom filter
void insert_bloom(Bloom* b, const char* key) {
    (*b).keys[(*b).data_size++] = strdup(key); // Store the key in memory for later lookup
    for (int i = 0; i < (*b).k; i++) {
        unsigned int index = simple_hash(key, i + 1) % (*b).m ; // Generate k different hash indices
        (*b).vector[index] = 1; // Set corresponding bit to 1
    }

}

// Check if a key may be in Bloom filter
int contains_bloom(Bloom* b, const char* key) {
    for (int i = 0; i < (*b).k; i++) {
        unsigned int index = simple_hash(key, i + 1) % (*b).m; 
        if ((*b).vector[index] == 0) 
        return 0; // If any bit is 0, the item is definitely not present
    }
    return 1;  // All bits are 1 thhe item might be present (could be a false positive)
}


// Get key from Bloom filter (simulate a lookup)
char* get_bloom(Bloom* b, const char* key) {
    if (contains_bloom(b, key)) {
        for (int i = 0; i < (*b).data_size; i++) {
            if (strcmp((*b).keys[i], key) == 0) {
                return (*b).keys[i];
            }
        }

        (*b).false_positive++;
    }
    return NULL;
}

// Generate a random string of length n using given chars
char* rand_data(int n, const char* chars) {
    int len = strlen(chars);
    char* res = malloc(n + 1); // **C strings terminated by \0 (+1 for \0)
    for (int i = 0; i < n; i++)
        res[i] = chars[rand() % len];
    
    res[n] = '\0'; // C string specific
    return res;
}





// Test the Bloom filter with false positive count
float bloom_test(int m, int n, int k) {
    Bloom* bloom = create_bloom(m, k);
    char** inserted = malloc(n * sizeof(char*));


    for (int i = 0; i < n; i++) {
        inserted[i] = rand_data(10, "abcde");
        insert_bloom(bloom, inserted[i]);
    }

    for (int i = 0; i < n; i++) {
        char* query = rand_data(10, "fghil");
        get_bloom(bloom, query);
        free(query);
    }


    float percent = (float)(*bloom).false_positive / n * 100.0;

    // Cleanup (Free up all space)
    for (int i = 0; i < n; i++)
        free(inserted[i]);

    
    free(inserted);

    for (int i = 0; i < (*bloom).data_size; i++)
        free((*bloom).keys[i]);
    free((*bloom).keys);
    free((*bloom).vector);
    free(bloom);

    return percent;
}



// Compute the theoretical false positive rate
double theoretical_fp(int m, int n, int k) {
    double exp_part = exp(-(double)(k * n) / m);
    return pow(1.0 - exp_part, k) * 100.0; // return as percentage
}

int main() {
    srand(time(NULL));
    int m = 10000;
    int n = 1000;
    FILE* f = fopen("bloom_output.csv", "w");
    fprintf(f, "k,false_positive,theoretical_fp\n");


    for (int k = 1; k <= 63; ++k) {

        float perc = bloom_test(m, n, k);                   // experimental result
        double theory = theoretical_fp(m, n, k);           // theoretical result
        printf("k = %2d → %.2f%% actual, %.2f%% theoretical\n", k, perc, theory);
        fprintf(f, "%d,%.2f,%.2f\n", k, perc, theory);     // save both to CSV
    }


    fclose(f);
    printf("results saved to 'bloom_output.csv'\n");
    return 0;
}
