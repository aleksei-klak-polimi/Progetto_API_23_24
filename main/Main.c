#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libraries/String.h"
#include "libraries/Map.h"
#include "libraries/Recipies.h"


//STRUCTURES

struct numberedTimedItem{
    String  name;
    int     amount;
    int     time;
    int     totalWeigth;
};


struct numberedTimedItemList{
    struct numberedTimedItem        *el;
    struct numberedTimedItemList    *next;
};

struct numberedTimedItemQueue{
    String                              ingredient;
    struct numberedTimedItemList        *head;
    struct numberedTimedItemList        *tail;
};

struct numberedTimedItemListList{
    int                                 totalAmount;
    struct numberedTimedItemList        *el;
    struct numberedTimedItemListList    *next;
};


struct numberedTimedItemQueueList{
    struct numberedTimedItemQueue       *el;
    struct numberedTimedItemQueueList   *next;
};


struct numberedTimedItemListMap{
    struct numberedTimedItemListList *hashArray[HASHMAPSIZE];
};


struct numberedTimedItemQueueMap{
    struct numberedTimedItemQueueList *hashArray[HASHMAPSIZE];
};


struct numberedTimedItemListTree{
    int         expiration;
    StringList  *ingredients;

    int                              isBlack;
    struct numberedTimedItemListTree *parent;
    struct numberedTimedItemListTree *right;
    struct numberedTimedItemListTree *left;
};


struct Courier{
    int                             frequency;
    int                             capacity;
    struct numberedTimedItemList    *ordersHead;
};





//TYPEDEFS
typedef struct numberedTimedItem                orderedItem;
typedef struct numberedTimedItemList            orderedItemList;
typedef struct numberedTimedItemQueue           orderedItemQueue;
typedef struct numberedTimedItemQueueList       orderedItemQueueList;
typedef struct numberedTimedItemQueueMap        orderedItemQueueMap;

typedef struct numberedTimedItem                ingredientLot;
typedef struct numberedTimedItemList            ingredientLotList;
typedef struct numberedTimedItemListList        ingredientLotListList;
typedef struct numberedTimedItemListMap         warehouseMap;
typedef struct numberedTimedItemListTree        warehouseTreeNode;

warehouseTreeNode NIL_NODE = {0, NULL, 1, NULL, NULL, NULL};
warehouseTreeNode *NIL = &NIL_NODE;

typedef struct Courier                          Courier;





//METHOD DECLARATION
//INSTRUCTIONS
int             addRecipie(recipiesMap *book);
int             removeRecipie(recipiesMap *book);
int             resupply(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueueMap *ordersByIngredient, orderedItemQueue *ordersPending, orderedItemQueue *ordersReady);
int             order(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueue *ordersReady, orderedItemQueue *ordersWaiting, orderedItemQueueMap *ordersByIngredient, int time);
void            loadCourier(Courier *courier, recipiesMap *book, orderedItemQueue *ordersReady);

//SUPPLIES
void            updateInventory(warehouseMap *map, warehouseTreeNode *root);
void            supplyInventory(warehouseMap *map, warehouseTreeNode *root, ingredientLotList *s);
void            addIngredientToTree(warehouseTreeNode **d_root, ingredientLot *s);
StringList      *removeNodeFromTreeByTime(warehouseTreeNode **d_root, int time);
void            deleteNodeFromTree(warehouseTreeNode **root, warehouseTreeNode *node);
void            rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x);
void            rebalanceTreeAfterInsertion(warehouseTreeNode **root, warehouseTreeNode *x);
void            leftRotate(warehouseTreeNode **root, warehouseTreeNode *x);
void            rightRotate(warehouseTreeNode **root, warehouseTreeNode *x);
int             readSupplies(ingredientLotList *s);
void            addIngredientToMap(warehouseMap *map, ingredientLot*s);
void            removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient);
void            removeNodeFromIngredientMap(warehouseMap *map, int hash, ingredientLotListList *hashHead, ingredientLotListList *prevHashHead, ingredientLotList *ingredientHead, ingredientLotList *prevIngredientHead);
void            treeTransplant(warehouseTreeNode **root, warehouseTreeNode *u, warehouseTreeNode *v);
void            removeIngredientFromTreeByTime(warehouseTreeNode **d_root, int time, String ingredient);
int             removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity);
void            removeIngredientsFromWarehouseByTime(warehouseTreeNode **root, warehouseMap *map, int time);

//ORDERS
int             readOrder(orderedItem *item, int time);
void            printOrder(orderedItem *item);
void            addOrderToIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead);
void            removeOrderFromIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead);
void            removeOrderFromPending(orderedItem *item, orderedItemQueue *ordersWaiting);
void            addOrderToReady(orderedItem *item, orderedItemQueue *ordersReady);

//COURIER
int             setupCourier(Courier *c);
void            loadCourier(Courier *courier, recipiesMap *book, orderedItemQueue *ordersReady);
void            printCourierContents(Courier *courier);
void            clearCourierOrdersMemory(Courier *courier);

//DEBUG PRINTS
void            printSupplies(ingredientLotList *s);
void            printRBTree(warehouseTreeNode *node, int level);
void            printSpaces(int count);
void            printRecipie(recipie *r);
void            printRecipieBook(recipiesMap *book);
void            printOrderQueue(orderedItemQueue *queue);
void            printOrdersByIngredientMap(orderedItemQueueMap *ordersByIngredient);




