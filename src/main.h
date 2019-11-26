#include "stuff.h"
#include "map.h"

typedef struct  {
    int lines;
    int keys;

    int start_time;
    int time;
    int scrollspeed;
    Map* map;
    int* notes;
    int* erase;
} GameState;

void gameplay(Map* map);

void draw(GameState* gameState);
void update(GameState* gameState);
void drawBoardInit(GameState* gameState);
void drawBoard(GameState* gameState);
int timeDistanceToLine(GameState* gameState, int time);