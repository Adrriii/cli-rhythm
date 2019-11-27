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
    int meta = 0;
    int maxt = 0;

    while ((r = fscanf(fp, "%s\n", line)) != EOF) {
        if(!strcmp(line,"[HitObjects]")) {
            meta = 0;
            notes = 1;
            continue;
        }
        if(!strcmp(line,"[General]")) {
            notes = 0;
            meta = 1;
            continue;
        }
    
        if(!strcmp(line,"")) {
            notes = 0;
            meta = 0;
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

            append(map->columns[col], time);

            if(time > maxt) {
                maxt = time;
            }
            map->length = maxt + 1000;

            continue;
        }

        if(meta) {
            char* type;
            type = strtok(line, ":");

            if(!strcmp(type,"AudioFilename")) {
                map->audio_path = strtok(NULL, ":");

                //if (map->audio_path[0] == ' ') 
                    //memmove(map->audio_path, map->audio_path+1, strlen(map->audio_path));
            }

            //free(type);
        }

    }

    return map;
}
