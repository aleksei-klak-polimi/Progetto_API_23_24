#include "Warehouse.h"

void removeIngredientsFromWarehouseByTime(warehouseTreeNode **root, warehouseMap *map, int time){
    //Removes all ingredients with expiration at the provided time from both Map and tree
    StringList *ingredients = removeNodeFromTreeByTime(root, time);

    StringList *prev;
    while(ingredients != NULL){
        removeIngredientFromMapByTime(map, time, ingredients->el);
        prev = ingredients;
        ingredients = ingredients->next;
        free(prev);
    }
}

int removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity){
    //Returns 0 if there were less ingredients available than specified amount and nothing is deleted
    //Returns 1 if the ingredients were successfully removed

    //Check if there are enough ingredients for recipie
    ingredientList *firstIngredientNode = recipie->head;
    ingredientList *currentIngredientNode = firstIngredientNode;
    ingredient *ingredient;

    int hash;

    ingredientLotListList *hashHead;

    //check for each ingredient if there are enough in storage
    while(currentIngredientNode != NULL){
        ingredient = currentIngredientNode->el;
        hash = sdbm_hash(ingredient->name);

        hashHead = map->hashArray[hash];

        int breaker = 0;
        while(breaker == 0){
            if(hashHead == NULL){
                return 0;
            }
            if(strcmp(hashHead->el->el->name, ingredient->name) == 0){
                breaker = 1;
                if(hashHead->totalAmount < (ingredient->amount *quantity)){
                    return 0;
                }
            }
            else if(hashHead->next == NULL){
                return 0;
            }
            else{
                hashHead = hashHead->next;
            }
        }
        currentIngredientNode = currentIngredientNode->next;
    }
    //If function has not returned at this point then each ingredient of the recipie is found in the warehouse with enough amount


    currentIngredientNode = firstIngredientNode;
    int totalAmount = currentIngredientNode->el->amount *quantity;

    ingredientLotListList *prevHashHead = NULL;
    ingredientLotList *ingredientHead;
    ingredientLotList *prevIngredientHead = NULL;

    while(currentIngredientNode != NULL){
        //Locate the hashHead
        hash = sdbm_hash(currentIngredientNode->el->name);
        hashHead = map->hashArray[hash];
        ingredient = currentIngredientNode->el;

        int breaker = 0;
        while(breaker == 0){
            if(strcmp(hashHead->el->el->name, ingredient->name) == 0){
                breaker = 1;
            }
            else{
                    prevHashHead = hashHead;
                    hashHead = hashHead->next;
                }
        }
        //HashHead located
        ingredientHead = hashHead->el;

        //Trim the list of Lots
        while(totalAmount > 0){
            if(totalAmount >= ingredientHead->el->amount){
                //Need to delete the whole node as all ingredients are used

                //Decrease remaining amount
                totalAmount -= ingredientHead->el->amount;

                //Go to next node
                prevIngredientHead = ingredientHead;
                ingredientHead = ingredientHead->next;

                //Delete prev node from tree and Map
                removeIngredientFromTreeByTime(root, prevIngredientHead->el->time, prevIngredientHead->el->name);
                removeNodeFromIngredientMap(map, hash, hashHead, prevHashHead, prevIngredientHead, NULL);
            }
            else{
                //Ingredients will be left over so no need to delete the node, only decrease the amount in hashHead
                hashHead->totalAmount -= totalAmount;
                totalAmount = 0;
            }
        }

        currentIngredientNode = currentIngredientNode->next;
    }
    return 1;
}

