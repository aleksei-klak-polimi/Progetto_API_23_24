#ifndef ORDERS_H_INCLUDED
#define ORDERS_H_INCLUDED

#include "TimedItems.h"
#include "Map.h"
#include "Recipies.h"
#include "Warehouse.h"

struct numberedTimedLinkedItem{
    recipie     *recipie;
    short int   amount;
    int         time;
    int         totalWeigth;
    struct numberedTimedLinkedItem *next;
};


struct numberedTimedItemQueue{
    char                                    *ingredient;
    struct numberedTimedLinkedItem          *head;
    struct numberedTimedLinkedItem          *tail;
};


struct numberedTimedItemQueueList{
    struct numberedTimedItemQueue       *el;
    struct numberedTimedItemQueueList   *next;
};


struct numberedTimedItemQueueMap{
    struct numberedTimedItemQueueList *hashArray[HASHMAPSIZE];
};



typedef struct numberedTimedLinkedItem          orderedItem;
typedef struct numberedTimedItemQueue           orderedItemQueue;
typedef struct numberedTimedItemQueueList       orderedItemQueueList;
typedef struct numberedTimedItemQueueMap        orderedItemQueueMap;



int             readOrder(orderedItem *item, int time, recipiesMap *book);
void            printOrder(orderedItem *item);
void            removeOrderFromPending(orderedItem *current, orderedItem *prev, orderedItemQueue *ordersWaiting);
void            addOrderToReady(orderedItem *item, orderedItemQueue *ordersReady);
void            fulfillOrdersPending(warehouseMap *map, warehouseTreeNode **root, orderedItemQueue *ordersPending, orderedItemQueue *ordersReady);


#endif