int main(){
    int ch = 0;
    int time = 0;
    int i = 0;
    String command;

    recipiesMap         *book = malloc(sizeof(*book));
    warehouseMap        *whMap = malloc(sizeof(*whMap));

    orderedItemQueueMap *ordersByIngredientsMap = malloc(sizeof(*ordersByIngredientsMap));
    orderedItemQueue    *ordersReady = malloc(sizeof(*ordersReady));
    orderedItemQueue    *ordersPending = malloc(sizeof(*ordersPending));

    warehouseTreeNode   *rootNode = NULL;
    warehouseTreeNode   **root = &rootNode;
    Courier             *courier = malloc(sizeof(*courier));

    //initialize the cookBook to have all entries NULL.
    for(i = 0; i < HASHMAPSIZE; i++){
        book->hashArray[i] = NULL;
        whMap->hashArray[i] = NULL;
        ordersByIngredientsMap->hashArray[i] = NULL;
    }
    ordersReady->head = NULL;
    ordersReady->tail = NULL;
    
    ordersPending->head = NULL;
    ordersPending->tail = NULL;

    courier->ordersHead = NULL;
    //todo finish initializing other structures


    //main while loop
    while(ch != EOF){
        //debug prints
        /*printf("\n\n\nWarehouse Tree contents:\n");
        printRBTree(*root, 0);

        printf("\nOrders ready:\n");
        printOrderQueue(ordersReady);

        printf("\nOrders pending:\n");
        printOrderQueue(ordersPending);

        printf("\n\n\n");*/



        //SETUP COURIER
        if(time == 0){
            //Set up courier
            ch = setupCourier(courier);
        }
        
        //Remove expired ingredients
        removeIngredientsFromWarehouseByTime(root, whMap, time);

        //CHECK IF COURIER TIME
        if(time != 0 && time % courier->frequency == 0){
            //debug print
            /*printf("\nReady orders queue contents:\n");
            printOrderQueue(ordersReady);
            printf("\n");*/


            loadCourier(courier, book, ordersReady);
            printCourierContents(courier);
            clearCourierOrdersMemory(courier);
        }





        //RESET ANY PREVIOUS COMMAND
        for(i = 0; i < STRINGSIZE; i++){
            command[i] = '\0';
        }

        //READ NEW COMMAND
        i = 0;
        while((ch = fgetc(stdin)) != ' ' && ch != EOF && ch != '\n'){
            command[i] = ch;
            i++;
        }
        command[i] = '\0';

        //INTERPRET THE COMMAND
        if(strcmp("aggiungi_ricetta", command) == 0){
            addRecipie(book);

            //debug
            //printRecipieBook(book);
        }
        else if(strcmp("rimuovi_ricetta", command) == 0){
            removeRecipie(book);

            //debug
            //printRecipieBook(book);
        }
        else if(strcmp("rifornimento", command) == 0){
            resupply(whMap, root, book, ordersByIngredientsMap, ordersPending, ordersReady);

            //debug
            //printRBTree(*root, 0);
        }
        else if(strcmp("ordine", command) == 0){
            order(whMap, root, book, ordersReady, ordersPending, ordersByIngredientsMap, time);
        }
        
        time++;

        /*printf("\nDebug tree print:\n");
        printRBTree(*root, 0);*/

        /*printf("\nDebug map by ingredients print:\n");
        printOrdersByIngredientMap(ordersByIngredientsMap);*/
    }

    return 0;
}






int addRecipie(recipiesMap *book){
    //calls function to read recipie from input buffer
    //then calls function to add recipie to recipie book
    //malloc for recipie.
    int ch;
    recipie *r = malloc(sizeof(*r));
    ch = readRecipie(r);
    insertRecipie(book, r);

    return ch;
}

int resupply(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueueMap *ordersByIngredient, orderedItemQueue *ordersPending, orderedItemQueue *ordersReady){
    int ch;

    ingredientLotList *lot;
    lot = malloc(sizeof(*lot));

    ch = readSupplies(lot);

    ingredientLotList *IngredientLotNavigator = lot;
    while(IngredientLotNavigator != NULL){
        addIngredientToTree(root, IngredientLotNavigator->el);
        addIngredientToMap(map, IngredientLotNavigator->el);
        IngredientLotNavigator = IngredientLotNavigator->next;
    }

    //For each ingredient added with resupply try to fulfill pending orders from Ingredient map
    IngredientLotNavigator = lot;
    int hash;
    orderedItemQueueList *hashHead;
    orderedItemList *orderListNavigator = NULL;
    orderedItem *item;
    recipie *recipie;

    //Queue That stores all the orders that depend on the ingredients in this resupply batch ordered
    //By time of order
    orderedItemQueue *affectedOrdersQueue = malloc(sizeof(*affectedOrdersQueue));
    affectedOrdersQueue->head = NULL;
    affectedOrdersQueue->tail = NULL;

    while(IngredientLotNavigator != NULL){
        //Cycle all ingredients in the resupply chain
        hash = sdbm_hash(IngredientLotNavigator->el->name);

        hashHead = ordersByIngredient->hashArray[hash];

        if(hashHead != NULL){
            int breaker = 0;
            while(breaker == 0){
                //Find list of orders associated with current ingredient
                if(strcmp(hashHead->el->ingredient, IngredientLotNavigator->el->name) == 0){
                    breaker = 1;
                    orderListNavigator = hashHead->el->head;
                }
                else{
                    if(hashHead->next != NULL){
                        hashHead = hashHead->next;
                    }
                    else{
                        breaker = 1;
                    }
                }
            }
        }

        while(orderListNavigator != NULL){
            //Add each order to the queue of affected orders sorted by date of order           
            if(affectedOrdersQueue->head == NULL){
                //insert item into queue head

                affectedOrdersQueue->head = malloc(sizeof(*orderListNavigator));
                affectedOrdersQueue->tail = affectedOrdersQueue->head;
                affectedOrdersQueue->head->el = orderListNavigator->el;
                affectedOrdersQueue->head->next = NULL;
            }
            else{
                if(affectedOrdersQueue->tail->el->time < orderListNavigator->el->time){
                    //If item is older than the tail, place it as the new tail
                    affectedOrdersQueue->tail->next = malloc(sizeof(*orderListNavigator));
                    affectedOrdersQueue->tail->next->el = orderListNavigator->el;
                    affectedOrdersQueue->tail->next->next = NULL;
                    affectedOrdersQueue->tail = affectedOrdersQueue->tail->next;
                }
                else if(affectedOrdersQueue->head->el->time > orderListNavigator->el->time){
                    //Item is younger than the queue's head, replace the head
                    orderedItemList *newHead = malloc(sizeof(*newHead));
                    newHead->el = orderListNavigator->el;
                    newHead->next = affectedOrdersQueue->head;
                    affectedOrdersQueue->head = newHead;
                }
                else{
                    //item has to be placed somewhere inside the queue, navigate the queue and splice in the item
                    orderedItemList *affectedQueueNavigator = affectedOrdersQueue->head;

                    int breaker = 0;
                    while(breaker == 0){
                        if(affectedQueueNavigator->el == orderListNavigator->el){
                            //item is already queued, no need to perform additional actions
                            breaker = 1;
                        }
                        //If next item is older than the one we are splicing then splice our between current and next
                        else if(affectedQueueNavigator->next->el->time > orderListNavigator->el->time){
                            breaker = 1;

                            orderedItemList *newNode = malloc(sizeof(*newNode));
                            newNode->el = orderListNavigator->el;
                            newNode->next = affectedQueueNavigator->next;
                            affectedQueueNavigator->next = newNode;

                        }
                        else{
                            //Go to next node
                            affectedQueueNavigator = affectedQueueNavigator->next;
                        }                        
                    }
                }
            }
            orderListNavigator = orderListNavigator->next;
        }
        orderListNavigator = NULL;

        IngredientLotNavigator = IngredientLotNavigator->next;
    }

    //Try to fulfill the queue and free each node after attempted to fulfill

    orderedItemList *affectedQueueNavigator = affectedOrdersQueue->head;
    orderedItemList *prevNode = NULL;

    while(affectedQueueNavigator != NULL){
        recipie = retrieveRecipie(book, affectedQueueNavigator->el->name);

        if(removeIngredientsFromWarehouseByOrder(root, map, recipie, affectedQueueNavigator->el->amount) == 1){
            //Order was fulfilled, remove from pending and from map and add to orders ready

            item = affectedQueueNavigator->el;

            removeOrderFromIngredientMap(item, ordersByIngredient, recipie->head);
            removeOrderFromPending(item, ordersPending);
            addOrderToReady(item, ordersReady);
        }

        prevNode = affectedQueueNavigator;
        affectedQueueNavigator = affectedQueueNavigator->next;

        free(prevNode);
    }

    free(affectedOrdersQueue);
            




    //Clean the list used to store the lots that were read
    ingredientLotList *curr = lot;
    ingredientLotList *next;

    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }


    printf("rifornito\n");

    return ch;
}





