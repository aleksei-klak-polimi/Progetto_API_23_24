#include "String.h"
#include "WarehouseTree.h"
#include "WarehouseMap.h"
#include "Recipies.h"

int             resupplyWarehouse(int time, warehouseMap *map, warehouseTreeNode **root);
void            removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity);
void            removeIngredientsFromWarehouseByTime(warehouseTreeNode **root, warehouseMap *map, int time);
int             isOrderFulfillable(warehouseMap *map, recipie *recipie, int quantity);
