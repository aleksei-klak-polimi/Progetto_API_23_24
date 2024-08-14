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
int             resupply(warehouseMap *map, warehouseTreeNode **root);
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
StringList      *removeNodeFromTreeByTime(warehouseTreeNode **d_root, int time);
void            deleteNodeFromTree(warehouseTreeNode **root, warehouseTreeNode *node);
void            rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x);
void            rebalanceTreeAfterInsertion(warehouseTreeNode **root, warehouseTreeNode *x);
void            leftRotate(warehouseTreeNode **root, warehouseTreeNode *x);
void            rightRotate(warehouseTreeNode **root, warehouseTreeNode *x);
int             readSupplies(ingredientLotList *s);
void            printSupplies(ingredientLotList *s);
void            printRBTree(warehouseTreeNode *node, int level);
void            printSpaces(int count);
void            addIngredientToMap(warehouseMap *map, ingredientLot*s);

//COURIER
int             setupCourier(Courier *c);





int main(){
    int ch = 0;
    int time = 0;
    int i = 0;
    String command;

    recipiesMap         cookBook;
    warehouseMap        whMap;
    warehouseTreeNode   *whTree = NULL;
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
            warehouseTreeNode **root =   &whTree;
            //Remove expired ingredients
            removeNodeFromTreeByTime(root, time);
            printf("\nRemoved expired:\n");
            printRBTree(*root, 0);


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

                resupply(map, root);

                //debug
                printRBTree(*root, 0);
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

int resupply(warehouseMap *map, warehouseTreeNode **root){
    int ch;

    ingredientLotList *lot;
    lot = malloc(sizeof(*lot));

    ch = readSupplies(lot);

    ingredientLotList *navigator = lot;
    while(navigator != NULL){
        addIngredientToTree(root, navigator->el);
        addIngredientToMap(map, navigator->el);
        navigator = navigator->next;
    }

    //Clean the list used to store the lots that were read
    ingredientLotList *curr = lot;
    ingredientLotList *next;

    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }


    printf("rifornito\n");

    return ch;
}





//RED-BLACK TREE WAREHOUSE FUNCTIONS
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
        *d_root = root;

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
            rebalanceTreeAfterInsertion(d_root, leaf);
        }
    }
}

StringList *removeNodeFromTreeByTime(warehouseTreeNode **d_root, int time){
    if(*d_root == NULL){
        return NULL;
    }

    StringList *returnList;
    warehouseTreeNode *leaf = *d_root;
    int breaker = 0;
    int removeLeaf = 0;
    while(breaker == 0){
        if(leaf->expiration == time){
            returnList = leaf->ingredients;
            removeLeaf = 1;
            breaker = 1;
        }
        else if(leaf->expiration > time){
            if(leaf->left == NULL){
                return NULL;
            }
            else{
                leaf = leaf->left;
            }
        }
        else if(leaf->expiration < time){
            if(leaf->right == NULL){
                return NULL;
            }
            else{
                leaf = leaf->right;
            }
        }
    }
    if(removeLeaf == 1){
        deleteNodeFromTree(d_root, leaf);
    }

    return returnList;
}

void removeIngredientFromTreeByTime(warehouseTreeNode **d_root, int time, String ingredient){
    //Finds the node in the tree associated to the time provided, and within the node deletes the
    //specified ingredient, if no ingredients are left afterwards then the node is deleted.
    //Frees from memory the string in the node

    int breaker = 0;
    warehouseTreeNode *node = *d_root;
    while(breaker == 0){
        if(node->expiration == time){
            breaker = 1;
        }
        else if(node->expiration > time){
            node = node->left;
        }
        else if(node->expiration < time){
            node = node->right;
        }
    }

    StringList *ingredients = node->ingredients;
    if(strcmp(ingredients->el, ingredient) == 0 && ingredients->next == NULL){
        //If the ingredient is the only ingredient in the node
        free(ingredients);
        deleteNodeFromTree(d_root, node);
    }
    else{
        while(strcmp(ingredients->next->el, ingredient) != 0){
            ingredients = ingredients->next;
        }

        StringList *ingredientFound = ingredients->next;
        ingredients->next = ingredientFound->next;
        free(ingredientFound);
    }
}

