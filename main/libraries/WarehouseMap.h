#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "String.h"
#include "Map.h"

struct numberedTimedItem{
    String  name;
    int     amount;
    int     time;
    int     totalWeigth;
};


struct numberedTimedItemList{
    struct numberedTimedItem        *el;
    struct numberedTimedItemList    *next;
};

struct numberedTimedItemListList{
    int                                 totalAmount;
    struct numberedTimedItemList        *el;
    struct numberedTimedItemListList    *next;
};

struct numberedTimedItemListMap{
    struct numberedTimedItemListList *hashArray[HASHMAPSIZE];
};



typedef struct numberedTimedItem                ingredientLot;
typedef struct numberedTimedItemList            ingredientLotList;
typedef struct numberedTimedItemListList        ingredientLotListList;
typedef struct numberedTimedItemListMap         warehouseMap;




void            addIngredientToMap(warehouseMap *map, ingredientLot*s);
void            removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient);
void            removeNodeFromIngredientMap(warehouseMap *map, int hash, ingredientLotListList *hashHead, ingredientLotListList *prevHashHead, ingredientLotList *ingredientHead, ingredientLotList *prevIngredientHead);