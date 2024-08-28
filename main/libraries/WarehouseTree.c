#include "WarehouseTree.h"

warehouseTreeNode NIL_NODE = {0, NULL, 1, NULL, NULL, NULL};
warehouseTreeNode *NIL = &NIL_NODE;





void addIngredientToTree(warehouseTreeNode **d_root, String ingredient, int expiration){
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

        root->right = NIL;
        root->left = NIL;
        root->parent = NIL;
        root->expiration = expiration;
        root->isBlack = 1;
        
        StringList *ingredients = malloc(sizeof(*ingredients));
        strcpy(ingredients->el, ingredient);
        ingredients->next = NULL;
        root->ingredients = ingredients;
    }
    else{
        //The double pointer points to a valid root therefore we need to insert into the tree

        warehouseTreeNode *y = NIL;
        warehouseTreeNode *x = *d_root;
        warehouseTreeNode *z;

        int duplicate = 0;

        while(x != NIL){
            y = x;
            if(expiration < x->expiration){
                x = x->left;
            }
            else if(expiration > x->expiration){
                x = x->right;
            }
            else if(expiration == x->expiration){
                //Node already exists
                duplicate = 1;
                StringList *ingredients = x->ingredients;

                int breaker = 0;
                while(breaker == 0){
                    if(strcmp(ingredients->el, ingredient) == 0){
                        breaker = 1;
                    }
                    else if(ingredients->next == NULL){
                        StringList *node = malloc(sizeof(*node));
                        strcpy(node->el, ingredient);
                        node->next = NULL;
                        ingredients->next = node;

                        breaker = 1;
                    }
                    else{
                        ingredients = ingredients->next;
                    }
                }

                break;
            }

        }

        if(duplicate == 0){
            z = malloc(sizeof(*z));
            z->expiration = expiration;
            z->ingredients = malloc(sizeof(*(z->ingredients)));
            z->ingredients->next = NULL;
            strcpy(z->ingredients->el, ingredient);

            z->parent = y;
            if(y == NIL){
                *d_root = z;
            }

            else if(z->expiration < y->expiration){
                y->left = z;
            }
            else{
                y->right = z;
            }

            z->left = NIL;
            z->right = NIL;
            z->isBlack = 0;

            rebalanceTreeAfterInsertion(d_root, z);
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
    else if(strcmp(ingredients->el, ingredient) == 0){
        //If the ingredient is the first in the node but it's not the only one
        node->ingredients = node->ingredients->next;
        free(ingredients);
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
    warehouseTreeNode *z = node;
    warehouseTreeNode *y = node;
    warehouseTreeNode *x;
    int yOrignialColor = y->isBlack;

    if(z->left == NIL){
        x = z->right;
        treeTransplant(root, z, z->right);
    }
    else if(z->right == NIL){
        x = z->left;
        treeTransplant(root, z, z->left);
    }
    else{
        //finding smallest successor of z
        y = z->right;
        while(y->left != NIL){
            y = y->left;
        }


        yOrignialColor = y->isBlack;
        x = y->right;

        if(y->parent == z){
            x->parent = y;
        }
        else{
            treeTransplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        treeTransplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->isBlack = z->isBlack;
    }
    if(yOrignialColor == 1){
        rebalanceTreeAfterDelete(root, x);
    }
}

void rebalanceTreeAfterDelete(warehouseTreeNode **root, warehouseTreeNode *x){
    // While x is not the root and x is black
    while (x != *root && x->isBlack == 1) {
        if (x == x->parent->left) {                                         // x is the left child
            warehouseTreeNode *w = x->parent->right;                        // w is x's sibling
            if (w->isBlack == 0) {                                          // Case 1: x's sibling w is red
                w->isBlack = 1;                                             // Recolor w as black
                x->parent->isBlack = 0;                                     // Recolor x's parent as red
                leftRotate(root, x->parent);                             // Left rotate on x's parent
                w = x->parent->right;                                       // Update w to be the new sibling of x
            }
            if (w->left->isBlack == 1 && w->right->isBlack == 1) {          // Case 2: Both of w's children are black
                w->isBlack = 0;                                             // Recolor w as red
                x = x->parent;                                              // Move x up the tree
            }
            else {
                if (w->right->isBlack == 1) {                               // Case 3: w's right child is black
                    w->left->isBlack = 1;                                   // Recolor w's left child as black
                    w->isBlack = 0;                                         // Recolor w as red
                    rightRotate(root, w);                                // Right rotate on w
                    w = x->parent->right;                                   // Update w to be the new sibling of x
                }
                                                                            // Case 4: w's right child is red
                w->isBlack = x->parent->isBlack;                            // Make w the same color as x's parent
                x->parent->isBlack = 1;                                     // Recolor x's parent as black
                w->right->isBlack = 1;                                      // Recolor w's right child as black 
                leftRotate(root, x->parent);                             // Left rotate on x's parent
                x = *root;                                                  // Set x to root to end loop
            }
        } else {                                                            // Symmetric case: x is the right child
            warehouseTreeNode *w = x->parent->left;
            if (w->isBlack == 0) {                                          // Case 1: x's sibling w is red
                w->isBlack = 1;
                x->parent->isBlack = 0;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->left->isBlack == 1 && w->right->isBlack == 1) {          // Case 2: Both of w's children are black
                w->isBlack = 0;
                x = x->parent;
            }
            else {
                if (w->left->isBlack == 1) {                                // Case 3: w's left child is black
                    w->right->isBlack = 1;
                    w->isBlack = 0;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                // Case 4: w's left child is red
                w->isBlack = x->parent->isBlack;
                x->parent->isBlack = 1;
                w->left->isBlack = 1;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->isBlack = 1; // Ensure the root node is always black
}

void rebalanceTreeAfterInsertion(warehouseTreeNode **root, warehouseTreeNode *z){
    warehouseTreeNode *y;

    while(z->parent->isBlack == 0){
        if(z->parent == z->parent->parent->left){
            y = z->parent->parent->right;

            if(y->isBlack == 0){
                z->parent->isBlack = 1;
                y->isBlack = 1;
                z->parent->parent->isBlack = 0;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->right){
                    z = z->parent;
                    leftRotate(root, z);
                }

                z->parent->isBlack = 1;
                z->parent->parent->isBlack = 0;
                rightRotate(root, z->parent->parent);
            }
        }
        else{
            y = z->parent->parent->left;

            if(y->isBlack == 0){
                z->parent->isBlack = 1;
                y->isBlack = 1;
                z->parent->parent->isBlack = 0;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->left){
                    z = z->parent;
                    rightRotate(root, z);
                }

                z->parent->isBlack = 1;
                z->parent->parent->isBlack = 0;
                leftRotate(root, z->parent->parent);
            }
        }
        (*root)->isBlack = 1;
    }
}

void leftRotate(warehouseTreeNode **root, warehouseTreeNode *x) {
    warehouseTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL) {
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
    if (x->right != NIL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NIL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void treeTransplant(warehouseTreeNode **root, warehouseTreeNode *u, warehouseTreeNode *v){
    if(u->parent == NIL){
        *root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    v->parent = u->parent;
}





// Recursive function to print the tree
void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        printf("  ");
    }
}

void printRBTree(warehouseTreeNode *node, int level) {
    if (node == NULL || node == NIL) {
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