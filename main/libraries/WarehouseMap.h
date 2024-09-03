#ifndef WRHS_MAP_INCLUDED
#define WRHS_MAP_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "String.h"
#include "TimedItems.h"
#include "Map.h"

struct numberedTimedLot{
    int                         amount;
    int                         time;
    struct numberedTimedLot     *next;
};


struct numberedTimedLotList{
    char                                *ingredientName;
    int                                 totalAmount;
    struct numberedTimedLot             *el;
    struct numberedTimedLotList         *next;
};


struct numberedTimedLotListMap{
    struct numberedTimedLotList *hashArray[HASHMAPSIZE];
};


typedef struct numberedTimedLot                 ingredientLot;
typedef struct numberedTimedLotList             ingredientLotListList;
typedef struct numberedTimedLotListMap          warehouseMap;




void            addIngredientToMap(warehouseMap *map, ingredientLot*s, char *ingredientName);
void            removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient);
void            removeNodeFromIngredientMap(warehouseMap *map, ingredientLotListList *hashHead, ingredientLot *ingredientHead, ingredientLot *prevIngredientHead);


#endif