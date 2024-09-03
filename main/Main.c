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
int             addRecipie(recipiesMap *book, warehouseMap *map);
int             removeRecipie(recipiesMap *book);
int             order(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueue *ordersReady, orderedItemQueue *ordersWaiting, orderedItemQueueMap *ordersByIngredient, int time);


//DEBUG PRINTS
void            printSupplies(ingredientLotList *s);
void            printRBTree(warehouseTreeNode *node, int level);
void            printSpaces(int count);
void            printRecipie(recipie *r);
void            printRecipieBook(recipiesMap *book);
void            printOrderQueue(orderedItemQueue *queue);
void            printOrdersByIngredientMap(orderedItemQueueMap *ordersByIngredient);
void            printIngredientMap(warehouseMap *map);




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
            printf("\n");

            printf("\nPending orders queue contents:\n");
            printOrderQueue(ordersPending);
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
            addRecipie(book, whMap);

            //debug
            //printRecipieBook(book);
        }
        else if(strcmp("rimuovi_ricetta", command) == 0){
            removeRecipie(book);

            //debug
            //printRecipieBook(book);
        }
        else if(strcmp("rifornimento", command) == 0){
            resupplyWarehouse(time, whMap, root);
            fulfillOrdersPending(whMap, root, ordersPending, ordersReady);

            //debug
            //printRBTree(*root, 0);
        }
        else if(strcmp("ordine", command) == 0){
            order(whMap, root, book, ordersReady, ordersPending, ordersByIngredientsMap, time);
        }
        
        time++;

        //printIngredientMap(whMap);

        /*printf("\nDebug tree print:\n");
        printRBTree(*root, 0);*/

        /*printf("\nDebug map by ingredients print:\n");
        printOrdersByIngredientMap(ordersByIngredientsMap);*/
    }

    return 0;
}






int addRecipie(recipiesMap *book, warehouseMap *map){
    //calls function to read recipie from input buffer
    //then calls function to add recipie to recipie book
    //malloc for recipie.
    int ch;
    recipie *r = malloc(sizeof(*r));
    ch = readRecipie(book, r, map);

    return ch;
}




//ORDER HANDLING
int order(warehouseMap *map, warehouseTreeNode **root, recipiesMap *book, orderedItemQueue *ordersReady, orderedItemQueue *ordersWaiting, orderedItemQueueMap *ordersByIngredient, int time){
    int ch;

    orderedItem *item = malloc(sizeof(*item));
    ch = readOrder(item, time);

    recipie *recipie = retrieveRecipie(book, item->name);
    item->recipie = recipie;

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
        ingredient *ingredientNode = recipie->head;
        int weigth = 0;
        while(ingredientNode != NULL){
            weigth += ingredientNode->amount;
            ingredientNode = ingredientNode->next;
        }
        item->totalWeigth = weigth * item->amount;

        //Handle the rest of the order procedures
        orderedItemList *orderNode = malloc(sizeof(*orderNode));
        orderNode->el = item;
        orderNode->next = NULL;

        if(isOrderFulfillable(map, recipie, item->amount) == 1){
            //The order was processed immediately, adding to orders ready

            removeIngredientsFromWarehouseByOrder(root, map, recipie, item->amount);

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


            //addOrderToIngredientMap(item, ordersByIngredient, recipie->head);
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
            recipie *node = book->hashArray[i];

            while(node != NULL){
                printRecipie(node);
                node = node->next;
            }
        }
    }
}

void printRecipie(recipie *r){
    printf("\nRecipie name: %s\n", r->name);

    ingredient *ingr = r->head;

    int i = 0;
    while(ingr != 0){
        printf("Ingredient %d: %s, %d\n", i, ingr->ingredientHead->ingredientName, ingr->amount);
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

    printf("\nQueue in full:\n");
    int i = 0;
    while(node != NULL){
        printf("Item number %d\n", i);
        printOrder(node->el);
        printf("\n");
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

void printIngredientMap(warehouseMap *map){
    int i;

    for (i = 0; i < HASHMAPSIZE; i++) {
        if (map->hashArray[i] != NULL) {
            printf("Pos: %d\n", i);

            ingredientLotListList *hashHead = map->hashArray[i];

            int j = 0;
            while (hashHead != NULL) {
                printf("  Head %d contains: ", j);

                if (hashHead->el == NULL) {
                    printf("NULL");
                } else {
                    printf("Ingredient: %s, amount: %d", hashHead->el->el->name, hashHead->totalAmount);
                }

                printf("\n");  // Add a newline for readability
                hashHead = hashHead->next;  // Move to the next hashHead in the list
                j++;
            }
        }
    }
}