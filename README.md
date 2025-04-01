# gpu-bloom-filter


### Background

- space efficient probabilistic data structure used to test set membership
- "Is this item "possibly" in the set, or definitely not??"
- False positives are possible but false negatives are not
- Useful for memory-efficiency --> space and speed 


### Example 

- Inserting an item into a bloom filter = Pass "apple" through k different hash functions
hash1("apple") --> 12
hash2("apple") --> 340
hash3("apple") --> 78

Set bits in bit array 
positions 12, 340, 78 --> set to "1"
"apple" is now in the bloom filter 
If all are 1 --> item might be in the set 
If any are 0 --> item is definitely not in the set 

* Never store original items, just the bit pattern 


## Algorithm Description

1. Initialize  Bloom filter with a bit vector of size `m` and `k` hash functions.  
2. Generate `n` random strings using a limited character set ( `"abcde"`).  
3. For each string:
   - Compute `k` hash values using a seeded hash function. -- can modify different hash functions to get more ideal FPR in the future.
   - Set those `k` bit positions in the vector to 1.
   - Store the string in a list for verification later.  
4. Generate `n` new random strings from a different character set (`"fghil"`).  
5. For each new string:
   - Check Bloom filter using the same k hash functions.
   - If all those bits are 1,  the key might be present.
   - If the key is not in the stored list, count it as a false positive.  
6. Calculate the false positive rate:  
   `(false positives / total test queries) * 100`.  
7. Repeat steps 1–6 for values of `k`  from 1 to 63. 
8. Add a theoretical calculation using equation onWiki.  
8. Record each result (`k`, false positive rate) into a CSV file.  
9. Output results to console and save the file for visualization.  

* False postive rate should increase as k increases because too many bits are set in the bit vector so the bloom filter gets overstaturated 

### Pseudocode:


function simple_hash(key, seed):
    hash = seed
    for each character in key:
        hash = hash * 101 + ASCII(character) (Can Test different hash functions later)
    return hash

function create_bloom(m, k):
    initialize Bloom with:
        bit_vector of size m (all 0s)

        number_of_hashes = k
        inserted_keys = empty list
        false_positive_count = 0
    return bloom

function insert_bloom(bloom, key):
    add key to inserted_keys
    for i = 1 to k:
        index = simple_hash(key, i) % m
        set bit_vector[index] = 1

function contains_bloom(bloom, key):
    for i = 1 to k:
        index = simple_hash(key, i) % m
        if bit_vector[index] == 0:
            return false
    return true

function get_bloom(bloom, key):
    if contains_bloom(key):
        if key is in inserted_keys:
            return key
        else:
            increment false_positive_count
    return NULL


// Not sure if this is the best way to test (Using disjoint character sets to see if FPR makes sense)

Insertion:
1. Each key sets k bits in the bit vector.
2. With more keys (n), more bits get flipped to 1.

Query:
1. To check a key, we cna hash it k times.
2. If all k corresponding bits are 1, the Bloom filter says "maybe it's in the set".

But:
The query key may be totally unrelated (like "fghil" vs "abcde").
but its k hash values might coincidentally land on k positions that were already set to 1 by other keys.
That's a false positive (Basically at high k's the FPR will also be high)


function rand_data(length, charset):
    generate random string of given length using characters from charset
    return string

function bloom_test(m, n, k):
    bloom = create_bloom(m, k)
    insert n random keys using charset "abcde"
    test n random queries using charset "fghil" (not in inserted set)
    return (false_positive_count / n) * 100



function main():
    set m = 10000, n = 1000
    open CSV file with headers "k,false_positive"
    for k = 1 to 63:
        rate = bloom_test(m, n, k)
        log k and rate to file
        print result
    close file

## Results

