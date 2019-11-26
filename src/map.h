#ifndef MAP
#define MAP

#include "stuff.h"

typedef enum {
    UNKNOWN,
    META,
    NOTES,
} ParseState;

typedef struct {
    int length;
    node** columns;
} Map;

Map* newMap();
Map* parseMap(char* path);
void saveMap(char* dest);

#endif