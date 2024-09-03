#ifndef WAREHOUSETREE_H_INCLUDED
#define WAREHOUSETREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "String.h"

struct numberedTimedItemListTree{
    int         expiration;
    StringList  *ingredients;

    int                              isBlack;
    struct numberedTimedItemListTree *parent;
    struct numberedTimedItemListTree *right;
    struct numberedTimedItemListTree *left;
};

typedef struct numberedTimedItemListTree        warehouseTreeNode;

void            treeTransplant(warehouseTreeNode **root, warehouseTreeNode *u, warehouseTreeNode *v);
void            removeIngredientFromTreeByTime(warehouseTreeNode **d_root, int time, char *ingredient);
void            addIngredientToTree(warehouseTreeNode **d_root, char *ingredient, int expiration);
StringList      *removeNodeFromTreeByTime(warehouseTreeNode **d_root, int time);
void            deleteNodeFromTree(warehouseTreeNode **root, warehouseTreeNode *node);
void            rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x);
void            rebalanceTreeAfterInsertion(warehouseTreeNode **root, warehouseTreeNode *x);
void            leftRotate(warehouseTreeNode **root, warehouseTreeNode *x);
void            rightRotate(warehouseTreeNode **root, warehouseTreeNode *x);

void            printRBTree(warehouseTreeNode *node, int level);

#endif