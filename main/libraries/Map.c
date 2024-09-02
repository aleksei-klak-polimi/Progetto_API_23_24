#include <stdint.h>
#include "Map.h"

unsigned int sdbm_hash(char *string){
    
    
    //SDBM implementation
    
    unsigned long hash = 0;
    int c;

    while((c = *string++)){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }


    //1:35 time


    //size 1048576
    //time 43 secs

    return hash % HASHMAPSIZE;



    //MURMUR implementation
    /*#define SEED 0x9747b28c  // You can change the seed to any value you prefer

    uint32_t len = 0;
    const uint32_t m = 0x5bd1e995;
    const int r = 24;
    uint32_t h = SEED ^ len;

    // Calculate the length of the string
    const char *str = string;
    while (*str++) len++;

    // Mix 4 bytes at a time into the hash
    const uint8_t *data = (const uint8_t *)string;
    while (len >= 4) {
        uint32_t k = *(uint32_t *)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    // Handle the last few bytes of the input array
    switch (len) {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    };

    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h % HASHMAPSIZE;


    //Map size 1048576
    //Time 47 secs

    //Map size 65536
    //Time 54 */
}