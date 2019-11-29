
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

#include "map.h"

Map* newMap() {
    Map* map = (Map*) malloc(sizeof(Map));

    map->columns = (node**) malloc(sizeof(node*) * 4);
    for(int k = 0; k < 4; k++) {
        map->columns[k] = newList(-1,NULL);
    }
    map->audio_path = malloc(sizeof(char)*255);
    strcpy(map->audio_path,"maps/");

    return map;
}

Map* parseMap(char* path) {
    Map* map = newMap();

    FILE* fp = fopen(path, "r");
    if(!fp) return NULL;

    int r;
    char line[100];
    

    ParseState state = UNKNOWN;

    while ((r = fscanf(fp, "%s\n", line)) != EOF) {
        if(!strcmp(line,"")) continue;
        
        if(!strcmp(line,";meta")) {
            state = META;
            continue;
        }
        if(!strcmp(line,";notes")) {
            state = NOTES;
            continue;
        }
        
        char* type;
        int value;
        int time,col;

        switch(state) {
            case META:
                type = strtok(line,":");
                
                if(!strcmp(type,"length")) {
                    map->length = atoi(strtok(NULL,":"));
                }
                
                if(!strcmp(type,"audio")) {
                    char* audio_name = strtok(NULL,":");
                }
                break;
            case NOTES:
                sscanf(line,"%d,%d",&time,&col);
                append(map->columns[col-1], time);
                break;
            default:
                break;
        }
    }

    return map;
}