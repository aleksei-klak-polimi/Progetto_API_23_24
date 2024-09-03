#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "String.h"
#include "TimedItems.h"
#include "Map.h"


typedef struct numberedTimedItem                ingredientLot;
typedef struct numberedTimedItemList            ingredientLotList;
typedef struct numberedTimedItemListList        ingredientLotListList;
typedef struct numberedTimedItemListMap         warehouseMap;




void            addIngredientToMap(warehouseMap *map, ingredientLot*s);
void            removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient);
void            removeNodeFromIngredientMap(warehouseMap *map, ingredientLotListList *hashHead, ingredientLotList *ingredientHead, ingredientLotList *prevIngredientHead);