//RED-BLACK TREE WAREHOUSE FUNCTIONS
void addIngredientToTree(warehouseTreeNode **d_root, ingredientLot *s){
    /*  Using double pointer to reference the root because:
        If the tree is still empty then we have to do a malloc
        The malloc assign to the pointer a new memory address
        If we give a simple pointer and malloc, then the caller function has no access to the new malloc memory
        If we have a pointer that points to the head then we can malloc the head and the caller still has the reference to
        the double pointer therefore sees the new malloc-ed memory.
    */


    if(*d_root == NULL){
        //If the double pointer points to NULL then we need to initialize the tree with a root.
        warehouseTreeNode *root = malloc(sizeof(*root));
        *d_root = root;

        root->right = NIL;
        root->left = NIL;
        root->parent = NIL;
        root->expiration = s->time;
        root->isBlack = 1;
        
        StringList *ingredients = malloc(sizeof(*ingredients));
        strcpy(ingredients->el, s->name);
        ingredients->next = NULL;
        root->ingredients = ingredients;
    }
    else{
        //The double pointer points to a valid root therefore we need to insert into the tree

        warehouseTreeNode *y = NIL;
        warehouseTreeNode *x = *d_root;
        warehouseTreeNode *z;

        int duplicate = 0;

        while(x != NIL){
            y = x;
            if(s->time < x->expiration){
                x = x->left;
            }
            else if(s->time > x->expiration){
                x = x->right;
            }
            else if(s->time == x->expiration){
                //Node already exists
                duplicate = 1;
                StringList *ingredients = x->ingredients;

                int breaker = 0;
                while(breaker == 0){
                    if(strcmp(ingredients->el, s->name) == 0){
                        breaker = 1;
                    }
                    else if(ingredients->next == NULL){
                        StringList *node = malloc(sizeof(*node));
                        strcpy(node->el, s->name);
                        node->next = NULL;
                        ingredients->next = node;

                        breaker = 1;
                    }
                    else{
                        ingredients = ingredients->next;
                    }
                }

                break;
            }

        }

        if(duplicate == 0){
            z = malloc(sizeof(*z));
            z->expiration = s->time;
            z->ingredients = malloc(sizeof(*(z->ingredients)));
            z->ingredients->next = NULL;
            strcpy(z->ingredients->el, s->name);

            z->parent = y;
            if(y == NIL){
                *d_root = z;
            }

            else if(z->expiration < y->expiration){
                y->left = z;
            }
            else{
                y->right = z;
            }

            z->left = NIL;
            z->right = NIL;
            z->isBlack = 0;

            rebalanceTreeAfterInsertion(d_root, z);
        }
    }
}

void removeIngredientsFromWarehouseByTime(warehouseTreeNode **root, warehouseMap *map, int time){
    //Removes all ingredients with expiration at the provided time from both Map and tree
    StringList *ingredients = removeNodeFromTreeByTime(root, time);

    StringList *prev;
    while(ingredients != NULL){
        removeIngredientFromMapByTime(map, time, ingredients->el);
        prev = ingredients;
        ingredients = ingredients->next;
        free(prev);
    }
}

int removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity){
    //Returns 0 if there were less ingredients available than specified amount and nothing is deleted
    //Returns 1 if the ingredients were successfully removed

    //Check if there are enough ingredients for recipie
    ingredientList *firstIngredientNode = recipie->head;
    ingredientList *currentIngredientNode = firstIngredientNode;
    ingredient *ingredient;

    int hash;

    ingredientLotListList *hashHead;

    //check for each ingredient if there are enough in storage
    while(currentIngredientNode != NULL){
        ingredient = currentIngredientNode->el;
        hash = sdbm_hash(ingredient->name);

        hashHead = map->hashArray[hash];

        int breaker = 0;
        while(breaker == 0){
            if(hashHead == NULL){
                return 0;
            }
            if(strcmp(hashHead->el->el->name, ingredient->name) == 0){
                breaker = 1;
                if(hashHead->totalAmount < (ingredient->amount *quantity)){
                    return 0;
                }
            }
            else if(hashHead->next == NULL){
                return 0;
            }
            else{
                hashHead = hashHead->next;
            }
        }
        currentIngredientNode = currentIngredientNode->next;
    }
    //If function has not returned at this point then each ingredient of the recipie is found in the warehouse with enough amount


    currentIngredientNode = firstIngredientNode;
    int totalAmount = currentIngredientNode->el->amount *quantity;

    ingredientLotListList *prevHashHead = NULL;
    ingredientLotList *ingredientHead;
    ingredientLotList *prevIngredientHead = NULL;

    while(currentIngredientNode != NULL){
        //Locate the hashHead
        hash = sdbm_hash(currentIngredientNode->el->name);
        hashHead = map->hashArray[hash];
        ingredient = currentIngredientNode->el;

        int breaker = 0;
        while(breaker == 0){
            if(strcmp(hashHead->el->el->name, ingredient->name) == 0){
                breaker = 1;
            }
            else{
                    prevHashHead = hashHead;
                    hashHead = hashHead->next;
                }
        }
        //HashHead located
        ingredientHead = hashHead->el;

        //Trim the list of Lots
        while(totalAmount > 0){
            if(totalAmount >= ingredientHead->el->amount){
                //Need to delete the whole node as all ingredients are used

                //Decrease remaining amount
                totalAmount -= ingredientHead->el->amount;

                //Go to next node
                prevIngredientHead = ingredientHead;
                ingredientHead = ingredientHead->next;

                //Delete prev node from tree and Map
                removeIngredientFromTreeByTime(root, prevIngredientHead->el->time, prevIngredientHead->el->name);
                removeNodeFromIngredientMap(map, hash, hashHead, prevHashHead, prevIngredientHead, NULL);
            }
            else{
                //Ingredients will be left over so no need to delete the node, only decrease the amount in hashHead
                hashHead->totalAmount -= totalAmount;
                totalAmount = 0;
            }
        }

        currentIngredientNode = currentIngredientNode->next;
    }
    return 1;
}

StringList *removeNodeFromTreeByTime(warehouseTreeNode **d_root, int time){
    if(*d_root == NULL){
        return NULL;
    }

    StringList *returnList;
    warehouseTreeNode *leaf = *d_root;
    int breaker = 0;
    int removeLeaf = 0;
    while(breaker == 0){
        if(leaf->expiration == time){
            returnList = leaf->ingredients;
            removeLeaf = 1;
            breaker = 1;
        }
        else if(leaf->expiration > time){
            if(leaf->left == NULL){
                return NULL;
            }
            else{
                leaf = leaf->left;
            }
        }
        else if(leaf->expiration < time){
            if(leaf->right == NULL){
                return NULL;
            }
            else{
                leaf = leaf->right;
            }
        }
    }
    if(removeLeaf == 1){
        deleteNodeFromTree(d_root, leaf);
    }

    return returnList;
}

void removeIngredientFromTreeByTime(warehouseTreeNode **d_root, int time, String ingredient){
    //Finds the node in the tree associated to the time provided, and within the node deletes the
    //specified ingredient, if no ingredients are left afterwards then the node is deleted.
    //Frees from memory the string in the node

    int breaker = 0;
    warehouseTreeNode *node = *d_root;
    while(breaker == 0){
        if(node->expiration == time){
            breaker = 1;
        }
        else if(node->expiration > time){
            node = node->left;
        }
        else if(node->expiration < time){
            node = node->right;
        }
    }

    StringList *ingredients = node->ingredients;
    if(strcmp(ingredients->el, ingredient) == 0 && ingredients->next == NULL){
        //If the ingredient is the only ingredient in the node
        free(ingredients);
        deleteNodeFromTree(d_root, node);
    }
    else if(strcmp(ingredients->el, ingredient) == 0){
        //If the ingredient is the first in the node but it's not the only one
        node->ingredients = node->ingredients->next;
        free(ingredients);
    }
    else{
        while(strcmp(ingredients->next->el, ingredient) != 0){
            ingredients = ingredients->next;
        }

        StringList *ingredientFound = ingredients->next;
        ingredients->next = ingredientFound->next;
        free(ingredientFound);
    }
}

void removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient){
    int hash = sdbm_hash(ingredient);
    ingredientLotListList *hashHead = map->hashArray[hash];
    ingredientLotListList *prevHashHead = NULL;
    ingredientLotList *ingredientHead = NULL;
    ingredientLotList *prevIngredientHead = NULL;

    while(ingredientHead == NULL){
        if(strcmp(hashHead->el->el->name, ingredient) == 0){
            ingredientHead = hashHead->el;
        }
        else{
            prevHashHead = hashHead;
            hashHead = hashHead->next;
        }
    }

    while(ingredientHead->el->time != time){
        prevIngredientHead = ingredientHead;
        ingredientHead = ingredientHead->next;
    }
    removeNodeFromIngredientMap(map, hash, hashHead, prevHashHead, ingredientHead, prevIngredientHead);
}

void removeNodeFromIngredientMap(warehouseMap *map, int hash, ingredientLotListList *hashHead, ingredientLotListList *prevHashHead, ingredientLotList *ingredientHead, ingredientLotList *prevIngredientHead){
    hashHead->totalAmount -= ingredientHead->el->amount;
    if(prevIngredientHead != NULL){
        prevIngredientHead->next = ingredientHead->next;
    }
    else if(ingredientHead->next != NULL){
        hashHead->el = ingredientHead->next;
    }
    else{
        //Ingredient removed was the only ingredient in the list
        //Cleaning ListList
        if(prevHashHead != NULL){
            prevHashHead->next = hashHead->next;
        }
        else{
            map->hashArray[hash] = hashHead->next;
        }
        free(hashHead);
    }
    free(ingredientHead->el);
    free(ingredientHead);
}

