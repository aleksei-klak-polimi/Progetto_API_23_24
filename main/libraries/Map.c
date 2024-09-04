#include <stdint.h>
#include "Map.h"

unsigned int sdbm_hash(char *string){
        
    unsigned long hash = 0;
    int c;

    while((c = *string++)){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash % HASHMAPSIZE;
}