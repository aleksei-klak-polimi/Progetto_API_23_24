#ifndef ORDERS_H_INCLUDED
#define ORDERS_H_INCLUDED

#include "TimedItems.h"
#include "Map.h"
#include "Recipies.h"

struct numberedTimedItemQueue{
    char                                *ingredient;
    struct numberedTimedItemList        *head;
    struct numberedTimedItemList        *tail;
};


struct numberedTimedItemQueueList{
    struct numberedTimedItemQueue       *el;
    struct numberedTimedItemQueueList   *next;
};


struct numberedTimedItemQueueMap{
    struct numberedTimedItemQueueList *hashArray[HASHMAPSIZE];
};



typedef struct numberedTimedItem                orderedItem;
typedef struct numberedTimedItemList            orderedItemList;
typedef struct numberedTimedItemQueue           orderedItemQueue;
typedef struct numberedTimedItemQueueList       orderedItemQueueList;
typedef struct numberedTimedItemQueueMap        orderedItemQueueMap;



int             readOrder(orderedItem *item, int time);
void            printOrder(orderedItem *item);
void            addOrderToIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead);
void            removeOrderFromIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead);
void            removeOrderFromPending(orderedItem *item, orderedItemQueue *ordersWaiting);
void            addOrderToReady(orderedItem *item, orderedItemQueue *ordersReady);


#endif