void deleteNodeFromTree(warehouseTreeNode **root, warehouseTreeNode *node){
    //STEP 1: perform simple deletion
    warehouseTreeNode *z = node;
    warehouseTreeNode *y = node;
    warehouseTreeNode *x;
    int yOrignialColor = y->isBlack;

    if(z->left == NIL){
        x = z->right;
        treeTransplant(root, z, z->right);
    }
    else if(z->right == NIL){
        x = z->left;
        treeTransplant(root, z, z->left);
    }
    else{
        //finding smallest successor of z
        y = z->right;
        while(y->left != NIL){
            y = y->left;
        }


        yOrignialColor = y->isBlack;
        x = y->right;

        if(y->parent == z){
            x->parent = y;
        }
        else{
            treeTransplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        treeTransplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->isBlack = z->isBlack;
    }
    if(yOrignialColor == 1){
        rebalanceTreeAfterDelete(root, x);
    }
}

void rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x){
    // While x is not the root and x is black
    while (x != *root && x->isBlack == 1) {
        if (x == x->parent->left) {                                         // x is the left child
            warehouseTreeNode *w = x->parent->right;                        // w is x's sibling
            if (w->isBlack == 0) {                                          // Case 1: x's sibling w is red
                w->isBlack = 1;                                             // Recolor w as black
                x->parent->isBlack = 0;                                     // Recolor x's parent as red
                leftRotate(root, x->parent);                             // Left rotate on x's parent
                w = x->parent->right;                                       // Update w to be the new sibling of x
            }
            if (w->left->isBlack == 1 && w->right->isBlack == 1) {          // Case 2: Both of w's children are black
                w->isBlack = 0;                                             // Recolor w as red
                x = x->parent;                                              // Move x up the tree
            }
            else {
                if (w->right->isBlack == 1) {                               // Case 3: w's right child is black
                    w->left->isBlack = 1;                                   // Recolor w's left child as black
                    w->isBlack = 0;                                         // Recolor w as red
                    rightRotate(root, w);                                // Right rotate on w
                    w = x->parent->right;                                   // Update w to be the new sibling of x
                }
                                                                            // Case 4: w's right child is red
                w->isBlack = x->parent->isBlack;                            // Make w the same color as x's parent
                x->parent->isBlack = 1;                                     // Recolor x's parent as black
                w->right->isBlack = 1;                                      // Recolor w's right child as black 
                leftRotate(root, x->parent);                             // Left rotate on x's parent
                x = *root;                                                  // Set x to root to end loop
            }
        } else {                                                            // Symmetric case: x is the right child
            warehouseTreeNode *w = x->parent->left;
            if (w->isBlack == 0) {                                          // Case 1: x's sibling w is red
                w->isBlack = 1;
                x->parent->isBlack = 0;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->left->isBlack == 1 && w->right->isBlack == 1) {          // Case 2: Both of w's children are black
                w->isBlack = 0;
                x = x->parent;
            }
            else {
                if (w->left->isBlack == 1) {                                // Case 3: w's left child is black
                    w->right->isBlack = 1;
                    w->isBlack = 0;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                // Case 4: w's left child is red
                w->isBlack = x->parent->isBlack;
                x->parent->isBlack = 1;
                w->left->isBlack = 1;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->isBlack = 1; // Ensure the root node is always black
}

void rebalanceTreeAfterInsertion(warehouseTreeNode **root, warehouseTreeNode *z){
    warehouseTreeNode *y;

    while(z->parent->isBlack == 0){
        if(z->parent == z->parent->parent->left){
            y = z->parent->parent->right;

            if(y->isBlack == 0){
                z->parent->isBlack = 1;
                y->isBlack = 1;
                z->parent->parent->isBlack = 0;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->right){
                    z = z->parent;
                    leftRotate(root, z);
                }

                z->parent->isBlack = 1;
                z->parent->parent->isBlack = 0;
                rightRotate(root, z->parent->parent);
            }
        }
        else{
            y = z->parent->parent->left;

            if(y->isBlack == 0){
                z->parent->isBlack = 1;
                y->isBlack = 1;
                z->parent->parent->isBlack = 0;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->left){
                    z = z->parent;
                    rightRotate(root, z);
                }

                z->parent->isBlack = 1;
                z->parent->parent->isBlack = 0;
                leftRotate(root, z->parent->parent);
            }
        }
        (*root)->isBlack = 1;
    }
}

void leftRotate(warehouseTreeNode **root, warehouseTreeNode *x) {
    warehouseTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(warehouseTreeNode **root, warehouseTreeNode *y) {
    warehouseTreeNode *x = y->left;
    y->left = x->right;
    if (x->right != NIL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NIL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void treeTransplant(warehouseTreeNode **root, warehouseTreeNode *u, warehouseTreeNode *v){
    if(u->parent == NIL){
        *root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void addIngredientToMap(warehouseMap *map, ingredientLot*s){
    int hash = sdbm_hash(s->name);

    if(map->hashArray[hash] == NULL){
        //If spot is null then create new hashHead and new ingredientHead
        ingredientLotListList *hashHead = malloc(sizeof(*hashHead));
        ingredientLotList *ingredientHead = malloc(sizeof(*ingredientHead));

        hashHead->el = ingredientHead;
        hashHead->totalAmount = s->amount;
        hashHead->next = NULL;

        ingredientHead->el = s;
        ingredientHead->next = NULL;

        map->hashArray[hash] = hashHead;
    }
    else{
        //The hashHead already existed, locate ingredientHead
        int ingredientHeadFound = 0;
        int endOfList = 0;
        ingredientLotListList *hashHead = map->hashArray[hash];
        ingredientLotList *ingredientHead;

        do{
            if(strcmp(hashHead->el->el->name, s->name) == 0){
                ingredientHead = hashHead->el;
                ingredientHeadFound = 1;
            }
            else if(hashHead->next != NULL){
                hashHead = hashHead->next;
            }
            else{
                endOfList = 1;
            }
        }
        while(endOfList == 0 && ingredientHeadFound == 0);

        if(ingredientHeadFound == 0){
            hashHead->next = malloc(sizeof(*hashHead));
            hashHead = hashHead->next;

            ingredientHead = malloc(sizeof(*ingredientHead));

            hashHead->el = ingredientHead;
            hashHead->totalAmount = s->amount;
            hashHead->next = NULL;

            ingredientHead->el = s;
            ingredientHead->next = NULL;
        }
        else{
            //HashHead points to the beginning of the correct ingredientHead
            //Need to locate correct position inside ingredientList
            hashHead->totalAmount += s->amount;
            ingredientLotList *currentIngredientLot = hashHead->el;

            if(currentIngredientLot->el->time == s->time){
                //A lot with same expiration is at the head of the list

                //increment amount in the node
                currentIngredientLot->el->amount += s->amount;

                free(s);
            }
            else if(currentIngredientLot->el->time > s->time){
                //Head of ingredients list is greater than new ingredient lot
                hashHead->el = malloc(sizeof(*currentIngredientLot));
                hashHead->el->el = s;
                hashHead->el->next = currentIngredientLot;
            }
            else{
                //Navigate the ingredient list and stop on the first node where
                //next node expires later than s

                int nodeFound = 0;
                do{
                    if(currentIngredientLot->next == NULL || currentIngredientLot->next->el->time > s->time){
                        nodeFound = 1;
                    }
                    else{
                        currentIngredientLot = currentIngredientLot->next;
                    }
                }
                while(nodeFound == 0);

                if(!(currentIngredientLot->el->time == s->time)){
                    ingredientLotList *newLot = malloc(sizeof(*newLot));
                    newLot->el = s;
                    newLot->next = currentIngredientLot->next;
                    currentIngredientLot->next = newLot;
                }
                else{
                    currentIngredientLot->el->amount += s->amount;
                    free(s);
                }
            }
        }
    }
}





//ORDER HANDLING
int order(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueue *ordersReady, orderedItemQueue *ordersWaiting, orderedItemQueueMap *ordersByIngredient, int time){
    int ch;

    orderedItem *item = malloc(sizeof(*item));
    ch = readOrder(item, time);

    recipie *recipie = retrieveRecipie(book, item->name);

    if(recipie == NULL){
        //No matching recipie was found, order refused and cleared from memory
        printf("rifiutato\n");
        free(item);
    }
    else{
        //mark recipie as "orders relying on recipie"
        incrementRecipieUtilization(book, recipie->name);

        //Calcuate order weigth
        //todo store recipie weight in recipie struct
        ingredientList *ingredientNode = recipie->head;
        int weigth = 0;
        while(ingredientNode != NULL){
            weigth += ingredientNode->el->amount;
            ingredientNode = ingredientNode->next;
        }
        item->totalWeigth = weigth * item->amount;

        //Handle the rest of the order procedures
        orderedItemList *orderNode = malloc(sizeof(*orderNode));
        orderNode->el = item;
        orderNode->next = NULL;

        if(removeIngredientsFromWarehouseByOrder(root, map, recipie, item->amount) == 1){
            //The order was processed immediately, adding to orders ready
            if(ordersReady->head == NULL){
                ordersReady->head = orderNode;
                ordersReady->tail = orderNode;
            }
            else{
                ordersReady->tail->next = orderNode;
                ordersReady->tail = orderNode;
            }
        }
        else{
            //The order could not be processed due to lacking ingredients, adding order to waiting queue and to ingredientMap
            if(ordersWaiting->head == NULL){
                ordersWaiting->head = orderNode;
            }
            if(ordersWaiting->tail != NULL){
                ordersWaiting->tail->next = orderNode;
            }
            ordersWaiting->tail = orderNode;


            addOrderToIngredientMap(item, ordersByIngredient, recipie->head);
        }

        printf("accettato\n");
    }

    return ch;
}

void addOrderToIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead){

    ingredientList *ingredientNode = ingredientsHead;
    int hash;
    orderedItemQueueList *hashHead;
    orderedItemQueue *ordersQueue;
    orderedItemList *orderNode;

    while(ingredientNode != NULL){
        hash = sdbm_hash(ingredientNode->el->name);

        hashHead = ordersByIngredient->hashArray[hash];
        
        if(hashHead == NULL){
            //Hash head is empty, need to create new list of queues

            hashHead = malloc(sizeof(*hashHead));
            ordersByIngredient->hashArray[hash] = hashHead;
        
            ordersQueue = malloc(sizeof(*ordersQueue));
            hashHead->el = ordersQueue;
            hashHead->next = NULL;

            orderNode = malloc(sizeof(*orderNode));
            orderNode->el = item;
            orderNode->next = NULL;

            ordersQueue->head = orderNode;
            ordersQueue->tail = orderNode;
            strcpy(ordersQueue->ingredient, ingredientNode->el->name);
        }
        else{
            //There already is a hashHead, try to locate the correct queue
            ordersQueue = hashHead->el;

            int breaker = 0;
            while(breaker == 0){
                if(strcmp(ordersQueue->ingredient, ingredientNode->el->name) == 0 ){
                    //Correct queue located
                    breaker = 1;

                    orderNode = malloc(sizeof(*orderNode));
                    orderNode->el = item;
                    orderNode->next = NULL;

                    ordersQueue->tail->next = orderNode;
                    ordersQueue->tail = orderNode;
                }
                else if(hashHead->next != NULL){
                    hashHead = hashHead->next;
                }
                else{
                    breaker = 1;

                    //No mathcing hashHead was found, inserting new HashHead
                    hashHead->next = malloc(sizeof(*hashHead));
                    hashHead = hashHead->next;

                    ordersQueue = malloc(sizeof(*ordersQueue));
                    hashHead->el = ordersQueue;
                    hashHead->next = NULL;

                    orderNode = malloc(sizeof(*orderNode));
                    orderNode->el = item;
                    orderNode->next = NULL;

                    ordersQueue->head = orderNode;
                    ordersQueue->tail = orderNode;
                    strcpy(ordersQueue->ingredient, ingredientNode->el->name);
                }
            }
        }

        ingredientNode = ingredientNode->next;
    }
}

void removeOrderFromIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead){
    int hash;

    ingredientList *ingredientNode = ingredientsHead;
    orderedItemQueueList *hashHead, *prevHashHead;
    orderedItemQueue *ordersQueue;
    orderedItemList *orderNode, *prevOrderNode;

    while(ingredientNode != NULL){
        hash = sdbm_hash(ingredientNode->el->name);

        hashHead = ordersByIngredient->hashArray[hash];
        prevHashHead = NULL;

        while(hashHead != NULL){
            ordersQueue = hashHead->el;

            if(strcmp(ordersQueue->ingredient, ingredientNode->el->name) == 0 ){
                // Correct queue located
                orderNode = ordersQueue->head;
                prevOrderNode = NULL;

                int breaker = 0;
                while(orderNode != NULL && breaker == 0){
                    if(orderNode->el == item){
                        // Order found, remove it from the queue
                        if(prevOrderNode == NULL){
                            //Removing the head of the queue
                            ordersQueue->head = orderNode->next;
                        }
                        else{
                            prevOrderNode->next = orderNode->next;
                        }

                        if(orderNode == ordersQueue->tail){
                            // If it's the tail, update the tail pointer
                            ordersQueue->tail = prevOrderNode;
                        }

                        free(orderNode);
                        breaker = 1;
                    }

                    prevOrderNode = orderNode;
                    orderNode = orderNode->next;
                }

                // Check if the queue is empty after removal
                if(ordersQueue->head == NULL){
                    // If queue is empty, remove the queue itself
                    free(ordersQueue);

                    if(prevHashHead == NULL){
                        // Remove the head of the hash list
                        ordersByIngredient->hashArray[hash] = hashHead->next;
                    } else {
                        prevHashHead->next = hashHead->next;
                    }

                    free(hashHead);

                    // If the hashArray slot is empty, set it to NULL
                    if(ordersByIngredient->hashArray[hash] == NULL){
                        break;
                    }
                }

                // Exit the loop after processing the correct queue
                break;
            }

            // Move to the next hashHead in the list
            prevHashHead = hashHead;
            hashHead = hashHead->next;
        }

         // Move to the next ingredient in the list
        ingredientNode = ingredientNode->next;
    }
}

void removeOrderFromPending(orderedItem *item, orderedItemQueue *ordersWaiting){
    orderedItemList *current = ordersWaiting->head;
    orderedItemList *prev = NULL;

    int breaker = 0;
    orderedItem *storedItem;
    while(breaker == 0){
        storedItem = current->el;
        if(strcmp(storedItem->name, item->name) == 0 && storedItem->amount == item->amount && storedItem->time == item->time){
            //Item found
            breaker = 1;
            if(prev == NULL){
                ordersWaiting->head = current->next;
            }
            else{
                prev->next = current->next;
            }

            free(current);
        }
        else{
            if(current->next == NULL){
                breaker = 1;
            }
            else{
                prev = current;
                current = current->next;
            }
        }
    }
}

void addOrderToReady(orderedItem *item, orderedItemQueue *ordersReady){
    orderedItemList *current = ordersReady->head;
    orderedItemList *prev = NULL;

    orderedItemList *newNode = malloc(sizeof(*newNode));
    newNode->el = item;

    int breaker = 0;
    while(breaker == 0){
        if(ordersReady->head == NULL){
            ordersReady->head = newNode;
            ordersReady->tail = newNode;
            newNode->next = NULL;

            breaker = 1;
        }
        else if(current->el->time <= item->time){
            if(current->next != NULL){
                prev = current;
                current = current->next;
            }
            else{
                breaker = 1;
                ordersReady->tail = newNode;
                current->next = newNode;
                newNode->next = NULL;
            }
        }
        else{
            breaker = 1;
            if(prev == NULL){
                ordersReady->head = newNode;
                newNode->next = current;
            }
            else{
                prev->next = newNode;
                newNode->next = current;
            }
        }
    }
}





//COURIER FUNCTIONS
int setupCourier(Courier *c){
    String input;
    int ch;
    int i = 0;

    while((ch = fgetc(stdin)) != '\n' && ch != EOF){
        input[i] = ch;
        i++;
    }
    input[i] = '\0';

    sscanf(input, "%d %d", &c->frequency, &c-> capacity);

    return ch;
}

void loadCourier(Courier *courier, recipiesMap *book, orderedItemQueue *ordersReady){
    if(ordersReady->head != NULL){
        int currentCourierLoad = 0;

        orderedItemList *orderNode;

        int breaker = 0;
        while(breaker == 0){
            orderNode = ordersReady->head;
            //Check if enough space in Courier
            if(currentCourierLoad + orderNode->el->totalWeigth < courier->capacity){
                //order fits in the courier
                //Load orders sorted by weight descending then by time ascending

                ordersReady->head = ordersReady->head->next;

                currentCourierLoad += orderNode->el->totalWeigth;

                //removing utilization from recipie book
                decrementRecipieUtilization(book, orderNode->el->name);


                if(courier->ordersHead == NULL || courier->ordersHead->el->totalWeigth < orderNode->el->totalWeigth || (courier->ordersHead->el->totalWeigth == orderNode->el->totalWeigth && courier->ordersHead->el->time > orderNode->el->time)){
                    //if no orders are in courier OR first order weighs less than the current order OR they weigh the same but
                    //the current order is fresher then replace the head with the current order
                    orderNode->next = courier->ordersHead;
                    courier->ordersHead = orderNode;
                }
                else{
                    orderedItemList *prev = NULL;
                    orderedItemList *current = courier->ordersHead;

                    int breaker = 0;
                    while(breaker == 0){
                        if(current->el->totalWeigth < orderNode->el->totalWeigth || (current->el->totalWeigth == orderNode->el->totalWeigth && current->el->time > orderNode->el->time)){
                            breaker = 1;
                            orderNode->next = current;

                            if(prev != NULL){
                                prev->next = orderNode;
                            }
                        }
                        else if(current->next == NULL){
                            breaker = 1;
                            current->next = orderNode;
                            orderNode->next = NULL;
                        }
                        else{
                            prev = current;
                            current = current->next;
                        }
                    }
                }

                if(ordersReady->head == NULL){
                    breaker = 1;
                }
            }
            else{
                //order does not fit in the courier, stop loading
                breaker = 1;
            }
        }
    }
}

void printCourierContents(Courier *courier){
    orderedItemList *currentOrder = courier->ordersHead;

    if(courier->ordersHead == NULL){
        printf("camioncino vuoto\n");
    }
    else{
        while (currentOrder != NULL) {
            printf("%d %s %d\n", currentOrder->el->time, currentOrder->el->name, currentOrder->el->amount);
            currentOrder = currentOrder->next;
        }
    }
}

void clearCourierOrdersMemory(Courier *courier){
    if(courier->ordersHead == NULL){
        return;
    }

    orderedItemList *current = courier->ordersHead->next;
    orderedItemList *prev = courier->ordersHead;

    int breaker = 0;
    while(breaker == 0){
        free(prev->el);
        free(prev);

        if(current == NULL){
            breaker = 1;
        }
        else{
            prev = current;
            current = current->next;
        }
    }

    courier->ordersHead = NULL;
}





//PARSING FUNCTIONS
int readRecipie(recipie *r){
    int ch;
    int i = 0;

    //READ NAME
    while((ch = fgetc(stdin)) != '\n' && ch != ' ' && ch != EOF){
        r->name[i] = ch;
        i++;
    }
    r->name[i] = '\0';


    ingredientList *head = 0;
    ingredientList *prev = 0;
    ingredientList *current = 0;
    
    //READ INGREDIENTS
    while(ch != '\n' && ch != EOF){
        ingredient *ingr;
        ingr = malloc(sizeof(*ingr));

        //READ INGREDIENT
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            ingr->name[i] = ch;
            i++;
        }
        ingr->name[i] = '\0';

        //READ AMOUNT
        i = 0;
        String amount;
        while((ch = fgetc(stdin)) != ' ' && ch != '\n' && ch != EOF){
            amount[i] = ch;
            i++;
        }
        amount[i] = '\0';
        ingr->amount = atoi(amount);


        //STORE INGREDIENT IN NODE
        if(current != 0){
            prev = current;
        }
        current = malloc(sizeof(*current));
        current->el = ingr;
        current->next = 0;
        //LINK PREV NODE TO NEW NODE
        if(prev != 0){
            prev->next = current;
        }
        else if(head == 0){
            head = current;
        }
    }

    //ADD LIST OF INGREDIENTS TO RECIPIE
    r->head = head;

    return ch;
}

int readSupplies(ingredientLotList *s){
    int ch = 0;
    int i;
    ingredientLotList *current = s;

    while(ch != '\n' && ch != EOF){


        //ALLOCATE ELEMENT
        ingredientLot *el;
        el = malloc(sizeof(*el));


        //READ INGREDIENT NAME
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            el->name[i] = ch;
            i++;
        }
        el->name[i] = '\0';


        //READ INGREDIENT AMOUNT
        i = 0;
        String amount;
        while((ch = fgetc(stdin)) != ' '){
            amount[i] = ch;
            i++;
        }
        amount[i] = '\0';
        el->amount = atoi(amount);


        //READ INGREDIENT TIME
        i = 0;
        String time;
        while((ch = fgetc(stdin)) != ' ' && ch != '\n' && ch != EOF){
            time[i] = ch;
            i++;
        }
        time[i] = '\0';
        el->time = atoi(time);


        //UPDATE LIST
        current->el = el;
        if(ch == ' '){
            current->next = malloc(sizeof(*current));
            current = current->next;
        }
        else{
            current->next = NULL;
        }
    }

    return ch;
}

int readOrder(orderedItem *item, int time){
    int ch = 0;
    int i;

    while(ch != '\n' && ch != EOF){

        //READ RECIPIE NAME
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            item->name[i] = ch;
            i++;
        }
        item->name[i] = '\0';


        //READ AMOUNT ORDERED
        i = 0;
        String amount;
        while((ch = fgetc(stdin)) != '\n'){
            amount[i] = ch;
            i++;
        }
        amount[i] = '\0';
        item->amount = atoi(amount);


        //ADD INGREDIENT TIME
        item->time = time;
    }
    return ch;
}





//DEBUG PRINTING FUNCTIONS
void printRecipieBook(recipiesMap *book){
    int i;
    for(i = 0; i < HASHMAPSIZE; i++){
        if(book->hashArray[i] != NULL){
            recipiesList *node = book->hashArray[i];

            while(node != NULL){
                printRecipie(node->el);
                node = node->next;
            }
        }
    }
}

void printRecipie(recipie *r){
    printf("\nRecipie name: %s\n", r->name);

    ingredientList *ingr = r->head;

    int i = 0;
    while(ingr != 0){
        printf("Ingredient %d: %s, %d\n", i, ingr->el->name, ingr->el->amount);
        ingr = ingr->next;
        i++;
    }
}

void printSupplies(ingredientLotList *s){
    printf("\nLot:\n");
    
    ingredientLotList *ingr = s;
    while(ingr != NULL){
        printf("Ingredient: %s\nAmount: %d\nExpiration: %d\n", ingr->el->name, ingr->el->amount, ingr->el->time);
        ingr = ingr->next;
    }
}

void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        printf("  ");
    }
}

