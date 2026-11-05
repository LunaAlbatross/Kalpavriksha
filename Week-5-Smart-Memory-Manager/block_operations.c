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

static int growBlockArray(struct MemoryManager *manager)
{
    int newCapacity = manager->capacity * 2;
    struct Block *newBlocks;
    newBlocks = realloc(manager->blocks,
                        newCapacity * sizeof(struct Block));
    if (newBlocks == NULL) {
        printf("Could not grow the block list.\n");
        return 0;
    }
    manager->blocks = newBlocks;
    manager->capacity = newCapacity;
    return 1;
}

static int makeRoomForBlock(struct MemoryManager *manager)
{
    if (manager->blockCount < manager->capacity) {
        return 1;
    }
    return growBlockArray(manager);
}

static void insertBlock(struct MemoryManager *manager, int index, struct Block newBlock)
{
    int i;
    for (i = manager->blockCount;i>index; i--) {
        manager->blocks[i] = manager->blocks[i - 1];
    }
    manager->blocks[index] = newBlock;
    manager->blockCount++;
}

static void removeBlock(struct MemoryManager *manager, int index)
{
    int i;
    for (i=index; i<manager->blockCount - 1; i++) {
        manager->blocks[i] = manager->blocks[i + 1];
    }
    manager->blockCount--;
}

static void markBlockFree(struct MemoryManager *manager, int index)
{
    manager->blocks[index].id = 0;
    manager->blocks[index].isAllocated = 0;
}

static void mergeFreeBlocks(struct MemoryManager *manager)
{
    int i = 0;
    while (i < manager->blockCount - 1) {
        if (manager->blocks[i].isAllocated == 0 &&
            manager->blocks[i + 1].isAllocated == 0) {
            manager->blocks[i].size += manager->blocks[i + 1].size;
            removeBlock(manager, i + 1);
        } else {
            i++;
        }
    }
}

//Find a free block using the selected allocation strategy.
static int firstFit(const struct MemoryManager *manager, int size)
{
    int i;
    for (i = 0; i < manager->blockCount; i++) {
        if (manager->blocks[i].isAllocated == 0 &&
            manager->blocks[i].size >= size) {
            return i;
        }
    }
    return -1;
}

static int bestFit(const struct MemoryManager *manager, int size)
{
    int i;
    int best = -1;
    for (i = 0; i < manager->blockCount; i++) {
        if (manager->blocks[i].isAllocated == 0 &&
            manager->blocks[i].size >= size &&
            (best == -1 ||
             manager->blocks[i].size < manager->blocks[best].size)) {
            best = i;
        }
    }
    return best;
}

static int worstFit(const struct MemoryManager *manager, int size)
{
    int i;
    int worst = -1;
    for (i = 0; i < manager->blockCount; i++) {
        if (manager->blocks[i].isAllocated == 0 &&
            manager->blocks[i].size >= size &&
            (worst == -1 ||
             manager->blocks[i].size > manager->blocks[worst].size)) {
            worst = i;
        }
    }
    return worst;
}

static int findFreeBlock(const struct MemoryManager *manager, int size)
{
    if (manager->strategy == FIRST_FIT) {
        return firstFit(manager, size);
    } else if (manager->strategy == BEST_FIT) {
        return bestFit(manager, size);
    }
    return worstFit(manager, size);
}

static int findAllocation(const struct MemoryManager *manager, int id)
{
    int i;
    for (i = 0; i < manager->blockCount; i++) {
        if (manager->blocks[i].isAllocated == 1 &&
            manager->blocks[i].id == id) {
            return i;
        }
    }
    return -1;
}

static int allocateInFreeBlock(struct MemoryManager *manager, int index, int size, int id)
{
    int remainingSize = manager->blocks[index].size - size;
    int start = manager->blocks[index].start;
    struct Block freeBlock;
    if (remainingSize > 0 && !makeRoomForBlock(manager)) {
        return 0;
    }
    manager->blocks[index] = makeBlock(start, size, id, 1);
    if (remainingSize > 0) {
        freeBlock = makeBlock(start + size, remainingSize, 0, 0);
        insertBlock(manager, index + 1, freeBlock);
    }
    return 1;
}

