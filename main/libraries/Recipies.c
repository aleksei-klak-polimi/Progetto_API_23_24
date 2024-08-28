#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Recipies.h"

int readRecipie(recipie *r){
    int ch;
    int i = 0;

    //READ NAME
    while((ch = fgetc(stdin)) != '\n' && ch != ' ' && ch != EOF){
        r->name[i] = ch;
        i++;
    }
    r->name[i] = '\0';


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
            ingr->name[i] = ch;
            i++;
        }
        ingr->name[i] = '\0';

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

    return ch;
}

void insertRecipie(recipiesMap *book, recipie *recipie){
    //Inserts given recipie into recipie book
    //malloc recipiesList

    int hash = sdbm_hash(recipie->name);
    int duplicate = 0;

    if(book->hashArray[hash] == NULL){
        //No recipies with same hash, create first hashHead
        recipiesList *hashHead = malloc(sizeof(*hashHead));
        hashHead->el = recipie;
        hashHead->next = NULL;
        hashHead->ordersPending = 0;

        book->hashArray[hash] = hashHead;

        printf("aggiunta\n");
    }
    else{
        recipiesList *hashNode = book->hashArray[hash];

        /*
        While(node != NULL) causes segmentation fault at line node->next = newNode
        While(node->next != NULL) skips the check on if the names of the recipies are the same when the list is size = 1
        do while allows to check the name on size 1 and prevents segmentation fault.
        */
        do {   
            if(strcmp(hashNode->el->name, recipie->name) == 0){
                duplicate = 1;
                break;
            }
            else{
                hashNode = hashNode->next;
            }
        }
        while(hashNode->next != NULL);

        if(duplicate == 0){
            recipiesList *newNode;
            newNode = malloc(sizeof(*newNode));
            newNode->el = recipie;
            newNode->next = NULL;
            newNode->ordersPending = 0;

            hashNode->next = newNode;

            printf("aggiunta\n");
        }
        else{
            printf("ignorato\n");
        }
    }
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
        recipiesList *prev = book->hashArray[hash];
        recipiesList *target;

        //CHECK IF THE HEAD OF THE LIST IS THE MATCH
        if(strcmp(book->hashArray[hash]->el->name, name) == 0){
            found = 1;
            if(book->hashArray[hash]->ordersPending == 0){
                allowed = 1;
                target = book->hashArray[hash];

                //If next was null then array now has position[hash] = null
                //if next was another node then old_next is the head at array[hash]
                book->hashArray[hash] = prev->next;
            }
        }
        else{
            //Recipie to remove is not the head, checking other nodes
            
            //To keep the list linked, we iterate through the list using prev and see if prev->next is the match
            //this way we can link together the remainder of the list after we cut out prev->next
            while(prev->next != NULL){
                if(strcmp(prev->next->el->name, name) == 0){
                    found = 1;
                    if(book->hashArray[hash]->ordersPending == 0){
                        allowed = 1;
                        target = prev->next;

                        //Remove the target from list
                        prev->next = target->next;
                    }
                    break;
                }
                else{
                    prev = prev->next;
                }
            }
        }

        if(found == 0){
            printf("non presente\n");
        }
        else if(allowed == 1){
            //CLEARING MEMORY
            ingredientList *ingredientCurr = target->el->head;
            ingredientList *ingredientPrev = NULL;

            while(ingredientCurr != NULL){
                //Clear all ingredients
                ingredientPrev = ingredientCurr;
                ingredientCurr = ingredientCurr->next;
                free(ingredientPrev);
            }

            //Clear recipie and hashHead from memory
            free(target->el);
            free(target);


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