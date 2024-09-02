#ifndef TIMEDITEMS_H_INCLUDED
#define TIMEDITEMS_H_INCLUDED

#include "String.h"

struct numberedTimedItem{
    char    *name;
    int     amount;
    int     time;
    int     totalWeigth;
};


struct numberedTimedItemList{
    struct numberedTimedItem        *el;
    struct numberedTimedItemList    *next;
};

#endif