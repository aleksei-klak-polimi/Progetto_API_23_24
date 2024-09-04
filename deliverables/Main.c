#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define STRINGSIZE 256
#define HASHMAPSIZE 65536

typedef char String[STRINGSIZE];

typedef struct StringList{
    String              el;
    struct StringList   *next;
}StringList;





//STRUCTS

//Timed Items
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

struct numberedTimedItemListList{
    char                                *ingredientName;
    int                                 totalAmount;
    struct numberedTimedItemList        *el;
    struct numberedTimedItemListList    *next;
};

struct numberedTimedItemListMap{
    struct numberedTimedItemListList *hashArray[HASHMAPSIZE];
};


//WareHouseMap
struct numberedTimedLot{
    int                         amount;
    int                         time;
    struct numberedTimedLot     *next;
};

struct numberedTimedLotList{
    char                                *ingredientName;
    int                                 totalAmount;
    struct numberedTimedLot             *el;
    struct numberedTimedLotList         *next;
};

struct numberedTimedLotListMap{
    struct numberedTimedLotList *hashArray[HASHMAPSIZE];
};


//WareHouseTree
struct numberedTimedItemListTree{
    int         expiration;
    StringList  *ingredients;

    int                              isBlack;
    struct numberedTimedItemListTree *parent;
    struct numberedTimedItemListTree *right;
    struct numberedTimedItemListTree *left;
};


//Recipies
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


//Orders
struct numberedTimedLinkedItem{
    struct namedNumberedItemList    *recipie;
    short int                       amount;
    int                             time;
    int                             totalWeigth;
    struct numberedTimedLinkedItem  *next;
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


//Courier
struct Courier{
    int                                 frequency;
    int                                 capacity;
    struct numberedTimedLinkedItem      *ordersHead;
};





//TYPEDEFS

//WareHouseMap
typedef struct numberedTimedLot                 ingredientLot;
typedef struct numberedTimedLotList             ingredientLotListList;
typedef struct numberedTimedLotListMap          warehouseMap;

//WareHouseTree
typedef struct numberedTimedItemListTree        warehouseTreeNode;
warehouseTreeNode NIL_NODE = {0, NULL, 1, NULL, NULL, NULL};
warehouseTreeNode *NIL = &NIL_NODE;

//Recipies
typedef struct numberedItem                     ingredient;
typedef struct namedNumberedItemList            recipie;
typedef struct namedNumberedItemListListMap     recipiesMap;

//Orders
typedef struct numberedTimedLinkedItem          orderedItem;
typedef struct numberedTimedItemQueue           orderedItemQueue;
typedef struct numberedTimedItemQueueList       orderedItemQueueList;
typedef struct numberedTimedItemQueueMap        orderedItemQueueMap;

//Courier
typedef struct Courier                          Courier;





//METHOD PROTOTYPES

//Map
unsigned int    sdbm_hash(char *string);

//WareHouseMap
void            addIngredientToMap(warehouseMap *map, ingredientLot*s, char *ingredientName);
void            removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient);
void            removeNodeFromIngredientMap(warehouseMap *map, ingredientLotListList *hashHead, ingredientLot *ingredientHead, ingredientLot *prevIngredientHead);

//WareHouseTree
void            treeTransplant(warehouseTreeNode **root, warehouseTreeNode *u, warehouseTreeNode *v);
void            removeIngredientFromTreeByTime(warehouseTreeNode **d_root, int time, char *ingredient);
void            addIngredientToTree(warehouseTreeNode **d_root, char *ingredient, int expiration);
StringList      *removeNodeFromTreeByTime(warehouseTreeNode **d_root, int time);
void            deleteNodeFromTree(warehouseTreeNode **root, warehouseTreeNode *node);
void            rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x);
void            rebalanceTreeAfterInsertion(warehouseTreeNode **root, warehouseTreeNode *x);
void            leftRotate(warehouseTreeNode **root, warehouseTreeNode *x);
void            rightRotate(warehouseTreeNode **root, warehouseTreeNode *x);

//WareHouse
int             resupplyWarehouse(int time, warehouseMap *map, warehouseTreeNode **root);
void            removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity);
void            removeIngredientsFromWarehouseByTime(warehouseTreeNode **root, warehouseMap *map, int time);
int             isOrderFulfillable(recipie *recipie, int quantity);

