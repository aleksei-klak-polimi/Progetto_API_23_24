#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Recipies.h"

int readRecipie(recipiesMap *book, recipie *r){
    int ch;
    int i = 0;
    char buffer[256];

    //READ NAME
    while((ch = fgetc(stdin)) != '\n' && ch != ' ' && ch != EOF){
        buffer[i] = ch;
        i++;
    }
    buffer[i] = '\0';

    r->name = malloc(strlen(buffer)+1);
    strcpy(r->name, buffer);


    //Check if recipie is already in the map before reading the rest of the ingredients
    int duplicate = 0;
    int hash = sdbm_hash(r->name);

    recipiesList *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->el->name, r->name) == 0){
            duplicate = 1;
            break;
        }

        hashHead = hashHead->next;
    }

    if(duplicate == 1){
        //recipie with same name was already stored in the map

        //free the room already allocated to the recipie
        free(r);

        printf("ignorato\n");

        //skip stdin to next line, ignore the rest of the recipie
        while(ch != '\n' && ch != EOF){
            ch = getc(stdin);
        }

        return ch;
    }

    //If function did not return then duplicate was 0, keep reading the recipie as normal



    ingredientList *head = NULL;
    ingredientList *prev = NULL;
    ingredientList *current = NULL;

    int totalWeigth = 0;
    
    //READ INGREDIENTS
    while(ch != '\n' && ch != EOF){
        ingredient *ingr = malloc(sizeof(*ingr));

        //READ INGREDIENT
        i = 0;
        while((ch = fgetc(stdin)) != ' '){
            buffer[i] = ch;
            i++;
        }
        buffer[i] = '\0';

        ingr->name = malloc(strlen(buffer)+1);
        strcpy(ingr->name, buffer);



        //READ AMOUNT
        i = 0;
        String amount;
        while((ch = fgetc(stdin)) != ' ' && ch != '\n' && ch != EOF){
            amount[i] = ch;
            i++;
        }
        amount[i] = '\0';
        ingr->amount = atoi(amount);
        totalWeigth += ingr->amount;


        //STORE INGREDIENT IN NODE
        if(current != NULL){
            prev = current;
        }
        current = malloc(sizeof(*current));
        current->el = ingr;
        current->next = NULL;
        //LINK PREV NODE TO NEW NODE
        if(prev != NULL){
            prev->next = current;
        }
        else if(head == NULL){
            head = current;
        }
    }

    //ADD LIST OF INGREDIENTS TO RECIPIE
    r->head = head;
    r->weight = totalWeigth;

    //Insert recipie into map
    hashHead = malloc(sizeof(*hashHead));
    hashHead->el = r;
    hashHead->ordersPending = 0;

    hashHead->next = book->hashArray[hash];
    book->hashArray[hash] = hashHead;

    printf("aggiunta\n");


    return ch;
}

int removeRecipie(recipiesMap *book){
    int ch;
    int i = 0;
    String name;
    int hash;

    //READ RECIPIE NAME
    while((ch = fgetc(stdin)) != '\n' && ch != EOF){
        name[i] = ch;
        i++;
    }
    name[i] = '\0';

    hash = sdbm_hash(name);



    //CHECK IF MAP CONTAINS HASH
    if(book->hashArray[hash] == NULL){
        printf("non presente\n");
    }
    else{
        //IF MAP CONTAINS HASH CHECK THE LIST FOR MATCHES
        int found = 0;
        int allowed = 0;
        recipiesList *prevHashHead = NULL;
        recipiesList *currHashHead = book->hashArray[hash];

        //CHECK IF THE HEAD OF THE LIST IS THE MATCH
        if(strcmp(book->hashArray[hash]->el->name, name) == 0){
            found = 1;
            if(book->hashArray[hash]->ordersPending == 0){
                allowed = 1;

                //If next was null then array now has position[hash] = null
                //if next was another node then old_next is the head at array[hash]
                book->hashArray[hash] = currHashHead->next;
            }
        }
        else{
            //Recipie to remove is not the head, checking other nodes
            
            //To keep the list linked, we iterate through the list using prev and see if prev->next is the match
            //this way we can link together the remainder of the list after we cut out prev->next
            while(currHashHead != NULL){
                if(strcmp(currHashHead->el->name, name) == 0){
                    found = 1;
                    if(currHashHead->ordersPending == 0){
                        allowed = 1;

                        //Remove the target from list
                        prevHashHead->next = currHashHead->next;
                    }
                    break;
                }
                else{
                    prevHashHead = currHashHead;
                    currHashHead = currHashHead->next;
                }
            }
        }

        if(found == 0){
            printf("non presente\n");
        }
        else if(allowed == 1){
            //CLEARING MEMORY
            ingredientList *ingredientCurr = currHashHead->el->head;
            ingredientList *ingredientPrev = NULL;

            while(ingredientCurr != NULL){
                //Clear all ingredients
                ingredientPrev = ingredientCurr;
                ingredientCurr = ingredientCurr->next;
                free(ingredientPrev);
            }

            //Clear recipie and hashHead from memory
            free(currHashHead->el);
            free(currHashHead);


            printf("rimossa\n");
        }
        else if(found == 1 && allowed == 0){
            //the recipie was found but is in use by pending orders
            printf("ordini in sospeso\n");
        }
    }

    return ch;
}

recipie *retrieveRecipie(recipiesMap *book, String name){
    //Given the provided book and recipie name, the function returns a pointer to the recipie if it is found in the book.
    //Returns null otherwise

    int hash = sdbm_hash(name);
    recipiesList *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->el->name, name) == 0){
            return hashHead->el;
        }
        else{
            hashHead = hashHead->next;
        }
    }

    return NULL;
}

void decrementRecipieUtilization(recipiesMap *book, String name){
    int hash = sdbm_hash(name);
    recipiesList *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->el->name, name) == 0){
            hashHead->ordersPending--;
            break;
        }
        else{
            hashHead = hashHead->next;
        }
    }
}

void incrementRecipieUtilization(recipiesMap *book, String name){
    int hash = sdbm_hash(name);
    recipiesList *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->el->name, name) == 0){
            hashHead->ordersPending++;
            break;
        }
        else{
            hashHead = hashHead->next;
        }
    }
}