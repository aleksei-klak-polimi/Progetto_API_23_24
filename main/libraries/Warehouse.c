#include "Warehouse.h"
#include "String.h"
#include <stdlib.h>
#include <string.h>

int resupplyWarehouse(int time, warehouseMap *map, warehouseTreeNode **root){
    int ch = 0;
    int i;
    String nameBuffer;
    String buffer;
    int amount;
    int expiration;
    ingredientLot *s;

    while(ch != '\n' && ch != EOF){


        //ALLOCATE ELEMENT
        s = malloc(sizeof(*s));


        //READ INGREDIENT NAME
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            nameBuffer[i] = ch;
            i++;
        }
        nameBuffer[i] = '\0';


        //READ INGREDIENT AMOUNT
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            buffer[i] = ch;
            i++;
        }
        buffer[i] = '\0';
        amount = atoi(buffer);


        //READ INGREDIENT TIME
        i = 0;
        while((ch = fgetc(stdin)) != ' ' && ch != '\n' && ch != EOF){
            buffer[i] = ch;
            i++;
        }
        buffer[i] = '\0';
        expiration = atoi(buffer);


        if(expiration > time){
            s = malloc(sizeof(*s));
            s->time = expiration;
            s->amount = amount;


            addIngredientToTree(root, nameBuffer, s->time);
            addIngredientToMap(map, s, nameBuffer);
        }
    }
    printf("rifornito\n");

    return ch;
}

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

void removeIngredientsFromWarehouseByOrder(warehouseTreeNode **root, warehouseMap *map, recipie *recipie, int quantity){
    ingredient *firstIngredientNode = recipie->head;
    ingredient *currentIngredientNode = firstIngredientNode;

    ingredientLotListList *hashHead;

    currentIngredientNode = firstIngredientNode;
    int totalAmount;

    ingredientLot *ingredientHead;
    ingredientLot *prevIngredientHead = NULL;

    while(currentIngredientNode != NULL){
        totalAmount = currentIngredientNode->amount * quantity;

        hashHead = currentIngredientNode->ingredientHead;
        //HashHead located
        ingredientHead = hashHead->el;
        prevIngredientHead = NULL;

        //Trim the list of Lots
        while(totalAmount > 0){
            if(totalAmount >= ingredientHead->amount){
                //Need to delete the whole node as all ingredients are used

                //Decrease remaining amount
                totalAmount -= ingredientHead->amount;

                //Go to next node
                prevIngredientHead = ingredientHead;
                ingredientHead = ingredientHead->next;

                //Delete prev node from tree and Map
                removeIngredientFromTreeByTime(root, prevIngredientHead->time, hashHead->ingredientName);
                removeNodeFromIngredientMap(map, hashHead, prevIngredientHead, NULL);
            }
            else{
                //Ingredients will be left over so no need to delete the node, only decrease the amount in hashHead
                hashHead->totalAmount -= totalAmount;

                ingredientHead->amount -= totalAmount;


                totalAmount = 0;
            }
        }

        currentIngredientNode = currentIngredientNode->next;
    }
}

int isOrderFulfillable(warehouseMap *map, recipie *recipie, int quantity){
    //Returns 0 if there were less ingredients available than specified amount and nothing is deleted
    //Returns 1 if the ingredients were successfully removed

    //Check if there are enough ingredients for recipie
    ingredient *firstIngredientNode = recipie->head;
    ingredient *currentIngredientNode = firstIngredientNode;

    //check for each ingredient if there are enough in storage
    while(currentIngredientNode != NULL){

        if(currentIngredientNode->amount * quantity > currentIngredientNode->ingredientHead->totalAmount){
            return 0;
        }

        currentIngredientNode = currentIngredientNode->next;
    }

    return 1;
}

