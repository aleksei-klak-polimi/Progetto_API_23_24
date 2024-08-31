#include "Warehouse.h"

int readSupplies(ingredientLotList *s){
    int ch = 0;
    int i;
    ingredientLotList *current = s;

    while(ch != '\n' && ch != EOF){


        //ALLOCATE ELEMENT
        ingredientLot *el;
        el = malloc(sizeof(*el));


        //READ INGREDIENT NAME
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            el->name[i] = ch;
            i++;
        }
        el->name[i] = '\0';


        //READ INGREDIENT AMOUNT
        i = 0;
        String amount;
        while((ch = fgetc(stdin)) != ' '){
            amount[i] = ch;
            i++;
        }
        amount[i] = '\0';
        el->amount = atoi(amount);


        //READ INGREDIENT TIME
        i = 0;
        String time;
        while((ch = fgetc(stdin)) != ' ' && ch != '\n' && ch != EOF){
            time[i] = ch;
            i++;
        }
        time[i] = '\0';
        el->time = atoi(time);


        //UPDATE LIST
        current->el = el;
        if(ch == ' '){
            current->next = malloc(sizeof(*current));
            current = current->next;
        }
        else{
            current->next = NULL;
        }
    }

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
            else{
                hashHead = hashHead->next;
            }
        }
        currentIngredientNode = currentIngredientNode->next;
    }
    //If function has not returned at this point then each ingredient of the recipie is found in the warehouse with enough amount


    currentIngredientNode = firstIngredientNode;
    int totalAmount;

    ingredientLotListList *prevHashHead = NULL;
    ingredientLotList *ingredientHead;
    ingredientLotList *prevIngredientHead = NULL;

    while(currentIngredientNode != NULL){
        totalAmount = currentIngredientNode->el->amount * quantity;

        //Locate the hashHead
        hash = sdbm_hash(currentIngredientNode->el->name);
        hashHead = map->hashArray[hash];
        prevHashHead = NULL;
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
        prevIngredientHead = NULL;

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

                ingredientHead->el->amount -= totalAmount;


                totalAmount = 0;
            }
        }

        currentIngredientNode = currentIngredientNode->next;
    }
    return 1;
}