k =  1 → 8.10% actual, 9.52% theoretical
k =  2 → 6.90% actual, 3.29% theoretical
k =  3 → 5.30% actual, 1.74% theoretical
k =  4 → 8.00% actual, 1.18% theoretical
k =  5 → 8.80% actual, 0.94% theoretical
k =  6 → 8.10% actual, 0.84% theoretical
k =  7 → 10.20% actual, 0.82% theoretical
k =  8 → 11.20% actual, 0.85% theoretical
k =  9 → 16.00% actual, 0.91% theoretical
k = 10 → 14.90% actual, 1.02% theoretical
k = 11 → 19.40% actual, 1.16% theoretical
k = 12 → 18.50% actual, 1.36% theoretical
k = 13 → 21.90% actual, 1.60% theoretical
k = 14 → 21.30% actual, 1.90% theoretical
k = 15 → 23.60% actual, 2.27% theoretical
k = 16 → 26.10% actual, 2.71% theoretical
k = 17 → 28.40% actual, 3.24% theoretical
k = 18 → 32.70% actual, 3.87% theoretical
k = 19 → 33.90% actual, 4.60% theoretical
k = 20 → 39.70% actual, 5.46% theoretical
k = 21 → 39.30% actual, 6.44% theoretical
k = 22 → 44.90% actual, 7.55% theoretical
k = 23 → 37.60% actual, 8.80% theoretical
k = 24 → 42.00% actual, 10.20% theoretical
k = 25 → 52.50% actual, 11.75% theoretical
k = 26 → 50.20% actual, 13.44% theoretical
k = 27 → 52.20% actual, 15.28% theoretical
k = 28 → 57.40% actual, 17.26% theoretical
k = 29 → 56.60% actual, 19.37% theoretical
k = 30 → 61.20% actual, 21.61% theoretical
k = 31 → 61.30% actual, 23.96% theoretical
k = 32 → 61.70% actual, 26.40% theoretical
k = 33 → 62.70% actual, 28.93% theoretical
k = 34 → 69.30% actual, 31.54% theoretical
k = 35 → 69.40% actual, 34.19% theoretical
k = 36 → 71.40% actual, 36.89% theoretical
k = 37 → 73.30% actual, 39.60% theoretical
k = 38 → 74.60% actual, 42.33% theoretical
k = 39 → 74.40% actual, 45.04% theoretical
k = 40 → 74.40% actual, 47.74% theoretical
k = 41 → 82.50% actual, 50.40% theoretical
k = 42 → 81.70% actual, 53.02% theoretical
k = 43 → 75.00% actual, 55.57% theoretical
k = 44 → 86.60% actual, 58.07% theoretical
k = 45 → 88.40% actual, 60.49% theoretical
k = 46 → 86.80% actual, 62.83% theoretical
k = 47 → 83.80% actual, 65.09% theoretical
k = 48 → 86.90% actual, 67.26% theoretical
k = 49 → 88.80% actual, 69.33% theoretical
k = 50 → 86.90% actual, 71.32% theoretical
k = 51 → 84.80% actual, 73.21% theoretical
k = 52 → 90.60% actual, 75.00% theoretical
k = 53 → 95.60% actual, 76.70% theoretical
k = 54 → 92.60% actual, 78.31% theoretical
k = 55 → 93.80% actual, 79.83% theoretical
k = 56 → 91.00% actual, 81.26% theoretical
k = 57 → 91.40% actual, 82.61% theoretical
k = 58 → 95.20% actual, 83.87% theoretical
k = 59 → 95.40% actual, 85.06% theoretical
k = 60 → 95.90% actual, 86.16% theoretical
k = 61 → 95.70% actual, 87.20% theoretical
k = 62 → 93.50% actual, 88.17% theoretical
k = 63 → 93.80% actual, 89.07% theoretical
results saved to 'bloom_output.csv'

best FPR is k = 3 for actual 
best FPR is k = 7 for theoretical (k = (m/n)ln2 = 6.93 = 7)



## Resources

https://en.wikipedia.org/wiki/Bloom_filter
https://glowingpython.blogspot.com/2013/01/bloom-filter.html
https://www.cs.jhu.edu/~langmea/resources/lecture_notes/115_bloom_filters_pub.pdf

## run command
gcc simple_bloom.c -o bloom