void deleteNodeFromTree(warehouseTreeNode **root, warehouseTreeNode *node){
    //STEP 1: perform simple deletion


    //If node has no children simply delete node and pass to the rebalance function a NIL node representing the deleted node
    if(node->right == NULL && node->left == NULL){
        warehouseTreeNode *parent = node->parent;

        if(parent != NULL){
            //If the node to delete is not the root then sever the parent-child connection
            if(parent->right == node){
                parent->right = NULL;
            }
            else{
                parent->left = NULL;
            }
            //todo check if this free does not delete the list I am returning
            rebalanceTreeAfterDelete(root, node);
            free(node);
        }
        else{
            //The node to delete is the root
            *root = NULL;

            //todo check if this free does not delete the list I am returning
            free(node);
        }
    }


    //If node has one child copy the child in the parent and delete the child
    else if(node->right == NULL || node->left == NULL){

        warehouseTreeNode *child;
        warehouseTreeNode *parent = node;
        
        if(parent->right != NULL){
            child = parent->right;
        }
        else{
            child = parent->left;
        }

        parent->expiration = child->expiration;
        parent->ingredients = child->ingredients;
        parent->right = child->right;
        parent->left = child->left;

        if(child->right != NULL){
            child->right->parent = parent;
        }
        if(child->left != NULL){
            child->left->parent = parent;
        }

        //todo check if this free does not delete the list I am returning
        rebalanceTreeAfterDelete(root, child);
        free(child);
    }


    //If node has two children, find its successor, copy it into the node and perform deletion of the successor
    else if(node->right != NULL && node->left != NULL){
        //find smallest successor to our node
        warehouseTreeNode *successor = node->right;

        int breaker = 0;
        while(breaker == 0){
            if(successor->left == NULL){
                breaker = 1;
            }
            else{
                successor = successor->left;
            }
        }

        //copy contents of successor into node
        node->expiration = successor->expiration;
        node->ingredients = successor->ingredients;

        //remove old successor node from tree
        deleteNodeFromTree(root, successor);
    }
}

void rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x){
    // While x is not the root and x is black
    while (x != *root && x->isBlack) {
        if (x != x->parent->right && x->parent->right != NULL) {  // x is the left child
            warehouseTreeNode *w = x->parent->right; // w is x's sibling
            if (!w->isBlack) { // Case 1: x's sibling w is red
                w->isBlack = 1;               // Recolor w as black
                x->parent->isBlack = 0;        // Recolor x's parent as red
                leftRotate(root, x->parent);   // Left rotate on x's parent
                w = x->parent->right;          // Update w to be the new sibling of x
            }
            if ((w->left == NULL || w->left->isBlack == 1) && (w->right == NULL || w->right->isBlack == 1)) { // Case 2: Both of w's children are black
                w->isBlack = 0;               // Recolor w as red
                x = x->parent;                // Move x up the tree
            }
            else {
                if (w->right == NULL || w->right->isBlack == 1) { // Case 3: w's right child is black
                    if(w->left != NULL){
                        w->left->isBlack = 1;   // Recolor w's left child as black
                    }
                    w->isBlack = 0;          // Recolor w as red
                    rightRotate(root, w);    // Right rotate on w
                    w = x->parent->right;    // Update w to be the new sibling of x
                }
                // Case 4: w's right child is red
                w->isBlack = x->parent->isBlack; // Make w the same color as x's parent
                x->parent->isBlack = 1;          // Recolor x's parent as black
                if(w->right != NULL){
                    w->right->isBlack = 1;       // Recolor w's right child as black
                }   
                leftRotate(root, x->parent);     // Left rotate on x's parent
                x = *root;                       // Set x to root to end loop
            }
        } else {  // Symmetric case: x is the right child
            warehouseTreeNode *w = x->parent->left;
            if (!w->isBlack) { // Case 1: x's sibling w is red
                w->isBlack = 1;
                x->parent->isBlack = 0;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if ((w->left == NULL || w->left->isBlack == 1) && (w->right == NULL || w->right->isBlack == 1)) { // Case 2: Both of w's children are black
                w->isBlack = 0;
                x = x->parent;
            } else {
                if (w->left == NULL || w->left->isBlack) { // Case 3: w's left child is black
                    if(w->right != NULL){
                        w->right->isBlack = 1;
                    }
                    w->isBlack = 0;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                // Case 4: w's left child is red
                w->isBlack = x->parent->isBlack;
                x->parent->isBlack = 1;
                if(w->left != NULL){
                    w->left->isBlack = 1;
                }
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->isBlack = 1; // Ensure the root node is always black
}

void rebalanceTreeAfterInsertion(warehouseTreeNode **root, warehouseTreeNode *x){
    if(*root == x){
        x->isBlack = 1;
    }
    else{
        warehouseTreeNode *child = x;
        warehouseTreeNode *parent = child->parent;
        warehouseTreeNode *grandpa;
        warehouseTreeNode *uncle;

        while(child != *root && parent->isBlack == 0){
            parent = child->parent;
            grandpa = parent->parent;

            if(parent == grandpa->left){
                uncle = grandpa->right;

                if(uncle != NULL && uncle->isBlack == 0){   // Case 1: Uncle is red
                    parent->isBlack = 1;                    // Recolor parent black
                    uncle->isBlack = 1;                     // Recolor uncle black
                    grandpa->isBlack = 0;                   // Recolor grandparent red

                    child = grandpa;                        //Restart from grandparent
                }
                else{
                    if(child == parent->right){
                        child = parent;
                        parent = child->parent;
                        grandpa = parent->parent;
                        leftRotate(root, child);
                    }
                    parent->isBlack = 1;
                    grandpa->isBlack = 0;
                    rightRotate(root, grandpa);
                }
            }
            else{
                uncle = grandpa->left;
                if(uncle != NULL && uncle->isBlack == 0){
                    parent->isBlack = 1;
                    uncle->isBlack = 1;
                    grandpa->isBlack = 0;

                    child = grandpa;
                }
                else{
                    if(child == parent->left){
                        child = parent;
                        parent = child->parent;
                        grandpa = parent->parent;
                        rightRotate(root, child);
                    }
                    parent->isBlack = 1;
                    grandpa->isBlack = 0;
                    leftRotate(root, grandpa);
                }
            }
        }
        (*root)->isBlack = 1;
    }
}

void leftRotate(warehouseTreeNode **root, warehouseTreeNode *x) {
    warehouseTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(warehouseTreeNode **root, warehouseTreeNode *y) {
    warehouseTreeNode *x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void addIngredientToMap(warehouseMap *map, ingredientLot*s){
    int hash = sdbm_hash(s->name);

    if(map->hashArray[hash] == NULL){
        //If spot is null then create new hashHead and new ingredientHead
        ingredientLotListList *hashHead = malloc(sizeof(*hashHead));
        ingredientLotList *ingredientHead = malloc(sizeof(*ingredientHead));

        hashHead->el = ingredientHead;
        hashHead->next = NULL;

        ingredientHead->el = s;
        ingredientHead->next = NULL;
    }
    else{
        //The hashHead already existed, locate ingredientHead
        int ingredientHeadFound = 0;
        int endOfList = 0;
        ingredientLotListList *hashHead = map->hashArray[hash];
        ingredientLotList *ingredientHead;

        do{
            if(strcmp(hashHead->el->el->name, s->name) == 0){
                ingredientHead = hashHead->el;
                ingredientHeadFound = 1;
            }
            else if(hashHead->next != NULL){
                hashHead = hashHead->next;
            }
            else{
                endOfList = 1;
            }
        }
        while(endOfList == 0 && ingredientHeadFound == 0);

        if(ingredientHeadFound == 0){
            hashHead->next = malloc(sizeof(*hashHead));
            hashHead = hashHead->next;

            ingredientHead = malloc(sizeof(*ingredientHead));

            hashHead->el = ingredientHead;
            hashHead->next = NULL;

            ingredientHead->el = s;
            ingredientHead->next = NULL;
        }
        else{
            //HashHead points to the beginning of the correct ingredientHead
            //Need to locate correct position inside ingredientList
            ingredientLotList *currentIngredientLot = hashHead->el;

            if(currentIngredientLot->el->time == s->time){
                //A lot with same expiration is at the head of the list

                //increment amount in the node
                currentIngredientLot->el->amount += s->amount;

                free(s);
            }
            else if(currentIngredientLot->el->time > s->time){
                //Head of ingredients list is greater than new ingredient lot
                hashHead->el = malloc(sizeof(*currentIngredientLot));
                hashHead->el->el = s;
                hashHead->el->next = currentIngredientLot;
            }
            else{
                //Navigate the ingredient list and stop on the first node where
                //next node expires later than s

                int nodeFound = 0;
                do{
                    if(currentIngredientLot->next == NULL || currentIngredientLot->next->el->time > s->time){
                        nodeFound = 1;
                    }
                    else{
                        currentIngredientLot = currentIngredientLot->next;
                    }
                }
                while(nodeFound == 0);

                if(!(currentIngredientLot->el->time == s->time)){
                    ingredientLotList *newLot = malloc(sizeof(*newLot));
                    newLot->el = s;
                    newLot->next = currentIngredientLot->next;
                    currentIngredientLot->next = newLot;
                }
                else{
                    currentIngredientLot->el->amount += s->amount;
                    free(s);
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

void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        printf("  ");
    }
}

// Recursive function to print the tree
void printRBTree(warehouseTreeNode *node, int level) {
    if (node == NULL) {
        return;
    }

    // Print the right subtree first (higher values on the right)
    printRBTree(node->right, level + 1);

    // Print current node
    printSpaces(level);  // Indent according to the current level
    printf("%s[Expiration: %d]\n", node->isBlack ? "B" : "R", node->expiration);

    // Print the left subtree
    printRBTree(node->left, level + 1);
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