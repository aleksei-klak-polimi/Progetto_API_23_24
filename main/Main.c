#include <stdio.h>


//METHOD DECLARATION
int addRecipie();
int removeRecipie();
int resupply();
int order();
int loadCurrier();





//OPERATIONAL STRUCTURES
typedef struct String {
    char    characters[255];
} String;

enum Command {
    aggiungi_ricetta,
    rimuovi_ricetta,
    rifornimento,
    ordine
};



#define HASHMAPSIZE 512

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





int main(){

    while(1 > 2){
        //Primo check da fare su camioncino
        //Dopo implementare switch case sulle keyword per capire quale funzione chiamare
    }



    printf("Hello World!");
    return 0;
}