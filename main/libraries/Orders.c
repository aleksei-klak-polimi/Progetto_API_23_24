#include "Recipies.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "Orders.h"


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