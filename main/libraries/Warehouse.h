#include "String.h"
#include "WarehouseTree.h"
#include "WarehouseMap.h"
#include "Recipies.h"


int             removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity);
void            removeIngredientsFromWarehouseByTime(warehouseTreeNode **root, warehouseMap *map, int time);
