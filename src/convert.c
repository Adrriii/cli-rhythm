#include "stdio.h"
#include <stdlib.h>
#include <string.h>

#include "convert.h"

Map* parseOsuMap(char* folder,char* file) {
    Map* map = newMap();

    char* file_path = malloc(sizeof(char) * 255);
    strcpy(file_path, folder);
    map->audio_path = malloc(sizeof(char) * 255);
    strcpy(map->audio_path, folder);
    strcat(file_path, file);
    FILE* fp = fopen(file_path, "r");
    if(!fp) return NULL;

    int r;
    char line[255];

 
    int notes = 0;
    int meta = 0;
    int maxt = 0;

    while(fgets(line, 255, (FILE*) fp)) {
        line[strlen(line) -1] = '\0';
        
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
                char* audio_name = strtok(NULL, ":");

                if(audio_name == NULL) {
                    map->audio_path = "None";
                    continue;
                }
                
                if (audio_name[0] == ' ') {
                    memmove(audio_name, audio_name+1, strlen(audio_name));
                }

                strcat(map->audio_path, audio_name);
            }
        }

    }

    return map;
}
