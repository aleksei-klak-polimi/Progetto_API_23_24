#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAPSIZE 1024
#define STRINGSIZE 256





//OPERATIONAL STRUCTURES
typedef char String[STRINGSIZE];

typedef struct StringList{
    String              el;
    struct StringList   *next;
}StringList;





//STRUCTURES
struct numberedItem{
    String  name;
    int     amount;
};


struct numberedItemList{
    struct numberedItem     *el;
    struct numberedItemList *next;
};


struct namedNumberedItemList{
    String                      name;
    struct numberedItemList     *head;
};


struct namedNumberedItemListList{
    int                                 ordersPending;
    struct namedNumberedItemList        *el;
    struct namedNumberedItemListList    *next;
};


struct namedNumberedItemListListMap{
    struct namedNumberedItemListList   *hashArray[HASHMAPSIZE];
};


struct numberedTimedItem{
    String  name;
    int     amount;
    int     time;
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
    struct numberedTimedItemList    ordersHead;
};





//TYPEDEFS
typedef struct numberedItem                     ingredient;
typedef struct numberedItemList                 ingredientList;
typedef struct namedNumberedItemList            recipie;
typedef struct namedNumberedItemListList        recipiesList;
typedef struct namedNumberedItemListListMap     recipiesMap;

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

typedef struct Courier                          Courier;





//METHOD DECLARATION
//INSTRUCTIONS
int             addRecipie(recipiesMap *book);
int             removeRecipie(recipiesMap *book);
int             resupply(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueueMap *ordersByIngredient, orderedItemQueue *ordersPending, orderedItemQueue *ordersReady);
int             order(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueue *ordersReady, orderedItemQueue *ordersWaiting, orderedItemQueueMap *ordersByIngredient, int time);
int             loadCurrier();

//RECIPIES
unsigned int    sdbm_hash(String string);
void            insertRecipie(recipiesMap *book, recipie *recipie);
int             readRecipie(recipie *r);
void            printRecipie(recipie *r);
void            deleteRecipie(recipiesMap book, String name);
recipie         *retrieveRecipie(recipiesMap *book, String name);
void            printRecipieBook(recipiesMap *book);

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
void            printSupplies(ingredientLotList *s);
void            printRBTree(warehouseTreeNode *node, int level);
void            printSpaces(int count);
void            addIngredientToMap(warehouseMap *map, ingredientLot*s);
void            removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient);
void            removeNodeFromIngredientMap(warehouseMap *map, int hash, ingredientLotListList *hashHead, ingredientLotListList *prevHashHead, ingredientLotList *ingredientHead, ingredientLotList *prevIngredientHead);
void            removeIngredientFromTreeByTime(warehouseTreeNode **d_root, int time, String ingredient);
int             removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity);

//ORDERS
int             readOrder(orderedItem *item, int time);
void            printOrder(orderedItem *item);
void            addOrderToIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead);
void            removeOrderFromIngredientMap(orderedItem *item, orderedItemQueueMap *ordersByIngredient, ingredientList *ingredientsHead);
void            removeOrderFromPending(orderedItem *item, orderedItemQueue *ordersWaiting);
void            addOrderToReady(orderedItem *item, orderedItemQueue *ordersReady);

//COURIER
int             setupCourier(Courier *c);





