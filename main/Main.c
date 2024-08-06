#include <stdio.h>
#include <stdlib.h>

#define HASHMAPSIZE 512
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
    struct namedNumberedItemList   hashArray[HASHMAPSIZE];
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
int addRecipie();
int removeRecipie();
int resupply();
int order();
int loadCurrier();

//RECIPIES
void insertRecipie(recipiesMap book, recipie recipie);
void deleteRecipie(recipiesMap book, String name);
recipie retrieveRecipie(recipiesMap book, String name);

//COURIER
void setupCourier(Courier *c);





int main(){
    Courier courier;
    Courier *courierPointer = &courier;
    
    setupCourier(courierPointer);

    printf("Frequency: %d\nCapacity: %d\n", courier.frequency, courier.capacity);

    while(1 > 2){
        //Primo check da fare su camioncino
        //Dopo implementare switch case sulle keyword per capire quale funzione chiamare
    }

    return 0;
}

void setupCourier(Courier *c){
    String input;
    int ch;
    int i = 0;

    printf("Enter the two numbers\n");

    while((ch = fgetc(stdin)) != '\n' && ch != EOF){
        input[i] = ch;
        i++;
    }
    input[i] = '\0';

    sscanf(input, "%d %d", &c->frequency, &c-> capacity);

    return;
}