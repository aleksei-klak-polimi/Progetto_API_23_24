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