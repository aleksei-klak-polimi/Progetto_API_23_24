#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAPSIZE 1024
#define STRINGSIZE 256





//OPERATIONAL STRUCTURES
typedef char String[STRINGSIZE];

enum Command {
    aggiungi_ricetta,
    rimuovi_ricetta,
    rifornimento,
    ordine
};





//STRUCTURES
struct numberedItem{
    String  name;
    int     amount;
};


struct numberedItemList{
    struct numberedItem     *el;
    struct numberedItemList *next;
};


struct namedNumberedItemList{
    String                      name;
    struct numberedItemList     *head;
};


struct namedNumberedItemListList{
    struct namedNumberedItemList        *el;
    struct namedNumberedItemListList    *next;
};


struct namedNumberedItemListListMap{
    struct namedNumberedItemListList   *hashArray[HASHMAPSIZE];
};


struct numberedTimedItem{
    String  name;
    int     amount;
    int     time;
};


struct numberedTimedItemList{
    struct numberedTimedItem        el;
    struct numberedTimedItemList    *next;
};


struct numberedTimedItemListMap{
    struct numberedTimedItemList *hashArray[HASHMAPSIZE];
};

struct numberedTimedItemListTree{
    struct numberedTimedItemList *root;
    struct numberedTimedItemList *right;
    struct numberedTimedItemList *left;
};


struct Courier{
    int                             frequency;
    int                             capacity;
    struct numberedTimedItemList    ordersHead;
};





//TYPEDEFS
typedef struct numberedItem                     ingredient;
typedef struct numberedItemList                 ingredientList;
typedef struct namedNumberedItemList            recipie;
typedef struct namedNumberedItemListList        recipiesList;
typedef struct namedNumberedItemListListMap     recipiesMap;

typedef struct numberedTimedItem                orderedItem;
typedef struct numberedTimedItemList            orderedItemList;

typedef struct numberedTimedItem                ingredientLot;
typedef struct numberedTimedItemList            ingredientLotList;
typedef struct numberedTimedItemListMap         warehouseMap;
typedef struct numberedTimedItemListTree        warehouseTreeNode;

typedef struct Courier                          Courier;





//METHOD DECLARATION
//INSTRUCTIONS
int             addRecipie(recipiesMap *book);
int             removeRecipie(recipiesMap *book);
int             resupply();
int             order();
int             loadCurrier();

//RECIPIES
unsigned int    sdbm_hash(String string);
void            insertRecipie(recipiesMap *book, recipie *recipie);
int             readRecipie(recipie *r);
void            printRecipie(recipie *r);
void            deleteRecipie(recipiesMap book, String name);
recipie         retrieveRecipie(recipiesMap book, String name);
void            printRecipieBook(recipiesMap *book);

//COURIER
int             setupCourier(Courier *c);

int             updateInventory();





int main(){
    int ch = 0;
    int time = 0;
    int i = 0;
    String command;

    recipiesMap cookBook;
    Courier courier;

    //initialize the cookBook to have all entries NULL.
    for(i = 0; i < HASHMAPSIZE; i++){
        cookBook.hashArray[i] = NULL;
    }
    


    //main while loop
    while(ch != EOF){

        //SETUP COURIER
        if(time == 0){
            Courier *courierPointer = &courier;
    
            ch = setupCourier(courierPointer);
            printf("Frequency: %d\nCapacity: %d\n", courier.frequency, courier.capacity);
        }
        else{
            //CHECK IF COURIER TIME
            if(time % courier.frequency == 0){
                
            }





            //RESET ANY PREVIOUS COMMAND
            for(i = 0; i < STRINGSIZE; i++){
                command[i] = '\0';
            }
            //READ NEW COMMAND
            i = 0;
            while((ch = fgetc(stdin)) != ' ' && ch != EOF && ch != '\n'){
                command[i] = ch;
                i++;
            }
            command[i] = '\0';

            //INTERPRET THE COMMAND
            if(strcmp("aggiungi_ricetta", command) == 0){
                recipiesMap *book = &cookBook;
                addRecipie(book);

                //debug
                printRecipieBook(book);
            }
            else if(strcmp("rimuovi_ricetta", command) == 0){
                recipiesMap *book = &cookBook;
                removeRecipie(book);

                //debug
                printRecipieBook(book);
            }



        }


        time++;
    }

    return 0;
}







int setupCourier(Courier *c){
    String input;
    int ch;
    int i = 0;

    while((ch = fgetc(stdin)) != '\n' && ch != EOF){
        input[i] = ch;
        i++;
    }
    input[i] = '\0';

    sscanf(input, "%d %d", &c->frequency, &c-> capacity);

    return ch;
}

