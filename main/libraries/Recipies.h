#ifndef RECIPIES_H_INCLUDED
#define RECIPIES_H_INCLUDED

#include "String.h"
#include "Map.h"
#include "TimedItems.h"

struct numberedItem{
    char                                *name;
    int                                 amount;
    struct numberedTimedItemListList    *ingredientHead;
};


struct numberedItemList{
    struct numberedItem     *el;
    struct numberedItemList *next;
};


struct namedNumberedItemList{
    char                        *name;
    struct numberedItemList     *head;
    int                         weight;
};


struct namedNumberedItemListList{
    int                                 ordersPending;
    struct namedNumberedItemList        *el;
    struct namedNumberedItemListList    *next;
};


struct namedNumberedItemListListMap{
    struct namedNumberedItemListList   *hashArray[HASHMAPSIZE];
};





typedef struct numberedItem                     ingredient;
typedef struct numberedItemList                 ingredientList;
typedef struct namedNumberedItemList            recipie;
typedef struct namedNumberedItemListList        recipiesList;
typedef struct namedNumberedItemListListMap     recipiesMap;



//Parsing function
int             readRecipie(recipiesMap *book, recipie *r, struct numberedTimedItemListMap *whMap);

void            deleteRecipie(recipiesMap book, String name);
void            decrementRecipieUtilization(recipiesMap *book, String name);
void            incrementRecipieUtilization(recipiesMap *book, String name);
recipie         *retrieveRecipie(recipiesMap *book, String name);


#endif