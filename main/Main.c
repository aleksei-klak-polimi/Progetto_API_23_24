#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAPSIZE 1024
#define STRINGSIZE 256





//OPERATIONAL STRUCTURES
typedef char String[STRINGSIZE];

typedef struct StringList{
    String              el;
    struct StringList   *next;
}StringList;





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
    struct numberedTimedItem        *el;
    struct numberedTimedItemList    *next;
};

struct numberedTimedItemListList{
    struct numberedTimedItemList        *el;
    struct numberedTimedItemListList    *next;
};


struct numberedTimedItemListMap{
    struct numberedTimedItemListList *hashArray[HASHMAPSIZE];
};


struct numberedTimedItemListTree{
    int         expiration;
    StringList  *ingredients;

    int                              isBlack;
    struct numberedTimedItemListTree *parent;
    struct numberedTimedItemListTree *right;
    struct numberedTimedItemListTree *left;
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
typedef struct numberedTimedItemListList        ingredientLotListList;
typedef struct numberedTimedItemListMap         warehouseMap;
typedef struct numberedTimedItemListTree        warehouseTreeNode;

typedef struct Courier                          Courier;





//METHOD DECLARATION
//INSTRUCTIONS
int             addRecipie(recipiesMap *book);
int             removeRecipie(recipiesMap *book);
int             resupply(warehouseMap *map, warehouseTreeNode *root);
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

//SUPPLIES
void            updateInventory(warehouseMap *map, warehouseTreeNode *root);
void            supplyInventory(warehouseMap *map, warehouseTreeNode *root, ingredientLotList *s);
void            addIngredientToTree(warehouseTreeNode **d_root, ingredientLot *s);
int             readSupplies(ingredientLotList *s);
void            printSupplies(ingredientLotList *s);
void            printInventory(warehouseMap *map, warehouseTreeNode *root);

//COURIER
int             setupCourier(Courier *c);





int main(){
    int ch = 0;
    int time = 0;
    int i = 0;
    String command;

    recipiesMap         cookBook;
    warehouseMap        whMap;
    warehouseTreeNode   whTree;
    Courier             courier;

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
            else if(strcmp("rifornimento", command) == 0){
                warehouseMap *map =         &whMap;
                warehouseTreeNode *root =   &whTree;

                resupply(map, root);

                //debug
                //printInventory(map, root);
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

int resupply(warehouseMap *map, warehouseTreeNode *root){
    int ch;

    ingredientLotList *lot;
    lot = malloc(sizeof(*lot));

    ch = readSupplies(lot);
    printSupplies(lot);

    return ch;
}

void addIngredientToTree(warehouseTreeNode **d_root, ingredientLot *s){
    /*  Using double pointer to reference the root because:
        If the tree is still empty then we have to do a malloc
        The malloc assign to the pointer a new memory address
        If we give a simple pointer and malloc, then the caller function has no access to the new malloc memory
        If we have a pointer that points to the head then we can malloc the head and the caller still has the reference to
        the double pointer therefore sees the new malloc-ed memory.
    */


    if(*d_root == NULL){
        //If the double pointer points to NULL then we need to initialize the tree with a root.
        warehouseTreeNode *root = malloc(sizeof(*root));
        d_root = &root;

        root->right = NULL;
        root->left = NULL;
        root->parent = NULL;
        root->expiration = s->time;
        root->isBlack = 1;
        
        StringList *ingredients = malloc(sizeof(*ingredients));
        strcpy(ingredients->el, s->name);
        ingredients->next = NULL;
        root->ingredients = ingredients;
    }
    else{
        //The double pointer points to a valid root therefore we need to insert into the tree

        int lotExpiration = s->time;
        int parentFound = 0;
        warehouseTreeNode *parentLeaf = *d_root;


        //Navigating tree to find closest leaf to lot expiration
        while(parentFound == 0){
            if(parentLeaf->expiration > lotExpiration){
                if(parentLeaf->left == NULL){
                    parentFound = 1;
                }
                else{
                    parentLeaf = parentLeaf->left;
                }
            }
            else if(parentLeaf->expiration < lotExpiration){
                if(parentLeaf->right == NULL){
                    parentFound = 1;
                }
                else{
                    parentLeaf = parentLeaf->right;
                }
            }
            else if(parentLeaf->expiration == lotExpiration){
                parentFound = 1;
            }
        }

        if(parentLeaf->expiration == lotExpiration){
            //Leaf with required expiration already exists, no new leafs created, only adding ingredient to list.
            StringList *ingredients = parentLeaf->ingredients;

            int breaker = 0;
            while(breaker == 0){
                if(strcmp(ingredients->el, s->name) == 0){
                    breaker = 1;
                }
                else if(ingredients->next == NULL){
                    StringList *node = malloc(sizeof(*node));
                    strcpy(node->el, s->name);
                    node->next = NULL;
                    ingredients->next = node;

                    breaker = 1;
                }
                else{
                    ingredients = ingredients->next;
                }
            }
        }
        else{
            //No existing leaf has required expiration, creating new leaf
            warehouseTreeNode *leaf = malloc(sizeof(*leaf));

            leaf->right = NULL;
            leaf->left = NULL;
            leaf->parent = parentLeaf;
            leaf->expiration = lotExpiration;
            leaf->isBlack = 0;
        
            StringList *ingredients = malloc(sizeof(*ingredients));
            strcpy(ingredients->el, s->name);
            ingredients->next = NULL;
            leaf->ingredients = ingredients;

            if(parentLeaf->expiration > lotExpiration){
                parentLeaf->left = leaf;
            }
            else if(parentLeaf->expiration < lotExpiration){
                parentLeaf->right = leaf;
            }


            //Red-Black tree may not balanced, may need to perform a rotation
            if(parentLeaf->isBlack == 0){
                //parent is red
                //recoloring parent and uncle to black
                parentLeaf->parent->right->isBlack = 1;
                parentLeaf->parent->left->isBlack = 1;

                //Moving up one level
                warehouseTreeNode *grandpa = parentLeaf->parent;
                warehouseTreeNode *greatGrandpa = grandpa->parent;

                //Determining which rotation to perform
                if(grandpa->right->expiration == parentLeaf->expiration){
                    //Perform left rotation on grandpa
                    grandpa->parent = parentLeaf;
                    grandpa->right = parentLeaf->left;

                    parentLeaf->left->parent = grandpa;

                    parentLeaf->left = grandpa;
                    parentLeaf->parent = greatGrandpa;

                    if(greatGrandpa->right->expiration == grandpa->expiration){
                        greatGrandpa->right = parentLeaf;
                    }
                    else{
                        greatGrandpa->left = parentLeaf;
                    }
                }
                else{
                    //Perform right rotation on grandpa
                    grandpa->parent = parentLeaf;
                    grandpa->left = parentLeaf->right;

                    parentLeaf->right->parent = grandpa;

                    parentLeaf->right = grandpa;
                    parentLeaf->parent = greatGrandpa;

                    if(greatGrandpa->right->expiration == grandpa->expiration){
                        greatGrandpa->right = parentLeaf;
                    }
                    else{
                        greatGrandpa->left = parentLeaf;
                    }
                }
            }
        }
    }
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

void printSupplies(ingredientLotList *s){
    printf("\nLot:\n");
    
    ingredientLotList *ingr = s;
    while(ingr != NULL){
        printf("Ingredient: %s\nAmount: %d\nExpiration: %d\n", ingr->el->name, ingr->el->amount, ingr->el->time);
        ingr = ingr->next;
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