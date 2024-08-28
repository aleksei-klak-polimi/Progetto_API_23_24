#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libraries/String.h"
#include "libraries/Map.h"
#include "libraries/Recipies.h"
#include "libraries/Warehouse.h"
#include "libraries/Orders.h"
#include "libraries/Courier.h"




//METHOD DECLARATION
//INSTRUCTIONS
int             addRecipie(recipiesMap *book);
int             removeRecipie(recipiesMap *book);
int             resupply(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueueMap *ordersByIngredient, orderedItemQueue *ordersPending, orderedItemQueue *ordersReady);
int             order(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueue *ordersReady, orderedItemQueue *ordersWaiting, orderedItemQueueMap *ordersByIngredient, int time);


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
        addIngredientToTree(root, IngredientLotNavigator->el->name, IngredientLotNavigator->el->time);
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