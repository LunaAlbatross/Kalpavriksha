#include <stdio.h>
#include <stdlib.h>

#include "headers.h"

static struct Block makeBlock(int start, int size, int id, int isAllocated)
{
    struct Block block;
    block.start = start;
    block.size = size;
    block.id = id;
    block.isAllocated = isAllocated;
    return block;
}

int initializeMemory(struct MemoryManager *manager, int size)
{
    if (size < 1 || size > MAX_POOL_SIZE) {
        printf("Invalid pool size. Enter a value from 1 to %d bytes.\n",
               MAX_POOL_SIZE);
        return 0;
    }
    manager->poolSize = size;
    manager->memoryPool = malloc(size * sizeof(unsigned char));
    if (manager->memoryPool == NULL) {
        printf("Could not create the memory pool.\n");
        return 0;
    }
    manager->capacity = 1;
    manager->blocks = malloc(manager->capacity * sizeof(struct Block));
    if (manager->blocks == NULL) {
        printf("Could not create the block list.\n");
        free(manager->memoryPool);
        manager->memoryPool = NULL;
        return 0;
    }
    manager->blockCount = 1;
    manager->blocks[0] = makeBlock(0, size, 0, 0);
    manager->nextID = 1;
    manager->strategy = FIRST_FIT;
    manager->fragmentedRequest = 0;
    manager->freeBytesWhenFragmented = 0;
    return 1;
}

void freeMemory(struct MemoryManager *manager)
{
    free(manager->blocks);
    free(manager->memoryPool);
    manager->blocks = NULL;
    manager->memoryPool = NULL;
}

void displayMemoryMap(const struct MemoryManager *manager)
{
    int i;
    printf("Memory Map (%d bytes):\n", manager->poolSize);
    for (i = 0; i < manager->blockCount; i++) {
        if (manager->blocks[i].isAllocated == 1) {
            printf("[Allocated #%d: %d bytes, Start: %d]\n",
                   manager->blocks[i].id, manager->blocks[i].size,
                   manager->blocks[i].start);
        } else {
            printf("[Free: %d bytes, Start: %d]\n",
                   manager->blocks[i].size, manager->blocks[i].start);
        }
    }
}

void displayStatistics(const struct MemoryManager *manager)
{
    int i;
    int allocatedBytes = 0;
    int freeBytes = 0;
    int allocatedBlocks = 0;
    int freeBlocks = 0;
    int largestFreeBlock = 0;

    for (i = 0; i < manager->blockCount; i++) {
        if (manager->blocks[i].isAllocated == 1) {
            allocatedBytes += manager->blocks[i].size;
            allocatedBlocks++;
        } else {
            freeBytes += manager->blocks[i].size;
            freeBlocks++;
            if (manager->blocks[i].size > largestFreeBlock) {
                largestFreeBlock = manager->blocks[i].size;
            }
        }
    }

    printf("Total memory: %d bytes\n", manager->poolSize);
    printf("Allocated: %d bytes\n", allocatedBytes);
    printf("Free: %d bytes\n", freeBytes);
    printf("Allocated blocks: %d\n", allocatedBlocks);
    printf("Free blocks: %d\n", freeBlocks);
    printf("Largest free block: %d bytes\n", largestFreeBlock);

    if (manager->fragmentedRequest > 0) {
        printf("Note: The request for %d bytes failed even though %d bytes "
               "are free, because the free memory is fragmented.\n",
               manager->fragmentedRequest,
               manager->freeBytesWhenFragmented);
    }
}
