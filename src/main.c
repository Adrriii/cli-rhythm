#include "stdio.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

/**
 * Sets the variable to millisecond tim.
 */
long timeMs(int *t){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	*t = (currentTime.tv_sec * (int)1e6 + currentTime.tv_usec) / 1000;
}

void clearScreen()
{
  int r = system("clear");
  printf("\e[?25l");
}

void moveCursor(int x, int y) {
    printf("\033[%d;%dH",x,y);
}


void main(const char* argv, const int argc) {
    char* map_path = "maps/map.cry";

    int starttime;
    int lasttime;
    int currenttime;

    GameState* gameState = (GameState*) malloc(sizeof(GameState));

    gameState->lines = 40;
    gameState->keys = 4;
    gameState->notes = (int*) calloc(gameState->keys * gameState->lines, sizeof(int));
    gameState->erase = (int*) calloc(gameState->keys * gameState->lines, sizeof(int));
    
    gameState->map = parseMap(map_path);
    if(gameState->map == NULL) {
        printf("Invalid map \"%s\"\n",map_path);
        exit(0);
    }

    int FPS_CAP = 1000 / 120;
    int ft;

    gameState->scrollspeed = FPS_CAP / 1.5;

    timeMs ( &(gameState->start_time) );
    timeMs ( &lasttime );


    clearScreen();
    drawBoardInit(gameState);
    timeMs ( &currenttime );
    gameState->time = currenttime;
    while(gameState->time - gameState->start_time < gameState->map->length) {
        timeMs ( &currenttime );
        gameState->time = currenttime;

        ft = (currenttime - lasttime);
        update(gameState);
        if(ft >= FPS_CAP) {
            moveCursor(0,0);
            draw(gameState);

            timeMs ( &lasttime );
        }    
    }

    clearScreen();
}


void draw(GameState* gameState) {
    drawBoard(gameState);

    fflush(stdout);
}

void update(GameState* gameState) {  
    for(int i = 0; i < gameState->keys * gameState->lines; i++) {
        gameState->notes[i] = 0;
    }


    for(int k = 0; k < gameState->keys; k++) {
        node* n = gameState->map->columns[k];
        if(n->next == NULL) continue;
        n = n->next;

        while(n != NULL) {
            int line = -timeDistanceToLine(gameState, n->data);
            
            // It  works trust me
            /// i think
            if(line > 0) {
                if(line <= gameState->lines * 2) {

                    if(line % 2 == 0) {
                        line = line / 2;
                        gameState->notes[(k * gameState->lines) + (gameState->lines - line)] = 2;
                    } else {
                        line = line / 2;
                        int val = gameState->notes[(k * gameState->lines) + (gameState->lines - line)];
                        switch(val) {
                            case 0:
                                gameState->notes[(k * gameState->lines) + (gameState->lines - line)] = 1;
                                break;
                            case -1:
                                gameState->notes[(k * gameState->lines) + (gameState->lines - line)] = 2;
                                break;
                        }
                        line++;
                        val = gameState->notes[(k * gameState->lines) + (gameState->lines - line)];
                        if(line >0 && gameState->lines >= line) {
                            switch(val) {
                                case 0:
                                    gameState->notes[(k * gameState->lines) + (gameState->lines - line)] = -1;
                                    break;
                                case 1:
                                    gameState->notes[(k * gameState->lines) + (gameState->lines - line)] = 2;
                                    break;
                            }
                        }
                    }
                } else {
                    break;
                }
                //if(line == 40)
                //printf("%d\n", line);
            }
            n = n->next;
        }
    }
}

void drawBoardInit(GameState* gameState) {
    for(int l = 0; l < gameState->lines; l++) {
        printf("                          ||");
        for(int k = 0; k < gameState->keys; k++) {
            printf("      ");
            if(k != gameState->keys -1) {
                //putchar(' ');
            }
        }
        printf("||                           \n");
    }

    printf("                          ||¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯||                           \n");
    printf("                          ||     |     ||     |     ||                           \n");
    printf("                          ||     |     ||     |     ||                           \n");
    printf("                          ||     |     ||     |     ||                           \n");
    printf("                          ||________________________||                           \n");
 
}

void drawBoard(GameState* gameState) {

    for(int k = 0; k < gameState->keys; k++) {
        for(int l = 0; l < gameState->lines; l++) {
            if(gameState->erase[(k * gameState->lines) + l] == 2) {
                gameState->erase[(k * gameState->lines) + l] = 0;
                int x = l;
                int y = 29 + (k*6);
                moveCursor(x,y);
                printf("      ");
            }
        }
    }

    for(int k = 0; k < gameState->keys; k++) {
        for(int l = 0; l < gameState->lines; l++) {
            int x = l;
            int y = 29 + (k*6);
            moveCursor(x,y);
            switch(gameState->notes[(k * gameState->lines) + l]) {
                case 0:
                    if(gameState->erase[(k * gameState->lines) + l] == 1) {
                        gameState->erase[(k * gameState->lines) + l] = 2;
                    }
                    break;
                case -1:
                    printf("▄▄▄▄▄▄");
                    gameState->erase[(k * gameState->lines) + l] = 1;
                    break;
                case 1:
                    gameState->erase[(k * gameState->lines) + l] = 1;
                    printf("▀▀▀▀▀▀");
                    break;
                case 2:
                    gameState->erase[(k * gameState->lines) + l] = 1;
                    printf("██████");
                    break;
            }
        }
    }
}

Map* parseMap(char* path) {
    Map* map = (Map*) malloc(sizeof(Map));

    map->columns = (node**) malloc(sizeof(node*) * 4);
    for(int k = 0; k < 4; k++) {
        map->columns[k] = newList(-1,NULL);
    }

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

int timeDistanceToLine(GameState* gameState, int time) {
    return ((gameState->time - gameState->start_time) - time ) / (gameState->scrollspeed * 2);
}


//////////////

node* newList(int data,node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;
 
    return new_node;
}

node* append(node* head, int data)
{
    /* go to the last node */
    node* cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
 
    /* create a new node */
    node* new_node =  newList(data,NULL);
    cursor->next = new_node;
    return head;
}

void dispose(node* head)
{
    node* cursor, *tmp;
 
    if(head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while(cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}