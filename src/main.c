#include "stdio.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "convert.h"

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
    //gameplay(parseOsuMap("maps/map.cry"));
    gameplay(parseOsuMap("E:/osu!/Songs/beatmap-637088221942779431-song/Unknown Artist - .= (Adri) [!].osu"));
}

void gameplay(Map* map) {
    int starttime;
    int lasttime;
    int currenttime;

    GameState* gameState = (GameState*) malloc(sizeof(GameState));

    gameState->lines = 40;
    gameState->keys = 4;
    gameState->notes = (int*) calloc(gameState->keys * gameState->lines, sizeof(int));
    gameState->erase = (int*) calloc(gameState->keys * gameState->lines, sizeof(int));
    
    gameState->map = map;
    if(gameState->map == NULL) {
        printf("Invalid map\n");
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
        // start at 1 because weird but lazy to fix
        for(int l = 1; l < gameState->lines; l++) {
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

int timeDistanceToLine(GameState* gameState, int time) {
    return ((gameState->time - gameState->start_time) - time ) / (gameState->scrollspeed * 2);
}