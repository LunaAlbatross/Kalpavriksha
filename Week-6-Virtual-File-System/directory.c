#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memory_manager.h"
#include "directory.h"

void insertChild(FileNode *parent, FileNode *newNode){
    if (parent->children == NULL){
        parent->children = newNode;
    }
    else{
        FileNode *temp = parent->children;
        while (temp->next != parent->children){
            temp = temp->next;
        }
        temp->next = newNode;
    }
    newNode->next = parent->children;
}

FileNode* searchChild(FileNode *parent, char *name){
    if (parent->children == NULL){
        return NULL;
    }
    FileNode *temp = parent->children;
    do {
        if (strcmp(temp->name, name) == 0){
            return temp;
        }
        temp = temp->next;
    } while (temp != parent->children);
    return NULL;
}
void listChildren(FileNode *parent){
    if (parent->children == NULL){
        printf("No children\n");
        return;
    }
    FileNode *temp = parent->children;
    do {
        if (temp->isDirectory){
            printf("%s/\n", temp->name);
        }
        else{
            printf("%s\n", temp->name);
        }
        temp = temp->next;
    } while (temp != parent->children);
}

int removeChild(FileNode *parent, char *name){
    if (parent->children==NULL){
        return 0;
    }
    FileNode *prev=parent->children;
    while (prev->next!=parent->children && strcmp(prev->next->name,name)!=0){
        prev=prev->next;
    }
    FileNode *toDelete=prev->next;
    if (strcmp(toDelete->name, name) != 0)
    {
        return 0;
    }
    if (toDelete->isDirectory && toDelete->children != NULL){
        printf("Cannot remove directory '%s' because it is not empty\n", name);
        return 0;
    }
    if (toDelete->next == toDelete){
        // Only child
        parent->children = NULL;
    }
    else if (toDelete == parent->children){
        // First child
        parent->children = toDelete->next;
        prev->next = parent->children;
    }
    else{
        // Middle or last child
        prev->next = toDelete->next;
    }
    free(toDelete);
    return 1;
}

void initializeFileData(FileNode *file) {
    if (file==NULL) return;
    file->fileSize=0;
    file->blockCount=0;
    file->blockPointers=NULL;
}

FileNode* createFile(char *name, FileNode *parent){
    if (parent==NULL || !parent->isDirectory){
        printf("Invalid parent directory\n");
        return NULL;
    }
    if (name==NULL || strlen(name)==0 || strlen(name)>50){
        printf("Invalid file name\n");
        return NULL;
    }
    if (searchChild(parent, name) != NULL){
        printf("File or directory with name '%s' already exists in '%s'\n", name, parent->name);
        return NULL;
    }
    FileNode *newFile=malloc(sizeof(FileNode));
    if (newFile==NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }
    strcpy(newFile->name,name);
    newFile->isDirectory=0;
    newFile->parent=parent;
    newFile->children=NULL;
    newFile->next=NULL;
    initializeFileData(newFile);
    return newFile;
}

int writeFile(FileNode *file, char *content, char disk[][BLOCK_SIZE], FreeBlock **freehead, FreeBlock **freetail){
    if (file==NULL || file->isDirectory){
        printf("Invalid file\n");
        return 0;
    }
    if (file->blockPointers!=NULL){
        return 0; // File already has content
    }
    if (content==NULL){
        printf("Content is NULL\n");
        return 0;
    }
    int contentLength=strlen(content);
    file->fileSize=contentLength+1; // +1 for null terminator
    file->blockCount=(file->fileSize+BLOCK_SIZE-1)/BLOCK_SIZE; // Calculate number of blocks needed
    file->blockPointers=malloc(file->blockCount*sizeof(int));
    if (file->blockPointers==NULL){
        printf("Memory allocation for block pointers failed\n");
        return 0;
    }
    int offset=0;
    int remaining=file->fileSize;
    for (int i=0;i<file->blockCount;i++){
        int block=allocateBlock(freehead, freetail);
        if (block==-1){
            printf("Failed to allocate block for file '%s'\n", file->name);
            for (int j=0;j<i;j++){
                returnBlock(freehead, freetail, file->blockPointers[j]);
            }
            free(file->blockPointers);  
            file->blockCount=0;
            file->fileSize=0;
            file->blockPointers=NULL;

            return 0;
        }
        file->blockPointers[i]=block;
    }
    for (int i=0;i<file->blockCount;i++){
        int bytesToWrite=(remaining<BLOCK_SIZE)?remaining:BLOCK_SIZE;
        memcpy(disk[file->blockPointers[i]], content+offset, bytesToWrite);
        offset+=bytesToWrite;
        remaining-=bytesToWrite;
    }
    return 1;
}

