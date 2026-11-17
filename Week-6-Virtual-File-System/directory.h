#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "memory_manager.h"

#define BLOCK_SIZE 512

typedef struct FileNode{
    char name[51];
    int isDirectory;
    struct FileNode *parent;
    struct FileNode *children;
    struct FileNode *next;
    int fileSize;
    int blockCount;
    int *blockPointers;
} FileNode;

void insertChild(FileNode *parent, FileNode *newNode);
FileNode* searchChild(FileNode *parent, char *name);
void listChildren(FileNode *parent);
int removeChild(FileNode *parent, char *name);
void initializeFileData(FileNode *file);
FileNode* createFile(char *name, FileNode *parent);
int writeFile(FileNode *file, char *content, char disk[][BLOCK_SIZE], FreeBlock **freehead, FreeBlock **freetail);
char* readFile(FileNode *file, char disk[][BLOCK_SIZE]);
void deleteFile(FileNode *file, FreeBlock **freehead, FreeBlock **freetail);
FileNode* makeDirectory(char *name, FileNode *parent);
int removeDirectory(FileNode *parent, char *name);
FileNode* changeDirectory(FileNode *cwd, FileNode *root, char *name);
void printWorkingDirectory(FileNode *cwd);
void displayDiskUsage(FreeBlock *freeHead, int totalBlocks);
void freeFileTree(FileNode *node);
#endif