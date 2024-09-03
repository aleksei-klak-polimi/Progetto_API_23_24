#ifndef RECIPIES_H_INCLUDED
#define RECIPIES_H_INCLUDED

#include "String.h"
#include "Map.h"
#include "TimedItems.h"

struct numberedItem{
    int                                 amount;
    struct numberedTimedItemListList    *ingredientHead;
};


struct numberedItemList{
    struct numberedItem     *el;
    struct numberedItemList *next;
};


struct namedNumberedItemList{
    int                                 ordersPending;
    char                                *name;
    struct numberedItemList             *head;
    int                                 weight;
    struct namedNumberedItemList        *next;
};


struct namedNumberedItemListListMap{
    struct namedNumberedItemList        *hashArray[HASHMAPSIZE];
};





typedef struct numberedItem                     ingredient;
typedef struct numberedItemList                 ingredientList;
typedef struct namedNumberedItemList            recipie;
typedef struct namedNumberedItemListListMap     recipiesMap;



//Parsing function
int             readRecipie(recipiesMap *book, recipie *r, struct numberedTimedItemListMap *whMap);

void            deleteRecipie(recipiesMap book, String name);
void            decrementRecipieUtilization(recipiesMap *book, String name);
void            incrementRecipieUtilization(recipiesMap *book, String name);
recipie         *retrieveRecipie(recipiesMap *book, String name);


#endif