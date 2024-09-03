#ifndef RECIPIES_H_INCLUDED
#define RECIPIES_H_INCLUDED

#include "String.h"
#include "Map.h"
#include "TimedItems.h"
#include "WarehouseMap.h"

struct numberedItem{
    int                                 amount;
    struct numberedTimedLotList         *ingredientHead;
    struct numberedItem                 *next;
};


struct namedNumberedItemList{
    int                                 ordersPending;
    char                                *name;
    struct numberedItem                 *head;
    int                                 weight;
    struct namedNumberedItemList        *next;
};


struct namedNumberedItemListListMap{
    struct namedNumberedItemList        *hashArray[HASHMAPSIZE];
};





typedef struct numberedItem                     ingredient;
typedef struct namedNumberedItemList            recipie;
typedef struct namedNumberedItemListListMap     recipiesMap;



//Parsing function
int             readRecipie(recipiesMap *book, recipie *r, struct numberedTimedLotListMap *whMap);

void            deleteRecipie(recipiesMap book, String name);
void            decrementRecipieUtilization(recipiesMap *book, String name);
void            incrementRecipieUtilization(recipiesMap *book, String name);
recipie         *retrieveRecipie(recipiesMap *book, String name);


#endif