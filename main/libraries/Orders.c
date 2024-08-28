#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "Orders.h"

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