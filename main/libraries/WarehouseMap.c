#include "WarehouseMap.h"
#include <stdio.h>
#include <string.h>

void removeIngredientFromMapByTime(warehouseMap *map, int time, String ingredient){
    int hash = sdbm_hash(ingredient);
    ingredientLotListList *hashHead = map->hashArray[hash];
    ingredientLotList *ingredientHead = NULL;
    ingredientLotList *prevIngredientHead = NULL;

    while(ingredientHead == NULL){
        if(strcmp(hashHead->el->el->name, ingredient) == 0){
            ingredientHead = hashHead->el;
        }
        else{
            hashHead = hashHead->next;
        }
    }

    while(ingredientHead->el->time != time){
        prevIngredientHead = ingredientHead;
        ingredientHead = ingredientHead->next;
    }
    removeNodeFromIngredientMap(map, hashHead, ingredientHead, prevIngredientHead);
}

void removeNodeFromIngredientMap(warehouseMap *map, ingredientLotListList *hashHead, ingredientLotList *ingredientHead, ingredientLotList *prevIngredientHead){
    hashHead->totalAmount -= ingredientHead->el->amount;
    if(prevIngredientHead != NULL){
        prevIngredientHead->next = ingredientHead->next;
    }
    else{
        hashHead->el = ingredientHead->next;
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
        hashHead->ingredientName = malloc(strlen(s->name)+1);
        strcpy(hashHead->ingredientName, s->name);

        ingredientHead->el = s;
        ingredientHead->next = NULL;

        map->hashArray[hash] = hashHead;
    }
    else{
        //The hashHead already existed, locate ingredientHead
        ingredientLotListList *hashHead = map->hashArray[hash];


        while(hashHead != NULL){
            if(strcmp(hashHead->ingredientName, s->name) == 0){
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
                hashHead->ingredientName = malloc(strlen(s->name)+1);
                strcpy(hashHead->ingredientName, s->name);
                break;
            }
            hashHead = hashHead->next;
        }
        


        //We are now necessarily inside the correct hashHead, add ingredient to the list

        hashHead->totalAmount += s->amount;

        if(hashHead->el == NULL){
            //The ingredient lot we are inserting is the first one for this list
            ingredientLotList *newNode = malloc(sizeof(*newNode));
            hashHead->el = newNode;
            hashHead->el->next = NULL;
            hashHead->el->el = s;
        }
        else{
            ingredientLotList *ingredientNavigator = hashHead->el;
            //Need to locate correct position inside ingredientList
            if(ingredientNavigator->el->time > s->time){
                //earliest ingredient in list is still later than s, replace list head
                ingredientLotList *newNode = malloc(sizeof(*newNode));
                hashHead->el = newNode;
                hashHead->el->next = ingredientNavigator;
                hashHead->el->el = s;
            }
            else{
                while(ingredientNavigator != NULL){
                    if(ingredientNavigator->el->time == s->time){
                        s->amount += ingredientNavigator->el->amount;
                        free(ingredientNavigator->el);
                        ingredientNavigator->el = s;

                        break;
                    }
                    else if(ingredientNavigator->el->time < s->time && (ingredientNavigator->next == NULL || ingredientNavigator->next->el->time > s->time)){
                        ingredientLotList *newNode = malloc(sizeof(*newNode));
                        newNode->el = s;
                        newNode->next = ingredientNavigator->next;
                        ingredientNavigator->next = newNode;

                        break;
                    }

                    ingredientNavigator = ingredientNavigator->next;
                }
            }
        }
    }
}