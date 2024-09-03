#ifndef ORDERS_H_INCLUDED
#define ORDERS_H_INCLUDED

#include "TimedItems.h"
#include "Map.h"
#include "Recipies.h"

struct numberedTimedLinkedItem{
    char        *name;
    recipie     *recipie; //35 sec without
    int         amount;
    int         time;
    int         totalWeigth;
};


struct numberedTimedLinkedItemList{
    struct numberedTimedLinkedItem      *el;
    struct numberedTimedLinkedItemList  *next;
};


struct numberedTimedItemQueue{
    char                                    *ingredient;
    struct numberedTimedLinkedItemList      *head;
    struct numberedTimedLinkedItemList      *tail;
};


struct numberedTimedItemQueueList{
    struct numberedTimedItemQueue       *el;
    struct numberedTimedItemQueueList   *next;
};


struct numberedTimedItemQueueMap{
    struct numberedTimedItemQueueList *hashArray[HASHMAPSIZE];
};



typedef struct numberedTimedLinkedItem          orderedItem;
typedef struct numberedTimedLinkedItemList      orderedItemList;
typedef struct numberedTimedItemQueue           orderedItemQueue;
typedef struct numberedTimedItemQueueList       orderedItemQueueList;
typedef struct numberedTimedItemQueueMap        orderedItemQueueMap;



int             readOrder(orderedItem *item, int time);
void            printOrder(orderedItem *item);
void            addOrderToIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead);
void            removeOrderFromIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead);
void            removeOrderFromPending(orderedItemList *current, orderedItemList *prev, orderedItemQueue *ordersWaiting);
void            addOrderToReady(orderedItem *item, orderedItemQueue *ordersReady);


#endif