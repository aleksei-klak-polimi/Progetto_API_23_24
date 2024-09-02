#include "Recipies.h"
#include "Orders.h"

struct Courier{
    int                                 frequency;
    int                                 capacity;
    struct numberedTimedLinkedItemList  *ordersHead;
};

typedef struct Courier                          Courier;

int             setupCourier(Courier *c);
void            loadCourier(Courier *courier, recipiesMap *book, orderedItemQueue *ordersReady);
void            printCourierContents(Courier *courier);
void            clearCourierOrdersMemory(Courier *courier);