#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory.h"
#include "memory_manager.h"
#define NUM_BLOCKS 100

int main()
{
    char disk[NUM_BLOCKS][BLOCK_SIZE]={0};
    FreeBlock *freeHead = NULL, *freeTail = NULL;
    initializeList(&freeHead, &freeTail, NUM_BLOCKS);
    FileNode *root = malloc(sizeof(FileNode));
    if (root == NULL) {
        printf("Memory allocation for root directory failed\n");
        return 1;
    }
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->children = NULL;
    root->next = NULL;

    initializeFileData(root);
    FileNode *cwd = root;
    char command[512];
    while (1){
        printf("VFS>");
        printWorkingDirectory(cwd);
        printf("> ");

        if (fgets(command,sizeof(command),stdin)==NULL){
            break;
        }
        command[strcspn(command,"\n")]=0; // Remove newline character
        char *operation=strtok(command," ");
        char *argument=strtok(NULL,"");
        if (operation==NULL){
            continue;   
        }
        if (strcmp(operation, "ls")==0){
            listChildren(cwd);
        }
        else if (strcmp(operation, "mkdir")==0){
            if (argument==NULL){
                printf("Usage: mkdir <directory_name>\n");
                continue;
            }
            FileNode *newDir=makeDirectory(argument, cwd);
            if (newDir!=NULL){
                printf("Directory '%s' created\n", argument);
            }
        }
        else if (strcmp(operation, "rmdir")==0){
            if (argument==NULL){
                printf("Usage: rmdir <directory_name>\n");
                continue;
            }
            int result=removeDirectory(cwd, argument);
            if (result){
                printf("Directory '%s' removed\n", argument);
            }
        }
        else if (strcmp(operation, "cd")==0){
            if (argument==NULL){
                printf("Usage: cd <directory_name>\n");
                continue;
            }
            FileNode *newCwd=changeDirectory(cwd, root, argument);
            if (newCwd!=cwd && newCwd!=NULL){
                cwd=newCwd;
                printf("Changed directory to '%s'\n", cwd->name);
            }
        }
        else if(strcmp(operation, "create")==0){
            if (argument==NULL){
                printf("Usage: create <file_name>\n");
                continue;
            }
            FileNode *newFile=createFile(argument, cwd);
            if (newFile!=NULL){
                insertChild(cwd, newFile);
                printf("File '%s' created\n", argument);
            }
        }
        else if(strcmp(operation, "write")==0){
            if (argument==NULL){
                printf("Usage: write <file_name> <content>\n");
                continue;
            }
            char *separator=strchr(argument,' ');
            if (separator==NULL){
                printf("Usage: write <file_name> <content>\n");
                continue;
            }
            *separator='\0';
            char *fileName=argument;
            char *content=separator+1;
            if (*content=='\0'){
                printf("Content cannot be empty\n");
                continue;
            }
            FileNode *file=searchChild(cwd, fileName);
            if (file==NULL || file->isDirectory){
                printf("File '%s' not found\n", fileName);
                continue;
            }
            int result=writeFile(file, content, disk, &freeHead, &freeTail);
            if (result){
                printf("Content written to file '%s'\n", fileName);
            }
            else{
                printf("Failed to write content to file '%s'\n", fileName);
            }
        }
        else if(strcmp(operation, "read")==0){
            if (argument==NULL){
                printf("Usage: read <file_name>\n");
                continue;
            }
            FileNode *file=searchChild(cwd, argument);
            if (file==NULL || file->isDirectory){
                printf("File '%s' not found\n", argument);
                continue;
            }
            char *content=readFile(file, disk);
            if (content!=NULL){
                printf("Content of file '%s': %s\n", argument, content);
                free(content);
            }
        }
        else if( strcmp(operation, "delete")==0){
            if (argument==NULL){
                printf("Usage: delete <file_name>\n");
                continue;
            }
            FileNode *file=searchChild(cwd, argument);
            if (file==NULL || file->isDirectory){
                printf("File '%s' not found\n", argument);
                continue;
            }
            deleteFile(file, &freeHead, &freeTail);
            printf("File '%s' deleted\n", argument);
        }
        else if (strcmp(operation, "df")==0){
            displayDiskUsage(freeHead, NUM_BLOCKS);
        }
        else if (strcmp(operation, "pwd")==0){
            printWorkingDirectory(cwd);
        }
        else if (strcmp(operation, "exit")==0){
            printf("Exiting Virtual File System...\n");
            free(root);
            free(freeHead);
            free(freeTail);
            break;
        }
        else{
            printf("Unknown command: %s\n", operation);
        }
    }
    freeFileTree(root);
    free(freeHead);
    free(freeTail);
    return 0;
}

