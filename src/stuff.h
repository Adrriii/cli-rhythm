#ifndef STUFF
#define STUFF

typedef struct node {
    int data;
    struct node* next;
} node; 

node* newList(int data,node* next);
node* append(node* head, int data);
void dispose(node* head);

#endif