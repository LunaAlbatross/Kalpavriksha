#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

FreeBlock* createNode(int index)
{
    FreeBlock *newNode = malloc(sizeof(FreeBlock));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    newNode->blockIndex = index;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}


void initializeList(FreeBlock **head, FreeBlock **tail, int count)
{
    for (int i = 0; i < count; i++)
    {
        FreeBlock *newNode = createNode(i);
        if (newNode == NULL){
            printf("Failed to create node for block index %d\n", i);
            return;
        }
        if (*head == NULL)
        {
            *head = newNode;
            *tail = newNode;
        }
        else
        {
            (*tail)->next = newNode;
            newNode->prev = *tail;
            *tail = newNode;
        }
    }
}

void displayList(FreeBlock *head){
    FreeBlock *temp = head;
    while(temp!=NULL){
        printf("Block Index: %d\n", temp->blockIndex);
        temp = temp->next;
    }
}

int allocateBlock(FreeBlock **head, FreeBlock **tail)
{
    if (*head == NULL)
    {
        printf("No free blocks available\n");
        return -1;
    }
    FreeBlock *temp=*head;
    int allocatedBlock=temp->blockIndex;
    *head=(*head)->next;
    if (*head!=NULL){
        (*head)->prev=NULL;
    }
    else{
        *tail=NULL;
    }
    free(temp);
    return allocatedBlock;
}

void returnBlock(FreeBlock **head, FreeBlock **tail, int index)
{
    FreeBlock *newNode = createNode(index);
    if (newNode == NULL)
        return;

    if (*head == NULL)
    {
        *head = newNode;
        *tail = newNode;
    }
    else
    {
        (*tail)->next = newNode;
        newNode->prev = *tail;

        *tail = newNode;
    }
    printf("Block %d returned successfully\n", index);
}
