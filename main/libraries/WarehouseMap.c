#include "WarehouseMap.h"

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

                //Replace duplicate ingredient lot with the one passed as parameter
                s->amount += currentIngredientLot->el->amount;
                free(currentIngredientLot->el);
                currentIngredientLot->el = s;
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
                while(nodeFound == 0){
                    if(currentIngredientLot->el->time == s->time){
                        nodeFound = 1;

                        //Replace duplicate ingredient lot with the one passed as parameter
                        s->amount += currentIngredientLot->el->amount;
                        free(currentIngredientLot->el);
                        currentIngredientLot->el = s;
                    }
                    else if(currentIngredientLot->el->time < s->time && (currentIngredientLot->next == NULL || currentIngredientLot->next->el->time > s->time)){
                        nodeFound = 1;
                        ingredientLotList *newNode = malloc(sizeof(*newNode));
                        newNode->el = s;
                        newNode->next = currentIngredientLot->next;
                        currentIngredientLot->next = newNode;
                    }
                    else{
                        currentIngredientLot = currentIngredientLot->next;
                    }
                }
            }
        }
    }
}