//Recipies
int             readRecipie(recipiesMap *book, struct numberedTimedLotListMap *whMap);
int             removeRecipie(recipiesMap *book);
recipie         *retrieveRecipie(recipiesMap *book, String name);

//Orders
int             readOrder(orderedItem *item, int time, recipiesMap *book);
void            printOrder(orderedItem *item);
void            removeOrderFromPending(orderedItem *current, orderedItem *prev, orderedItemQueue *ordersWaiting);
void            addOrderToReady(orderedItem *item, orderedItemQueue *ordersReady);
void            fulfillOrdersPending(warehouseMap *map, warehouseTreeNode **root, orderedItemQueue *ordersPending, orderedItemQueue *ordersReady);
int             order(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueue *ordersReady, orderedItemQueue *ordersWaiting, orderedItemQueueMap *ordersByIngredient, int time);

//Courier
int             setupCourier(Courier *c);
void            loadCourier(Courier *courier, orderedItemQueue *ordersReady);
void            printCourierContents(Courier *courier);
void            clearCourierOrdersMemory(Courier *courier);





//MAIN
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


    //main while loop
    while(ch != EOF){

        //SETUP COURIER
        if(time == 0){
            //Set up courier
            ch = setupCourier(courier);
        }
        
        //Remove expired ingredients
        removeIngredientsFromWarehouseByTime(root, whMap, time);

        //CHECK IF COURIER TIME
        if(time != 0 && time % courier->frequency == 0){

            loadCourier(courier, ordersReady);
            printCourierContents(courier);
            clearCourierOrdersMemory(courier);
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
            ch = readRecipie(book, whMap);
        }
        else if(strcmp("rimuovi_ricetta", command) == 0){
            removeRecipie(book);
        }
        else if(strcmp("rifornimento", command) == 0){
            resupplyWarehouse(time, whMap, root);
            fulfillOrdersPending(whMap, root, ordersPending, ordersReady);
        }
        else if(strcmp("ordine", command) == 0){
            order(whMap, root, book, ordersReady, ordersPending, ordersByIngredientsMap, time);
        }
        
        time++;
    }

    return 0;
}





//METHODS


//Map
unsigned int sdbm_hash(char *string){
        
    unsigned long hash = 0;
    int c;

    while((c = *string++)){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash % HASHMAPSIZE;
}


//WareHouseMap
void removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient){
    int hash = sdbm_hash(ingredient);
    ingredientLotListList *hashHead = map->hashArray[hash];
    ingredientLot *ingredientHead = NULL;
    ingredientLot *prevIngredientHead = NULL;

    while(ingredientHead == NULL){
        if(strcmp(hashHead->ingredientName, ingredient) == 0){
            ingredientHead = hashHead->el;
        }
        else{
            hashHead = hashHead->next;
        }
    }

    while(ingredientHead->time != time){
        prevIngredientHead = ingredientHead;
        ingredientHead = ingredientHead->next;
    }
    removeNodeFromIngredientMap(map, hashHead, ingredientHead, prevIngredientHead);
}

void removeNodeFromIngredientMap(warehouseMap *map, ingredientLotListList *hashHead, ingredientLot *ingredientHead, ingredientLot *prevIngredientHead){
    hashHead->totalAmount -= ingredientHead->amount;
    if(prevIngredientHead != NULL){
        prevIngredientHead->next = ingredientHead->next;
    }
    else{
        hashHead->el = ingredientHead->next;
    }

    free(ingredientHead);
}

