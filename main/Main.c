#include <stdio.h>
#include <stdlib.h>

#define HASHMAPSIZE 511
#define STRINGSIZE 255

//METHOD DECLARATION
int addRecipie();
int removeRecipie();
int resupply();
int order();
int loadCurrier();





//OPERATIONAL STRUCTURES
typedef struct String {
    char    characters[STRINGSIZE];
} String;

enum Command {
    aggiungi_ricetta,
    rimuovi_ricetta,
    rifornimento,
    ordine
};





//STRUCTURES
typedef struct numberedItem{
    String  name;
    int     amount;
}numberedItem;


typedef struct numberedItemList{
    numberedItem             el;
    struct numberedItemList *next;
}numberedItemList;


typedef struct namedNumberedItemList{
    String              name;
    numberedItemList    head;
}namedNumberedItemList;


typedef struct namedNumberedItemListList{
    namedNumberedItemList               el;
    struct namedNumberedItemListList    *next;
}namedNumberedItemListList;


typedef struct namedNumberedItemListListMap{
    namedNumberedItemList   hashArray[HASHMAPSIZE];
}namedNumberedItemListListMap;


typedef struct numberedTimedItem{
    String  name;
    int     amount;
    int     time;
}numberedTimedItem;


typedef struct numberedTimedItemList{
    numberedTimedItem                el;
    struct numberedTimedItemList    *next;
}numberedTimedItemList;



typedef numberedItem                    ingredient;
typedef numberedItemList                ingredientList;
typedef namedNumberedItemList           recipie;
typedef namedNumberedItemListList       recipiesList;
typedef namedNumberedItemListListMap    recipiesMap;

typedef numberedTimedItem               orderedItem;
typedef numberedTimedItemList           orderedItemList;
typedef numberedTimedItem               ingredientLot;
typedef numberedTimedItemList           ingredientLotList;





void insertRecipie(recipiesMap book, recipie recipie);
void deleteRecipie(recipiesMap book, String name);
recipie retrieveRecipie(recipiesMap book, String name);





//
typedef struct Courier{
    int             frequency;
    int             capacity;
    orderedItemList head;
} Courier;

void setupCourier(Courier *c);




int main(){
    Courier courier;
    Courier *courierPointer = &courier;
    
    setupCourier(courierPointer);

    printf("Frequency: %d\nCapacity: %d", courier.frequency, courier.capacity);

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
        input.characters[i] = ch;
        i++;
    }
    input.characters[i] = '\0';

    sscanf(input.characters, "%d %d", &c->frequency, &c-> capacity);

    return;
}