char* readFile(FileNode *file, char disk[][BLOCK_SIZE]){
    if (file==NULL || file->isDirectory){
        printf("Invalid file\n");
        return NULL;
    }
    if (file->blockPointers==NULL || file->blockCount==0){
        printf("File '%s' is empty\n", file->name);
        return NULL;
    }
    char *content=malloc(file->fileSize);
    if (content==NULL){
        printf("Memory allocation for content failed\n");
        return NULL;
    }
    int offset=0;
    int remaining=file->fileSize;
    for (int i=0;i<file->blockCount;i++){
        int bytesToRead=(remaining<BLOCK_SIZE)?remaining:BLOCK_SIZE;
        memcpy(content+offset, disk[file->blockPointers[i]], bytesToRead);
        offset+=bytesToRead;
        remaining-=bytesToRead;
    }
    content[file->fileSize-1]='\0'; // Ensure null termination
    return content;
}

void deleteFile(FileNode *file, FreeBlock **freehead, FreeBlock **freetail){
    if (file==NULL || file->isDirectory){
        printf("Invalid file\n");
        return;
    }
    if (file->blockPointers!=NULL){
        for (int i=0;i<file->blockCount;i++){
            returnBlock(freehead, freetail, file->blockPointers[i]);
        }
        free(file->blockPointers);
        file->blockPointers=NULL;
        file->blockCount=0;
        file->fileSize=0;
    }
    if (file->parent!=NULL){
        removeChild(file->parent, file->name);
    }
}

FileNode* makeDirectory(char *name, FileNode *parent){
    if (parent==NULL || !parent->isDirectory){
        printf("Invalid parent directory\n");
        return NULL;
    }
    if (name==NULL || strlen(name)==0 || strlen(name)>50){
        printf("Invalid directory name\n");
        return NULL;
    }
    if (searchChild(parent, name) != NULL){
        printf("File or directory with name '%s' already exists in '%s'\n", name, parent->name);
        return NULL;
    }
    FileNode *newDir=malloc(sizeof(FileNode));
    if (newDir==NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }
    strcpy(newDir->name,name);
    newDir->isDirectory=1;
    newDir->parent=parent;
    newDir->children=NULL;
    newDir->next=NULL;
    initializeFileData(newDir);
    insertChild(parent, newDir);
    return newDir;
}

int removeDirectory(FileNode *parent, char *name){
    if (parent==NULL || !parent->isDirectory || name==NULL){
        printf("Invalid parent directory\n");
        return 0;
    }
    FileNode *dir=searchChild(parent, name);
    if (dir==NULL || !dir->isDirectory){
        printf("Directory '%s' not found in '%s'\n", name, parent->name);
        return 0;
    }
    if (dir->children!=NULL){
        printf("Cannot remove directory '%s' because it is not empty\n", name);
        return 0;
    }
    return removeChild(parent, name);
}

FileNode* changeDirectory(FileNode *cwd, FileNode *root, char *name){
    if (cwd==NULL || !cwd->isDirectory || name==NULL){
        printf("Invalid current directory\n");
        return NULL;
    }
    if (strcmp(name, "..")==0){
        if (cwd->parent!=NULL){
            return cwd->parent;
        }
        else{
            printf("Already at root directory\n");
            return cwd;
        }
    }
    if (strcmp(name,"/")==0){
        return root;
    }
    FileNode *dir=searchChild(cwd, name);
    if (dir==NULL || !dir->isDirectory){
        printf("Directory '%s' not found in '%s'\n", name, cwd->name);
        return cwd;
    }
    return dir;
}
void printWorkingDirectory(FileNode *cwd){
    if (cwd==NULL){
        printf("Invalid current directory\n");
        return;
    }
    if (cwd->parent==NULL){
        printf("/\n");
        return;
    }
    FileNode *directories[100];
    int count = 0;
    FileNode *temp = cwd;
    while (count < 100 && temp->parent != NULL && temp != NULL) {
        directories[count] = temp;// Store the directory in the array
        count++;
        temp = temp->parent;// Move to the parent directory
    }
    if (temp==NULL || temp->parent != NULL) {
        printf("Invalid or corrupted directory structure\n");
        return;
    }
    printf("/");
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", directories[i]->name);// Print the directory name
        if (i > 0) {
            printf("/");
        }
    }
    printf("\n");
}

void displayDiskUsage(FreeBlock *freeHead, int totalBlocks){
    int freeCount=0;
    FreeBlock *temp=freeHead;
    while (temp!=NULL){
        freeCount++;
        temp=temp->next;
    }
    if (totalBlocks < 0 || freeCount > totalBlocks) {
        printf("Invalid disk information\n");
        return;
    }
    int usedCount=totalBlocks-freeCount;
    printf("Total Blocks: %d\n", totalBlocks);
    printf("Used Blocks: %d\n", usedCount);
    printf("Free Blocks: %d\n", freeCount);
}

void freeFileTree(FileNode *node){
    if (node==NULL){
        return;
    }
    if (node->isDirectory && node->children != NULL) {
        FileNode *last = node->children;
        while (last->next != node->children) {
            last = last->next;
        }
        last->next = NULL;
        FileNode *child = node->children;
        while (child != NULL) {
            FileNode *nextChild = child->next;
            freeFileTree(child);
            child = nextChild;
        }
    }
    if (!node->isDirectory && node->blockPointers!=NULL){
        free(node->blockPointers);
    }
    free(node);
}