// Recursive function to print the tree
void printRBTree(warehouseTreeNode *node, int level) {
    if (node == NULL || node == NIL) {
        return;
    }

    // Print the right subtree first (higher values on the right)
    printRBTree(node->right, level + 1);

    // Print current node
    printSpaces(level);  // Indent according to the current level
    printf("%s[Expiration: %d]\n", node->isBlack ? "B" : "R", node->expiration);

    // Print the left subtree
    printRBTree(node->left, level + 1);
}

void printOrder(orderedItem *item){
    printf("Ordered item: %s\n", item->name);
    printf("Amount: %d\n", item->amount);
    printf("Time of order: %d\n", item->time);
    printf("Total weigth: %d\n", item->totalWeigth);
}

void printOrderQueue(orderedItemQueue *queue){
    if(queue->head == NULL){
        printf("Queue is empty\n");
        return;
    }

    printf("queue head:\n");
    printOrder(queue->head->el);

    printf("queue tail:\n");
    printOrder(queue->tail->el);

    orderedItemList *node = queue->head;

    printf("Queue in full:\n");
    int i = 0;
    while(node != NULL){
        printf("Item number %d\n", i);
        printOrder(node->el);
        node = node->next;

        i++;
    }
}

void printOrdersByIngredientMap(orderedItemQueueMap *ordersByIngredient) {
    int i;

    for (i = 0; i < HASHMAPSIZE; i++) {
        if (ordersByIngredient->hashArray[i] != NULL) {
            printf("Pos: %d\n", i);

            orderedItemQueueList *hashHead = ordersByIngredient->hashArray[i];

            int j = 0;
            while (hashHead != NULL) {
                printf("  Head %d contains: ", j);

                if (hashHead->el == NULL) {
                    printf("NULL");
                } else {
                    int k = 0;

                    if (hashHead->el->head == NULL) {
                        printf("0");
                    } else {
                        orderedItemList *node = hashHead->el->head;

                        int breaker = 0;
                        while (breaker == 0) {
                            if(node != NULL){
                                node = node->next;
                                k++;
                            }
                            else{
                                breaker = 1;
                            }
                        }

                        printf("%d", k);
                    }
                }

                printf("\n");  // Add a newline for readability
                hashHead = hashHead->next;  // Move to the next hashHead in the list
                j++;
            }
        }
    }
}