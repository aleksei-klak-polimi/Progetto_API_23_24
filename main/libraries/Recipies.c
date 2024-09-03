#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Recipies.h"

int readRecipie(recipiesMap *book, recipie *r, struct numberedTimedItemListMap *whMap){
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
    int ingrHash;

    recipie *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->name, r->name) == 0){
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



    int totalWeigth = 0;
    r->head = NULL;
    ingredient *prev = NULL;
    
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


        //LINK INGREDIENT HEAD FROM MAP TO RECIPIE
        ingr->ingredientHead = NULL;
        ingrHash = sdbm_hash(buffer);
        struct numberedTimedItemListList *hashHead = NULL;

        if(whMap->hashArray[ingrHash] == NULL){
            hashHead = malloc(sizeof(*hashHead));
            hashHead->totalAmount = 0;
            hashHead->next = NULL;
            hashHead->el = NULL;
            hashHead->ingredientName = malloc(strlen(buffer)+1);
            strcpy(hashHead->ingredientName, buffer);

            ingr->ingredientHead = hashHead;
            whMap->hashArray[ingrHash] = hashHead;
        }
        else{
            hashHead = whMap->hashArray[ingrHash];

            while(ingr->ingredientHead == NULL){
                if(strcmp(hashHead->ingredientName, buffer) == 0){
                    ingr->ingredientHead = hashHead;
                }
                else if(hashHead->next == NULL){
                    hashHead->next = malloc(sizeof(*hashHead));
                    hashHead = hashHead->next;

                    hashHead->totalAmount = 0;
                    hashHead->next = NULL;
                    hashHead->el = NULL;
                    hashHead->ingredientName = malloc(strlen(buffer)+1);
                    strcpy(hashHead->ingredientName, buffer);

                    ingr->ingredientHead = hashHead;
                }

                hashHead = hashHead->next;
            }
        }





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

        ingr->next = NULL;


        //STORE INGREDIENT IN NODE
        if(r->head == NULL){
            r->head = ingr;
        }
        else if(r->head->amount <= ingr->amount){
            ingr->next = r->head;
            r->head = ingr;
        }
        else{
            prev = r->head;

            while(prev != NULL){
                if(prev->amount >= ingr->amount && (prev->next == NULL || prev->next->amount < ingr->amount)){
                    ingr->next = prev->next;
                    prev->next = ingr;

                    break;
                }

                prev = prev->next;
            }
        }
    }

    //ADD LIST OF INGREDIENTS TO RECIPIE
    r->weight = totalWeigth;

    //Insert recipie into map
    r->ordersPending = 0;

    r->next = book->hashArray[hash];
    book->hashArray[hash] = r;

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
        recipie *prevHashHead = NULL;
        recipie *currHashHead = book->hashArray[hash];

        //CHECK IF THE HEAD OF THE LIST IS THE MATCH
        if(strcmp(book->hashArray[hash]->name, name) == 0){
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
                if(strcmp(currHashHead->name, name) == 0){
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
            ingredient *ingredientCurr = currHashHead->head;
            ingredient *ingredientPrev = NULL;

            while(ingredientCurr != NULL){
                //Clear all ingredients
                ingredientPrev = ingredientCurr;
                ingredientCurr = ingredientCurr->next;
                free(ingredientPrev);
            }

            //Clear recipie and hashHead from memory
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
    recipie *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->name, name) == 0){
            return hashHead;
        }
        else{
            hashHead = hashHead->next;
        }
    }

    return NULL;
}

void decrementRecipieUtilization(recipiesMap *book, String name){
    int hash = sdbm_hash(name);
    recipie *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->name, name) == 0){
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
    recipie *hashHead = book->hashArray[hash];

    while(hashHead != NULL){
        if(strcmp(hashHead->name, name) == 0){
            hashHead->ordersPending++;
            break;
        }
        else{
            hashHead = hashHead->next;
        }
    }
}