void addIngredientToMap(warehouseMap *map, ingredientLot*s, char *ingredientName){
    int hash = sdbm_hash(ingredientName);

    if(map->hashArray[hash] == NULL){
        //If spot is null then create new hashHead and new ingredientHead
        ingredientLotListList *hashHead = malloc(sizeof(*hashHead));
        ingredientLot *ingredientHead = malloc(sizeof(*ingredientHead));

        hashHead->el = ingredientHead;
        hashHead->totalAmount = s->amount;
        hashHead->next = NULL;
        hashHead->ingredientName = malloc(strlen(ingredientName)+1);
        strcpy(hashHead->ingredientName, ingredientName);

        ingredientHead = s;
        ingredientHead->next = NULL;

        map->hashArray[hash] = hashHead;
    }
    else{
        //The hashHead already existed, locate ingredientHead
        ingredientLotListList *hashHead = map->hashArray[hash];


        while(hashHead != NULL){
            if(strcmp(hashHead->ingredientName, ingredientName) == 0){
                //current hash head is the correct one
                break;
            }
            else if(hashHead->next == NULL){
                //No hashHead match was found, appending new hashead at the end of the bucket
                hashHead->next = malloc(sizeof(*hashHead));
                hashHead = hashHead->next;

                hashHead->totalAmount = 0;
                hashHead->next = NULL;
                hashHead->el = NULL;
                hashHead->ingredientName = malloc(strlen(ingredientName)+1);
                strcpy(hashHead->ingredientName, ingredientName);
                break;
            }
            hashHead = hashHead->next;
        }
        


        //We are now necessarily inside the correct hashHead, add ingredient to the list

        hashHead->totalAmount += s->amount;

        if(hashHead->el == NULL){
            //The ingredient lot we are inserting is the first one for this list
            hashHead->el = s;
            hashHead->el->next = NULL;
        }
        else{
            ingredientLot *ingredientNavigator = hashHead->el;
            //Need to locate correct position inside ingredientList
            if(ingredientNavigator->time > s->time){
                //earliest ingredient in list is still later than s, replace list head
                hashHead->el = s;
                hashHead->el->next = ingredientNavigator;
            }
            else{
                ingredientLot *prevLot = NULL;
                while(ingredientNavigator != NULL){
                    if(ingredientNavigator->time == s->time){
                        s->amount += ingredientNavigator->amount;
                        
                        s->next = ingredientNavigator->next;

                        if(prevLot != NULL){
                            prevLot->next = s;
                        }
                        else{
                            hashHead->el = s;
                        }

                        free(ingredientNavigator);

                        break;
                    }
                    else if(ingredientNavigator->time < s->time && (ingredientNavigator->next == NULL || ingredientNavigator->next->time > s->time)){
                        s->next = ingredientNavigator->next;
                        ingredientNavigator->next = s;

                        break;
                    }
                    prevLot = ingredientNavigator;
                    ingredientNavigator = ingredientNavigator->next;
                }
            }
        }
    }
}


