#include "stdio.h"
#include <stdlib.h>
#include <string.h>

#include "convert.h"

Map* parseOsuMap(char* path) {
    Map* map = newMap();

    FILE* fp = fopen(path, "r");
    if(!fp) return NULL;

    int r;
    char line[100];
 
    int notes = 0;
    int maxt = 0;

    while ((r = fscanf(fp, "%s\n", line)) != EOF) {
        if(!strcmp(line,"[HitObjects]")) {
            notes = 1;
            continue;
        }
    
        if(!strcmp(line,"")) {
            notes = 0;
        }

        if(notes) {
            int x = atoi(strtok(line, ","));
            strtok(NULL, ",");
            int time = atoi(strtok(NULL, ","));
            int col;
            
            if(x <= 128) {
                col = 0;
            }
            else if(x <= 256) {
                col = 1;
            }
            else if(x <= 386) {
                col = 2;
            }
            else if(x <= 512) {
                col = 3;
            }

    printf("convert %d\n",col);
            append(map->columns[col], time);

            if(time > maxt) {
                maxt = time;
            }
        }

        map->length = maxt + 1000;
    }

    return map;
}