int main(){
    int ch = 0;
    int time = 0;
    int i = 0;
    String command;

    recipiesMap         cookBook;
    warehouseMap        whMap;

    orderedItemQueueMap *ordersByIngredientsMap = malloc(sizeof(*ordersByIngredientsMap));
    orderedItemQueue    *ordersReady = malloc(sizeof(*ordersReady));
    orderedItemQueue    *ordersPending = malloc(sizeof(*ordersPending));

    warehouseTreeNode   *whTree = NULL;
    Courier             courier;

    //initialize the cookBook to have all entries NULL.
    for(i = 0; i < HASHMAPSIZE; i++){
        cookBook.hashArray[i] = NULL;
        whMap.hashArray[i] = NULL;
        ordersByIngredientsMap->hashArray[i] = NULL;
    }
    ordersReady->head = NULL;
    ordersReady->tail = NULL;
    
    ordersPending->head = NULL;
    ordersPending->tail = NULL;
    //todo finish initializing other structures


    //main while loop
    while(ch != EOF){

        //SETUP COURIER
        if(time == 0){
            Courier *courierPointer = &courier;
    
            ch = setupCourier(courierPointer);
            printf("Frequency: %d\nCapacity: %d\n", courier.frequency, courier.capacity);
        }
        else{
            warehouseTreeNode **root =   &whTree;
            //Remove expired ingredients
            removeNodeFromTreeByTime(root, time);
            printf("\nRemoved expired:\n");
            printRBTree(*root, 0);


            //CHECK IF COURIER TIME
            if(time % courier.frequency == 0){
                //todo
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
                recipiesMap *book = &cookBook;
                addRecipie(book);

                //debug
                printRecipieBook(book);
            }
            else if(strcmp("rimuovi_ricetta", command) == 0){
                recipiesMap *book = &cookBook;
                removeRecipie(book);

                //debug
                printRecipieBook(book);
            }
            else if(strcmp("rifornimento", command) == 0){
                warehouseMap *map =         &whMap;
                recipiesMap *book = &cookBook;

                resupply(map, root, book, ordersByIngredientsMap, ordersPending, ordersReady);

                //debug
                printRBTree(*root, 0);
            }
            else if(strcmp("ordine", command) == 0){
                warehouseMap *map = &whMap;
                recipiesMap *book = &cookBook;

                order(map, root, book, ordersReady, ordersPending, ordersByIngredientsMap, time);
            }
        }


        time++;
    }

    return 0;
}





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

int addRecipie(recipiesMap *book){
    int ch;
    recipie *r = malloc(sizeof(*r));
    ch = readRecipie(r);
    insertRecipie(book, r);

    return ch;
}

void insertRecipie(recipiesMap *book, recipie *recipie){
    int hash = sdbm_hash(recipie->name);
    int duplicate = 0;

    if(book->hashArray[hash] == NULL){
        recipiesList *head;
        head = malloc(sizeof(*head));
        head->el = recipie;
        head->next = NULL;
        head->ordersPending = 0;

        book->hashArray[hash] = head;
    }
    else{
        recipiesList *node = book->hashArray[hash];

        /*
        While(node != NULL) causes segmentation fault at line node->next = newNode
        While(node->next != NULL) skips the check on if the names of the recipies are the same when the list is size = 1
        do while allows to check the name on size 1 and prevents segmentation fault.
        */
        do {   
            if(strcmp(node->el->name, recipie->name) == 0){
                duplicate = 1;
                break;
            }
            else{
                node = node->next;
            }
        }
        while(node->next != NULL);

        if(duplicate == 0){
            recipiesList *newNode;
            newNode = malloc(sizeof(*newNode));
            newNode->el = recipie;
            newNode->next = NULL;
            newNode->ordersPending = 0;

            node->next = newNode;
        }
    }
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
        //todo check if printf is good for stdout
        printf("non presente\n");
    }
    else{
        //IF MAP CONTAINS HASH CHECK THE LIST FOR MATCHES
        int found = 0;
        int allowed = 0;
        recipiesList *prev = book->hashArray[hash];
        recipiesList *target;

        //CHECK IF THE HEAD OF THE LIST IS THE MATCH
        if(strcmp(book->hashArray[hash]->el->name, name) == 0){
            found = 1;
            if(book->hashArray[hash]->ordersPending != 0){
                allowed = 1;
                target = book->hashArray[hash];

                //If next was null then array now has position[hash] = null
                //if next was another node then old_next is the head at array[hash]
                book->hashArray[hash] = prev->next;
            }
        }
        else{
            
            //To keep the list linked, we iterate through the list using prev and see if prev->next is the match
            //this way we can link together the remainder of the list after we cut out prev->next

            while(prev->next != NULL){
                if(strcmp(prev->next->el->name, name) == 0){
                    found = 1;
                    if(book->hashArray[hash]->ordersPending != 0){
                        allowed = 1;
                        target = prev->next;

                        //Remove the target from list
                        prev->next = target->next;

                        break;
                    }
                }
                else{
                    prev = prev->next;
                }
            }
        }

        if(found == 0){
            //todo check if printf is good for stdout
            printf("non presente\n");
        }
        else if(allowed == 1){
            //CLEARING MEMORY
            free(target);
            printf("rimossa\n");
        }
        else if(found == 1 && allowed == 0){
            //the recipie was found but is in use by pending orders
            printf("non presente\n");
        }
    }

    return ch;
}