//WareHouseTree
void addIngredientToTree(warehouseTreeNode **d_root, char *ingredient, int expiration){
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
        root->expiration = expiration;
        root->isBlack = 1;
        
        StringList *ingredients = malloc(sizeof(*ingredients));
        strcpy(ingredients->el, ingredient);
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
            if(expiration < x->expiration){
                x = x->left;
            }
            else if(expiration > x->expiration){
                x = x->right;
            }
            else if(expiration == x->expiration){
                //Node already exists
                duplicate = 1;
                StringList *ingredients = x->ingredients;

                int breaker = 0;
                while(breaker == 0){
                    if(strcmp(ingredients->el, ingredient) == 0){
                        breaker = 1;
                    }
                    else if(ingredients->next == NULL){
                        StringList *node = malloc(sizeof(*node));
                        strcpy(node->el, ingredient);
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
            z->expiration = expiration;
            z->ingredients = malloc(sizeof(*(z->ingredients)));
            z->ingredients->next = NULL;
            strcpy(z->ingredients->el, ingredient);

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

void removeIngredientFromTreeByTime(warehouseTreeNode **d_root, int time, char *ingredient){
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

    free(z);
}

void rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x){
    while (x != *root && x->isBlack == 1) {
        if (x == x->parent->left) {
            warehouseTreeNode *w = x->parent->right;
            if (w->isBlack == 0) {
                w->isBlack = 1;
                x->parent->isBlack = 0;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->isBlack == 1 && w->right->isBlack == 1) {
                w->isBlack = 0;
                x = x->parent;
            }
            else {
                if (w->right->isBlack == 1) {
                    w->left->isBlack = 1;
                    w->isBlack = 0;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->isBlack = x->parent->isBlack;
                x->parent->isBlack = 1;
                w->right->isBlack = 1;
                leftRotate(root, x->parent);
                x = *root;
            }
        } else {
            warehouseTreeNode *w = x->parent->left;
            if (w->isBlack == 0) {
                w->isBlack = 1;
                x->parent->isBlack = 0;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->left->isBlack == 1 && w->right->isBlack == 1) {
                w->isBlack = 0;
                x = x->parent;
            }
            else {
                if (w->left->isBlack == 1) {
                    w->right->isBlack = 1;
                    w->isBlack = 0;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
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


//WareHouse
int resupplyWarehouse(int time, warehouseMap *map, warehouseTreeNode **root){
    int ch = 0;
    int i;
    String nameBuffer;
    String buffer;
    int amount;
    int expiration;
    ingredientLot *s;

    while(ch != '\n' && ch != EOF){


        //ALLOCATE ELEMENT
        s = malloc(sizeof(*s));


        //READ INGREDIENT NAME
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            nameBuffer[i] = ch;
            i++;
        }
        nameBuffer[i] = '\0';


        //READ INGREDIENT AMOUNT
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            buffer[i] = ch;
            i++;
        }
        buffer[i] = '\0';
        amount = atoi(buffer);


        //READ INGREDIENT TIME
        i = 0;
        while((ch = fgetc(stdin)) != ' ' && ch != '\n' && ch != EOF){
            buffer[i] = ch;
            i++;
        }
        buffer[i] = '\0';
        expiration = atoi(buffer);


        if(expiration > time){
            s = malloc(sizeof(*s));
            s->time = expiration;
            s->amount = amount;


            addIngredientToTree(root, nameBuffer, s->time);
            addIngredientToMap(map, s, nameBuffer);
        }
    }
    printf("rifornito\n");

    return ch;
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

void removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity){
    ingredient *firstIngredientNode = recipie->head;
    ingredient *currentIngredientNode = firstIngredientNode;

    ingredientLotListList *hashHead;

    currentIngredientNode = firstIngredientNode;
    int totalAmount;

    ingredientLot *ingredientHead;
    ingredientLot *prevIngredientHead = NULL;

    while(currentIngredientNode != NULL){
        totalAmount = currentIngredientNode->amount * quantity;

        hashHead = currentIngredientNode->ingredientHead;
        //HashHead located
        ingredientHead = hashHead->el;
        prevIngredientHead = NULL;

        //Trim the list of Lots
        while(totalAmount > 0){
            if(totalAmount >= ingredientHead->amount){
                //Need to delete the whole node as all ingredients are used

                //Decrease remaining amount
                totalAmount -= ingredientHead->amount;

                //Go to next node
                prevIngredientHead = ingredientHead;
                ingredientHead = ingredientHead->next;

                //Delete prev node from tree and Map
                removeIngredientFromTreeByTime(root, prevIngredientHead->time, hashHead->ingredientName);
                removeNodeFromIngredientMap(map, hashHead, prevIngredientHead, NULL);
            }
            else{
                //Ingredients will be left over so no need to delete the node, only decrease the amount in hashHead
                hashHead->totalAmount -= totalAmount;

                ingredientHead->amount -= totalAmount;


                totalAmount = 0;
            }
        }

        currentIngredientNode = currentIngredientNode->next;
    }
}

int isOrderFulfillable(recipie *recipie, int quantity){
    //Returns 0 if there were less ingredients available than specified amount and nothing is deleted
    //Returns 1 if the ingredients were successfully removed

    //Check if there are enough ingredients for recipie
    ingredient *firstIngredientNode = recipie->head;
    ingredient *currentIngredientNode = firstIngredientNode;

    //check for each ingredient if there are enough in storage
    while(currentIngredientNode != NULL){

        if(currentIngredientNode->amount * quantity > currentIngredientNode->ingredientHead->totalAmount){
            return 0;
        }

        currentIngredientNode = currentIngredientNode->next;
    }

    return 1;
}


//Recipies
int readRecipie(recipiesMap *book, struct numberedTimedLotListMap *whMap){
    recipie *r = malloc(sizeof(*r));
    int ch;
    int i = 0;
    char buffer[256];

    //READ NAME
    while((ch = fgetc(stdin)) != '\n' && ch != ' ' && ch != EOF){
        buffer[i] = ch;
        i++;
    }
    buffer[i] = '\0';

    r->name = malloc(strlen(buffer)+1);
    strcpy(r->name, buffer);


    //Check if recipie is already in the map before reading the rest of the ingredients
    int duplicate = 0;
    int hash = sdbm_hash(r->name);
    int ingrHash;

    recipie *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->name, r->name) == 0){
            duplicate = 1;
            break;
        }

        hashHead = hashHead->next;
    }

    if(duplicate == 1){
        //recipie with same name was already stored in the map

        //free the room already allocated to the recipie
        free(r);

        printf("ignorato\n");

        //skip stdin to next line, ignore the rest of the recipie
        while(ch != '\n' && ch != EOF){
            ch = getc(stdin);
        }

        return ch;
    }

    //If function did not return then duplicate was 0, keep reading the recipie as normal



    int totalWeigth = 0;
    r->head = NULL;
    ingredient *prev = NULL;
    
    //READ INGREDIENTS
    while(ch != '\n' && ch != EOF){
        ingredient *ingr = malloc(sizeof(*ingr));

        //READ INGREDIENT
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            buffer[i] = ch;
            i++;
        }
        buffer[i] = '\0';


        //LINK INGREDIENT HEAD FROM MAP TO RECIPIE
        ingr->ingredientHead = NULL;
        ingrHash = sdbm_hash(buffer);
        ingredientLotListList *hashHead = NULL;

        if(whMap->hashArray[ingrHash] == NULL){
            hashHead = malloc(sizeof(*hashHead));
            hashHead->totalAmount = 0;
            hashHead->next = NULL;
            hashHead->el = NULL;
            hashHead->ingredientName = malloc(strlen(buffer)+1);
            strcpy(hashHead->ingredientName, buffer);

            ingr->ingredientHead = hashHead;
            whMap->hashArray[ingrHash] = hashHead;
        }
        else{
            hashHead = whMap->hashArray[ingrHash];

            while(ingr->ingredientHead == NULL){
                if(strcmp(hashHead->ingredientName, buffer) == 0){
                    ingr->ingredientHead = hashHead;
                }
                else if(hashHead->next == NULL){
                    hashHead->next = malloc(sizeof(*hashHead));
                    hashHead = hashHead->next;

                    hashHead->totalAmount = 0;
                    hashHead->next = NULL;
                    hashHead->el = NULL;
                    hashHead->ingredientName = malloc(strlen(buffer)+1);
                    strcpy(hashHead->ingredientName, buffer);

                    ingr->ingredientHead = hashHead;
                }

                hashHead = hashHead->next;
            }
        }





        //READ AMOUNT
        i = 0;
        String amount;
        while((ch = fgetc(stdin)) != ' ' && ch != '\n' && ch != EOF){
            amount[i] = ch;
            i++;
        }
        amount[i] = '\0';
        ingr->amount = atoi(amount);
        totalWeigth += ingr->amount;

        ingr->next = NULL;


        //STORE INGREDIENT IN NODE
        if(r->head == NULL){
            r->head = ingr;
        }
        else if(r->head->amount <= ingr->amount){
            ingr->next = r->head;
            r->head = ingr;
        }
        else{
            prev = r->head;

            while(prev != NULL){
                if(prev->amount >= ingr->amount && (prev->next == NULL || prev->next->amount < ingr->amount)){
                    ingr->next = prev->next;
                    prev->next = ingr;

                    break;
                }

                prev = prev->next;
            }
        }
    }

    //ADD LIST OF INGREDIENTS TO RECIPIE
    r->weight = totalWeigth;

    //Insert recipie into map
    r->ordersPending = 0;

    r->next = book->hashArray[hash];
    book->hashArray[hash] = r;

    printf("aggiunta\n");


    return ch;
}

int removeRecipie(recipiesMap *book){
    int ch;
    int i = 0;
    String name;
    int hash;

    //READ RECIPIE NAME
    while((ch = fgetc(stdin)) != '\n' && ch != EOF){
        name[i] = ch;
        i++;
    }
    name[i] = '\0';

    hash = sdbm_hash(name);



    //CHECK IF MAP CONTAINS HASH
    if(book->hashArray[hash] == NULL){
        printf("non presente\n");
    }
    else{
        //IF MAP CONTAINS HASH CHECK THE LIST FOR MATCHES
        int found = 0;
        int allowed = 0;
        recipie *prevHashHead = NULL;
        recipie *currHashHead = book->hashArray[hash];

        //CHECK IF THE HEAD OF THE LIST IS THE MATCH
        if(strcmp(book->hashArray[hash]->name, name) == 0){
            found = 1;
            if(book->hashArray[hash]->ordersPending == 0){
                allowed = 1;

                //If next was null then array now has position[hash] = null
                //if next was another node then old_next is the head at array[hash]
                book->hashArray[hash] = currHashHead->next;
            }
        }
        else{
            //Recipie to remove is not the head, checking other nodes
            
            //To keep the list linked, we iterate through the list using prev and see if prev->next is the match
            //this way we can link together the remainder of the list after we cut out prev->next
            while(currHashHead != NULL){
                if(strcmp(currHashHead->name, name) == 0){
                    found = 1;
                    if(currHashHead->ordersPending == 0){
                        allowed = 1;

                        //Remove the target from list
                        prevHashHead->next = currHashHead->next;
                    }
                    break;
                }
                else{
                    prevHashHead = currHashHead;
                    currHashHead = currHashHead->next;
                }
            }
        }

        if(found == 0){
            printf("non presente\n");
        }
        else if(allowed == 1){
            //CLEARING MEMORY
            ingredient *ingredientCurr = currHashHead->head;
            ingredient *ingredientPrev = NULL;

            while(ingredientCurr != NULL){
                //Clear all ingredients
                ingredientPrev = ingredientCurr;
                ingredientCurr = ingredientCurr->next;
                free(ingredientPrev);
            }

            //Clear recipie and hashHead from memory
            free(currHashHead);


            printf("rimossa\n");
        }
        else if(found == 1 && allowed == 0){
            //the recipie was found but is in use by pending orders
            printf("ordini in sospeso\n");
        }
    }

    return ch;
}

recipie *retrieveRecipie(recipiesMap *book, String name){
    //Given the provided book and recipie name, the function returns a pointer to the recipie if it is found in the book.
    //Returns null otherwise

    int hash = sdbm_hash(name);
    recipie *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->name, name) == 0){
            return hashHead;
        }
        else{
            hashHead = hashHead->next;
        }
    }

    return NULL;
}


