#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

typedef struct FreeBlock {
    int blockIndex;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

FreeBlock* createNode(int index);
void initializeList(FreeBlock **head, FreeBlock **tail, int count);
int allocateBlock(FreeBlock **head, FreeBlock **tail);
void returnBlock(FreeBlock **head, FreeBlock **tail, int index);
void displayList(FreeBlock *head);

#endif