static void showAllocationFailure(struct MemoryManager *manager, int size)
{
    int i;
    int totalFree = 0;
    printf("Allocation failed: no free block is large enough.\n");
    for (i = 0; i < manager->blockCount; i++) {
        if (manager->blocks[i].isAllocated == 0) {
            totalFree += manager->blocks[i].size;
        }
    }

    if (totalFree >= size) {
        manager->fragmentedRequest = size;
        manager->freeBytesWhenFragmented = totalFree;
        printf("External fragmentation: free blocks have sizes ");
        for (i = 0; i < manager->blockCount; i++) {
            if (manager->blocks[i].isAllocated == 0) {
                printf("%d ", manager->blocks[i].size);
            }
        }
        printf("(total free: %d bytes).\n", totalFree);
    } else {
        manager->fragmentedRequest = 0;
        manager->freeBytesWhenFragmented = 0;
    }
}

int setStrategy(struct MemoryManager *manager, int newStrategy)
{
    if (newStrategy < FIRST_FIT || newStrategy > WORST_FIT) {
        printf("Invalid strategy. Choose 1, 2, or 3.\n");
        return 0;
    }
    manager->strategy = newStrategy;
    printf("Strategy changed.\n");
    return 1;
}

void allocateMemory(struct MemoryManager *manager, int size)
{
    int index;
    if (size <= 0 || size > manager->poolSize) {
        printf("Invalid size.\n");
        return;
    }
    index = findFreeBlock(manager, size);
    if (index == -1) {
        showAllocationFailure(manager, size);
        return;
    }
    if (!allocateInFreeBlock(manager, index, size, manager->nextID)) {
        return;
    }
    printf("Allocation successful. ID: %d, Start: %d, Size: %d\n",
           manager->nextID, manager->blocks[index].start, size);
    manager->nextID++;
}

void releaseMemory(struct MemoryManager *manager, int id)
{
    int index = findAllocation(manager, id);
    if (index == -1) {
        printf("Allocation ID not found.\n");
        return;
    }
    markBlockFree(manager, index);
    mergeFreeBlocks(manager);
    printf("Allocation released.\n");
}

void resizeMemory(struct MemoryManager *manager, int id, int newSize)
{
    int index = findAllocation(manager, id);
    int oldSize;
    int oldStart;
    int nextBlock;
    int freeIndex;
    int freeStart;
    int i;
    if (index == -1) {
        printf("Allocation ID not found.\n");
        return;
    }
    if (newSize <= 0 || newSize > manager->poolSize) {
        printf("Invalid size.\n");
        return;
    }

    oldSize = manager->blocks[index].size;
    oldStart = manager->blocks[index].start;
    if (newSize == oldSize) {
        printf("Size is unchanged.\n");
        return;
    }

    if (newSize < oldSize) {
        struct Block freeBlock;
        if (!makeRoomForBlock(manager)) {
            return;
        }
        manager->blocks[index].size = newSize;
        freeBlock = makeBlock(oldStart + newSize, oldSize - newSize, 0, 0);
        insertBlock(manager, index + 1, freeBlock);
        mergeFreeBlocks(manager);
        printf("Allocation resized to %d bytes.\n", newSize);
        return;
    }

    nextBlock = index + 1;
    if (nextBlock < manager->blockCount &&
        manager->blocks[nextBlock].isAllocated == 0 &&
        manager->blocks[nextBlock].size >= newSize - oldSize) {
        int extraSize = newSize - oldSize;
        manager->blocks[index].size = newSize;
        manager->blocks[nextBlock].start += extraSize;
        manager->blocks[nextBlock].size -= extraSize;
        if (manager->blocks[nextBlock].size == 0) {
            removeBlock(manager, nextBlock);
        }
        printf("Allocation resized to %d bytes.\n", newSize);
        return;
    }

    freeIndex = findFreeBlock(manager, newSize);
    if (freeIndex == -1) {
        showAllocationFailure(manager, newSize);
        printf("The original allocation was not changed.\n");
        return;
    }

    freeStart = manager->blocks[freeIndex].start;
    if (manager->blocks[freeIndex].size > newSize &&
        manager->blockCount == manager->capacity &&
        !makeRoomForBlock(manager)) {
        return;
    }

    for (i = 0; i < oldSize; i++) {
        manager->memoryPool[freeStart + i] =
            manager->memoryPool[oldStart + i];
    }
    if (!allocateInFreeBlock(manager, freeIndex, newSize, id)) {
        return;
    }
    for (i = 0; i < manager->blockCount; i++) {
        if (manager->blocks[i].start == oldStart &&
            manager->blocks[i].isAllocated == 1 &&
            manager->blocks[i].id == id) {
            markBlockFree(manager, i);
            break;
        }
    }

    mergeFreeBlocks(manager);
    printf("Allocation moved and resized to %d bytes.\n", newSize);
}