//Orders
void removeOrderFromPending(orderedItem *current, orderedItem *prev, orderedItemQueue *ordersWaiting){
    if(prev == NULL){
        //item to remove is the head of the queue
        if(current->next == NULL){
            //item is the only one in the queue
            ordersWaiting->head = NULL;
            ordersWaiting->tail = NULL;
        }
        else{
            //replace the head of the queue
            ordersWaiting->head = ordersWaiting->head->next;
        }
    }
    else if(current->next == NULL){
        //item to remove is tail
        ordersWaiting->tail = prev;
        prev->next = NULL;
    }
    else{
        //item is in the middle of the queue
        prev->next = current->next;
    }
}

void addOrderToReady(orderedItem *item, orderedItemQueue *ordersReady){
    orderedItem *current = ordersReady->head;
    orderedItem *prev = NULL;

    orderedItem *newNode = item;

    int breaker = 0;
    while(breaker == 0){
        if(ordersReady->head == NULL){
            ordersReady->head = newNode;
            ordersReady->tail = newNode;
            newNode->next = NULL;

            breaker = 1;
        }
        else if(current->time <= item->time){
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

int readOrder(orderedItem *item, int time, recipiesMap *book){
    int ch = 0;
    int i;
    String buffer;

    while(ch != '\n' && ch != EOF){

        //READ RECIPIE NAME
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            buffer[i] = ch;
            i++;
        }
        buffer[i] = '\0';

        //RETRIEVE RECIPIE
        item->recipie = retrieveRecipie(book, buffer);


        //READ AMOUNT ORDERED
        i = 0;
        while((ch = fgetc(stdin)) != '\n'){
            buffer[i] = ch;
            i++;
        }
        buffer[i] = '\0';
        item->amount = atoi(buffer);


        //ADD INGREDIENT TIME
        item->time = time;

        item->next = NULL;
    }
    return ch;
}

void fulfillOrdersPending(warehouseMap *map, warehouseTreeNode **root, orderedItemQueue *ordersPending, orderedItemQueue *ordersReady){
    orderedItem *currentOrder = ordersPending->head;
    orderedItem *prevOrder = NULL;
    recipie *recipie = NULL;

    while(currentOrder != NULL){
        recipie = currentOrder->recipie;

        if(isOrderFulfillable(recipie, currentOrder->amount) == 1){

            removeIngredientsFromWarehouseByOrder(root, map, recipie, currentOrder->amount);

            removeOrderFromPending(currentOrder, prevOrder, ordersPending);
            addOrderToReady(currentOrder, ordersReady);
            


            if(prevOrder == NULL){
                currentOrder = ordersPending->head;
            }
            else{
                currentOrder = prevOrder->next;
            }
        }
        else{
            prevOrder = currentOrder;
            currentOrder = currentOrder->next;
        }
    }
}

int order(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueue *ordersReady, orderedItemQueue *ordersWaiting, orderedItemQueueMap *ordersByIngredient, int time){
    int ch;

    orderedItem *item = malloc(sizeof(*item));
    ch = readOrder(item, time, book);

    recipie *recipie = item->recipie;

    if(recipie == NULL){
        //No matching recipie was found, order refused and cleared from memory
        printf("rifiutato\n");
        free(item);
    }
    else{
        //mark recipie as "orders relying on recipie"
        recipie->ordersPending++;

        //Calcuate order weigth
        item->totalWeigth = recipie->weight * item->amount;
        item->next = NULL;

        //Handle the rest of the order procedures

        if(isOrderFulfillable(recipie, item->amount) == 1){
            //The order was processed immediately, adding to orders ready

            removeIngredientsFromWarehouseByOrder(root, map, recipie, item->amount);

            if(ordersReady->head == NULL){
                ordersReady->head = item;
                ordersReady->tail = item;
            }
            else{
                ordersReady->tail->next = item;
                ordersReady->tail = item;
            }
        }
        else{
            //The order could not be processed due to lacking ingredients, adding order to waiting queue and to ingredientMap
            if(ordersWaiting->head == NULL){
                ordersWaiting->head = item;
            }
            if(ordersWaiting->tail != NULL){
                ordersWaiting->tail->next = item;
            }
            ordersWaiting->tail = item;


            //addOrderToIngredientMap(item, ordersByIngredient, recipie->head);
        }

        printf("accettato\n");
    }

    return ch;
}


//Courier
int setupCourier(Courier *c){
    String input;
    int ch;
    int i = 0;

    while((ch = fgetc(stdin)) != '\n' && ch != EOF){
        input[i] = ch;
        i++;
    }
    input[i] = '\0';

    sscanf(input, "%d %d", &c->frequency, &c->capacity);

    return ch;
}

void loadCourier(Courier *courier, orderedItemQueue *ordersReady){
    if(ordersReady->head != NULL){
        int currentCourierLoad = 0;

        orderedItem *orderNode;

        int breaker = 0;
        while(breaker == 0){
            orderNode = ordersReady->head;
            //Check if enough space in Courier
            if(currentCourierLoad + orderNode->totalWeigth <= courier->capacity){
                //order fits in the courier
                //Load orders sorted by weight descending then by time ascending

                //Remove order from queue
                ordersReady->head = ordersReady->head->next;

                //Increase courier load
                currentCourierLoad += orderNode->totalWeigth;

                //removing utilization from recipie book
                orderNode->recipie->ordersPending--;

                //Load order inside courier
                if(courier->ordersHead == NULL || courier->ordersHead->totalWeigth < orderNode->totalWeigth || (courier->ordersHead->totalWeigth == orderNode->totalWeigth && courier->ordersHead->time > orderNode->time)){
                    //if no orders are in courier OR first order weighs less than the current order OR they weigh the same but
                    //the current order is fresher then replace the head with the current order
                    orderNode->next = courier->ordersHead;
                    courier->ordersHead = orderNode;
                }
                else{
                    //Navigate through the list of orders in the courier to find correct placement
                    orderedItem *prev = NULL;
                    orderedItem *current = courier->ordersHead;

                    int breaker = 0;
                    while(breaker == 0){
                        if(current->totalWeigth < orderNode->totalWeigth || (current->totalWeigth == orderNode->totalWeigth && current->time > orderNode->time)){
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
                    //No more orders left in the ready queue, interrupt the loading while
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
    orderedItem *currentOrder = courier->ordersHead;

    if(courier->ordersHead == NULL){
        printf("camioncino vuoto\n");
    }
    else{
        while (currentOrder != NULL) {
            printf("%d %s %d\n", currentOrder->time, currentOrder->recipie->name, currentOrder->amount);
            currentOrder = currentOrder->next;
        }
    }
}

void clearCourierOrdersMemory(Courier *courier){
    if(courier->ordersHead == NULL){
        return;
    }

    orderedItem *current = courier->ordersHead;
    orderedItem *prev = NULL;

    while(current != NULL){
        prev = current;
        current = current->next;
        
        free(prev);
    }

    courier->ordersHead = NULL;
}