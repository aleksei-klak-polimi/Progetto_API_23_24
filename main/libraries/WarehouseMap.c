#include "WarehouseMap.h"
#include <stdio.h>
#include <string.h>

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