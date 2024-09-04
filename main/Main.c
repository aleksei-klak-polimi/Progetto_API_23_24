#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libraries/String.h"
#include "libraries/Map.h"
#include "libraries/Recipies.h"
#include "libraries/Warehouse.h"
#include "libraries/Orders.h"
#include "libraries/Courier.h"


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