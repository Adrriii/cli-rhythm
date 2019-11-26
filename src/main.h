
typedef struct node {
    int data;
    struct node* next;
} node; 

node* newList(int data,node* next);
node* append(node* head, int data);
void dispose(node* head);

typedef struct {
    int length;
    node** columns;
} Map;

typedef enum {
    UNKNOWN,
    META,
    NOTES,
} ParseState;

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

void draw(GameState* gameState);
void update(GameState* gameState);
void drawBoardInit(GameState* gameState);
void drawBoard(GameState* gameState);
int timeDistanceToLine(GameState* gameState, int time);
Map* parseMap(char* path);