int addRecipie(recipiesMap *book){
    int ch;
    recipie *r = malloc(sizeof(*r));
    ch = readRecipie(r);
    insertRecipie(book, r);

    return ch;
}

void insertRecipie(recipiesMap *book, recipie *recipie){
    int hash = sdbm_hash(recipie->name);
    int duplicate = 0;

    if(book->hashArray[hash] == NULL){
        recipiesList *head;
        head = malloc(sizeof(*head));
        head->el = recipie;
        head->next = NULL;

        book->hashArray[hash] = head;
    }
    else{
        recipiesList *node = book->hashArray[hash];

        /*
        While(node != NULL) causes segmentation fault at line node->next = newNode
        While(node->next != NULL) skips the check on if the names of the recipies are the same when the list is size = 1
        do while allows to check the name on size 1 and prevents segmentation fault.
        */
        do {   
            if(strcmp(node->el->name, recipie->name) == 0){
                duplicate = 1;
                break;
            }
            else{
                node = node->next;
            }
        }
        while(node->next != NULL);

        if(duplicate == 0){
            recipiesList *newNode;
            newNode = malloc(sizeof(*newNode));
            newNode->el = recipie;
            newNode->next = NULL;

            node->next = newNode;
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
        //todo check if printf is good for stdout
        printf("non presente\n");
    }
    else{
        //IF MAP CONTAINS HASH CHECK THE LIST FOR MATCHES
        int found = 0;
        recipiesList *prev = book->hashArray[hash];
        recipiesList *target;

        //CHECK IF THE HEAD OF THE LIST IS THE MATCH
        if(strcmp(book->hashArray[hash]->el->name, name) == 0){
            found = 1;
            target = book->hashArray[hash];

            //If next was null then array now has position[hash] = null
            //if next was another node then old_next is the head at array[hash]
            book->hashArray[hash] = prev->next;
        }
        else{
            
            //To keep the list linked, we iterate through the list using prev and see if prev->next is the match
            //this way we can link together the remainder of the list after we cut out prev->next

            while(prev->next != NULL){
                if(strcmp(prev->next->el->name, name) == 0){
                    found = 1;
                    target = prev->next;

                    //Remove the target from list
                    prev->next = target->next;

                    break;
                }
                else{
                    prev = prev->next;
                }
            }
        }

        if(found == 0){
            //todo check if printf is good for stdout
            printf("non presente\n");
        }
        else{
            //CLEARING MEMORY
            free(target);
            printf("rimossa\n");
        }
    }

    return ch;
}





//PARSING FUNCTIONS
int readRecipie(recipie *r){
    int ch;
    int i = 0;

    //READ NAME
    while((ch = fgetc(stdin)) != '\n' && ch != ' ' && ch != EOF){
        r->name[i] = ch;
        i++;
    }
    r->name[i] = '\0';


    ingredientList *head = 0;
    ingredientList *prev = 0;
    ingredientList *current = 0;
    
    //READ INGREDIENTS
    while(ch != '\n' && ch != EOF){
        ingredient *ingr;
        ingr = malloc(sizeof(*ingr));

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


        //STORE INGREDIENT IN NODE
        if(current != 0){
            prev = current;
        }
        current = malloc(sizeof(*current));
        current->el = ingr;
        current->next = 0;
        //LINK PREV NODE TO NEW NODE
        if(prev != 0){
            prev->next = current;
        }
        else if(head == 0){
            head = current;
        }
    }

    //ADD LIST OF INGREDIENTS TO RECIPIE
    r->head = head;

    return ch;
}





//DEBUG PRINTING FUNCTIONS
void printRecipieBook(recipiesMap *book){
    int i;
    for(i = 0; i < HASHMAPSIZE; i++){
        if(book->hashArray[i] != NULL){
            recipiesList *node = book->hashArray[i];

            while(node != NULL){
                printRecipie(node->el);
                node = node->next;
            }
        }
    }
}

void printRecipie(recipie *r){
    printf("\nRecipie name: %s\n", r->name);

    ingredientList *ingr = r->head;

    int i = 0;
    while(ingr != 0){
        printf("Ingredient %d: %s, %d\n", i, ingr->el->name, ingr->el->amount);
        ingr = ingr->next;
        i++;
    }
}





//UTILITY FUNCTIONS
unsigned int sdbm_hash(String string){
    unsigned long hash = 0;
    int c;

    while((c = *string++)){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash % HASHMAPSIZE;
}