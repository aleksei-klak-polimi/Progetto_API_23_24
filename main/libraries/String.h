#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED


#define STRINGSIZE 256

typedef char String[STRINGSIZE];

typedef struct StringList{
    String              el;
    struct StringList   *next;
}StringList;

#endif