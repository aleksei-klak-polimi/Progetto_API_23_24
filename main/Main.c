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
    struct numberedItem     el;
    struct numberedItemList *next;
};


struct namedNumberedItemList{
    String                      name;
    struct numberedItemList     head;
};


struct namedNumberedItemListList{
    struct namedNumberedItemList        el;
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

typedef struct Courier                          Courier;





//METHOD DECLARATION
//INSTRUCTIONS
int addRecipie(recipiesMap book);
int removeRecipie(recipiesMap book);
int resupply();
int order();
int loadCurrier();

//RECIPIES
unsigned int    sdbm_hash(String string);
void            insertRecipie(recipiesMap book, recipie recipie);
void            deleteRecipie(recipiesMap book, String name);
recipie         retrieveRecipie(recipiesMap book, String name);

//COURIER
int setupCourier(Courier *c);

int updateInventory();





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
                addRecipie(cookBook);
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

int addRecipie(recipiesMap book){
    //todo placeholder
    return 0;
}

void insertRecipie(recipiesMap book, recipie recipie){
    int hash = sdbm_hash(recipie.name);

    if(book.hashArray[hash] == NULL){
        recipiesList head;
        head.el = recipie;
        head.next = NULL;

        book.hashArray[hash] = &head;
    }
    else{
        recipiesList node = *book.hashArray[hash];

        while(node.next != NULL){   
            if(node.el.name == recipie.name)
                break;
            else{
                node = *node.next;
            }
        }

        recipiesList newNode;
        newNode.el = recipie;
        newNode.next = NULL;

        node.next = &newNode;
    }
}

unsigned int sdbm_hash(String string){
    unsigned long hash = 0;
    int c;

    while((c = *string++)){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash % HASHMAPSIZE;
}