recipie *retrieveRecipie(recipiesMap *book, String name){
    //Given the provided book and recipie name, the function returns a pointer to the recipie if it is found in the book.
    //Returns null otherwise

    int hash = sdbm_hash(name);
    recipiesList *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->el->name, name) == 0){
            return hashHead->el;
        }
        else{
            hashHead = hashHead->next;
        }
    }

    return NULL;
}

int resupply(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueueMap *ordersByIngredient, orderedItemQueue *ordersPending, orderedItemQueue *ordersReady){
    int ch;

    ingredientLotList *lot;
    lot = malloc(sizeof(*lot));

    ch = readSupplies(lot);

    ingredientLotList *navigator = lot;
    while(navigator != NULL){
        addIngredientToTree(root, navigator->el);
        addIngredientToMap(map, navigator->el);
        navigator = navigator->next;
    }

    //For each ingredient added with resupply try to fulfill pending orders from Ingredient map
    navigator = lot;
    int hash;
    orderedItemQueueList *hashHead;
    orderedItemList *orderList = NULL;
    orderedItemList *nextOrderNode = NULL;
    recipie *recipie;

    while(navigator != NULL){
        hash = sdbm_hash(navigator->el->name);

        hashHead = ordersByIngredient->hashArray[hash];

        int breaker = 0;
        while(breaker == 0){
            if(strcmp(hashHead->el->ingredient, navigator->el->name) == 0){
                breaker = 1;
                orderList = hashHead->el->head;
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

        while(orderList != NULL){
            //Try to fulfill each order
            recipie = retrieveRecipie(book, orderList->el->name);

            if(removeIngredientsFromWarehouseByOrder(root, map, recipie, orderList->el->amount) == 1){
                //Order was fulfilled, remove from pending and add to ready
                removeOrderFromPending(orderList->el, ordersPending);
                addOrderToReady(orderList->el, ordersReady);
            }


            orderList = nextOrderNode;
            nextOrderNode = orderList->next;
        }
        orderList = NULL;
        nextOrderNode = NULL;


        navigator = navigator->next;
    }




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

        root->right = NULL;
        root->left = NULL;
        root->parent = NULL;
        root->expiration = s->time;
        root->isBlack = 1;
        
        StringList *ingredients = malloc(sizeof(*ingredients));
        strcpy(ingredients->el, s->name);
        ingredients->next = NULL;
        root->ingredients = ingredients;
    }
    else{
        //The double pointer points to a valid root therefore we need to insert into the tree

        int lotExpiration = s->time;
        int parentFound = 0;
        warehouseTreeNode *parentLeaf = *d_root;


        //Navigating tree to find closest leaf to lot expiration
        while(parentFound == 0){
            if(parentLeaf->expiration > lotExpiration){
                if(parentLeaf->left == NULL){
                    parentFound = 1;
                }
                else{
                    parentLeaf = parentLeaf->left;
                }
            }
            else if(parentLeaf->expiration < lotExpiration){
                if(parentLeaf->right == NULL){
                    parentFound = 1;
                }
                else{
                    parentLeaf = parentLeaf->right;
                }
            }
            else if(parentLeaf->expiration == lotExpiration){
                parentFound = 1;
            }
        }

        if(parentLeaf->expiration == lotExpiration){
            //Leaf with required expiration already exists, no new leafs created, only adding ingredient to list.
            StringList *ingredients = parentLeaf->ingredients;

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
        }
        else{
            //No existing leaf has required expiration, creating new leaf
            warehouseTreeNode *leaf = malloc(sizeof(*leaf));

            leaf->right = NULL;
            leaf->left = NULL;
            leaf->parent = parentLeaf;
            leaf->expiration = lotExpiration;
            leaf->isBlack = 0;
        
            StringList *ingredients = malloc(sizeof(*ingredients));
            strcpy(ingredients->el, s->name);
            ingredients->next = NULL;
            leaf->ingredients = ingredients;

            if(parentLeaf->expiration > lotExpiration){
                parentLeaf->left = leaf;
            }
            else if(parentLeaf->expiration < lotExpiration){
                parentLeaf->right = leaf;
            }


            //Red-Black tree may not balanced, may need to perform a rotation
            rebalanceTreeAfterInsertion(d_root, leaf);
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
            if(strcmp(hashHead->el->el->name, ingredient->name)){
                if(hashHead->totalAmount < (ingredient->amount *quantity)){
                    return 0;
                }
                else{
                    breaker = 1;
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

        int breaker = 0;
        while(breaker == 0){
            if(strcmp(hashHead->el->el->name, ingredient->name)){
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


    //If node has no children simply delete node and pass to the rebalance function a NIL node representing the deleted node
    if(node->right == NULL && node->left == NULL){
        warehouseTreeNode *parent = node->parent;

        if(parent != NULL){
            //If the node to delete is not the root then sever the parent-child connection
            if(parent->right == node){
                parent->right = NULL;
            }
            else{
                parent->left = NULL;
            }
            //todo check if this free does not delete the list I am returning
            rebalanceTreeAfterDelete(root, node);
            free(node);
        }
        else{
            //The node to delete is the root
            *root = NULL;

            //todo check if this free does not delete the list I am returning
            free(node);
        }
    }


    //If node has one child copy the child in the parent and delete the child
    else if(node->right == NULL || node->left == NULL){

        warehouseTreeNode *child;
        warehouseTreeNode *parent = node;
        
        if(parent->right != NULL){
            child = parent->right;
        }
        else{
            child = parent->left;
        }

        parent->expiration = child->expiration;
        parent->ingredients = child->ingredients;
        parent->right = child->right;
        parent->left = child->left;

        if(child->right != NULL){
            child->right->parent = parent;
        }
        if(child->left != NULL){
            child->left->parent = parent;
        }

        //todo check if this free does not delete the list I am returning
        rebalanceTreeAfterDelete(root, child);
        free(child);
    }


    //If node has two children, find its successor, copy it into the node and perform deletion of the successor
    else if(node->right != NULL && node->left != NULL){
        //find smallest successor to our node
        warehouseTreeNode *successor = node->right;

        int breaker = 0;
        while(breaker == 0){
            if(successor->left == NULL){
                breaker = 1;
            }
            else{
                successor = successor->left;
            }
        }

        //copy contents of successor into node
        node->expiration = successor->expiration;
        node->ingredients = successor->ingredients;

        //remove old successor node from tree
        deleteNodeFromTree(root, successor);
    }
}

void rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x){
    // While x is not the root and x is black
    while (x != *root && x->isBlack) {
        if (x != x->parent->right && x->parent->right != NULL) {  // x is the left child
            warehouseTreeNode *w = x->parent->right; // w is x's sibling
            if (!w->isBlack) { // Case 1: x's sibling w is red
                w->isBlack = 1;               // Recolor w as black
                x->parent->isBlack = 0;        // Recolor x's parent as red
                leftRotate(root, x->parent);   // Left rotate on x's parent
                w = x->parent->right;          // Update w to be the new sibling of x
            }
            if ((w->left == NULL || w->left->isBlack == 1) && (w->right == NULL || w->right->isBlack == 1)) { // Case 2: Both of w's children are black
                w->isBlack = 0;               // Recolor w as red
                x = x->parent;                // Move x up the tree
            }
            else {
                if (w->right == NULL || w->right->isBlack == 1) { // Case 3: w's right child is black
                    if(w->left != NULL){
                        w->left->isBlack = 1;   // Recolor w's left child as black
                    }
                    w->isBlack = 0;          // Recolor w as red
                    rightRotate(root, w);    // Right rotate on w
                    w = x->parent->right;    // Update w to be the new sibling of x
                }
                // Case 4: w's right child is red
                w->isBlack = x->parent->isBlack; // Make w the same color as x's parent
                x->parent->isBlack = 1;          // Recolor x's parent as black
                if(w->right != NULL){
                    w->right->isBlack = 1;       // Recolor w's right child as black
                }   
                leftRotate(root, x->parent);     // Left rotate on x's parent
                x = *root;                       // Set x to root to end loop
            }
        } else {  // Symmetric case: x is the right child
            warehouseTreeNode *w = x->parent->left;
            if (!w->isBlack) { // Case 1: x's sibling w is red
                w->isBlack = 1;
                x->parent->isBlack = 0;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if ((w->left == NULL || w->left->isBlack == 1) && (w->right == NULL || w->right->isBlack == 1)) { // Case 2: Both of w's children are black
                w->isBlack = 0;
                x = x->parent;
            } else {
                if (w->left == NULL || w->left->isBlack) { // Case 3: w's left child is black
                    if(w->right != NULL){
                        w->right->isBlack = 1;
                    }
                    w->isBlack = 0;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                // Case 4: w's left child is red
                w->isBlack = x->parent->isBlack;
                x->parent->isBlack = 1;
                if(w->left != NULL){
                    w->left->isBlack = 1;
                }
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->isBlack = 1; // Ensure the root node is always black
}

void rebalanceTreeAfterInsertion(warehouseTreeNode **root, warehouseTreeNode *x){
    if(*root == x){
        x->isBlack = 1;
    }
    else{
        warehouseTreeNode *child = x;
        warehouseTreeNode *parent = child->parent;
        warehouseTreeNode *grandpa;
        warehouseTreeNode *uncle;

        while(child != *root && parent->isBlack == 0){
            parent = child->parent;
            grandpa = parent->parent;

            if(parent == grandpa->left){
                uncle = grandpa->right;

                if(uncle != NULL && uncle->isBlack == 0){   // Case 1: Uncle is red
                    parent->isBlack = 1;                    // Recolor parent black
                    uncle->isBlack = 1;                     // Recolor uncle black
                    grandpa->isBlack = 0;                   // Recolor grandparent red

                    child = grandpa;                        //Restart from grandparent
                }
                else{
                    if(child == parent->right){
                        child = parent;
                        parent = child->parent;
                        grandpa = parent->parent;
                        leftRotate(root, child);
                    }
                    parent->isBlack = 1;
                    grandpa->isBlack = 0;
                    rightRotate(root, grandpa);
                }
            }
            else{
                uncle = grandpa->left;
                if(uncle != NULL && uncle->isBlack == 0){
                    parent->isBlack = 1;
                    uncle->isBlack = 1;
                    grandpa->isBlack = 0;

                    child = grandpa;
                }
                else{
                    if(child == parent->left){
                        child = parent;
                        parent = child->parent;
                        grandpa = parent->parent;
                        rightRotate(root, child);
                    }
                    parent->isBlack = 1;
                    grandpa->isBlack = 0;
                    leftRotate(root, grandpa);
                }
            }
        }
        (*root)->isBlack = 1;
    }
}

void leftRotate(warehouseTreeNode **root, warehouseTreeNode *x) {
    warehouseTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
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
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
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

        orderedItemList *orderNode = malloc(sizeof(*orderNode));
        orderNode->el = item;
        orderNode->next = NULL;

        if(removeIngredientsFromWarehouseByOrder(root, map, recipie, item->amount) == 1){
            //The order was processed immediately, adding to orders ready
            if(ordersReady->head == NULL){
                ordersReady->head = orderNode;
            }
            if(ordersReady->tail != NULL){
                ordersReady->tail->next = orderNode;
            }
            ordersReady->tail = orderNode;
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

                    ordersQueue->head = orderNode;
                    ordersQueue->tail = orderNode;
                    strcpy(ordersQueue->ingredient, ingredientNode->el->name);
                }
            }
        }
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
                    if(orderNode->el->amount == item->amount && orderNode->el->time == item->time){
                        //todo check if possible to compare directly item and node, if they point to same memory address

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
        if(current->el->time <= item->time){
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
    if (node == NULL) {
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
}





//UTILITY FUNCTIONS
unsigned int sdbm_hash(String string){
    unsigned long hash = 0;
    int c;

    while((c = *string++)){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash % HASHMAPSIZE;
}