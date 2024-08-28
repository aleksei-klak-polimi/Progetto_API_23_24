#include <stdio.h>
#include <stdlib.h>

#include "Courier.h"


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

void loadCourier(Courier *courier, recipiesMap *book, orderedItemQueue *ordersReady){
    if(ordersReady->head != NULL){
        int currentCourierLoad = 0;

        orderedItemList *orderNode;

        int breaker = 0;
        while(breaker == 0){
            orderNode = ordersReady->head;
            //Check if enough space in Courier
            if(currentCourierLoad + orderNode->el->totalWeigth <= courier->capacity){
                //order fits in the courier
                //Load orders sorted by weight descending then by time ascending

                //Remove order from queue
                ordersReady->head = ordersReady->head->next;

                //Increase courier load
                currentCourierLoad += orderNode->el->totalWeigth;

                //removing utilization from recipie book
                decrementRecipieUtilization(book, orderNode->el->name);

                //Load order inside courier
                if(courier->ordersHead == NULL || courier->ordersHead->el->totalWeigth < orderNode->el->totalWeigth || (courier->ordersHead->el->totalWeigth == orderNode->el->totalWeigth && courier->ordersHead->el->time > orderNode->el->time)){
                    //if no orders are in courier OR first order weighs less than the current order OR they weigh the same but
                    //the current order is fresher then replace the head with the current order
                    orderNode->next = courier->ordersHead;
                    courier->ordersHead = orderNode;
                }
                else{
                    //Navigate through the list of orders in the courier to find correct placement
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

    orderedItemList *current = courier->ordersHead;
    orderedItemList *prev = NULL;

    while(current != NULL){
        prev = current;
        current = current->next;

        free(prev->el);
        free(prev);
    }

    courier->ordersHead = NULL;
}