#ifndef HEADERS_H
#define HEADERS_H
#define MAX_POOL_SIZE 1000000

#define FIRST_FIT 1
#define BEST_FIT 2
#define WORST_FIT 3

struct Block{
    int start;
    int size;
    int id;
    int isAllocated;
};

struct MemoryManager {
    unsigned char *memoryPool;
    struct Block *blocks;
    int poolSize;
    int blockCount;
    int capacity;
    int nextID;
    int strategy;
    int fragmentedRequest;
    int freeBytesWhenFragmented;
};

int readInteger(const char *prompt, int *value);
int initializeMemory(struct MemoryManager *manager, int size);
void freeMemory(struct MemoryManager *manager);
int setStrategy(struct MemoryManager *manager, int newStrategy);
void allocateMemory(struct MemoryManager *manager, int size);
void releaseMemory(struct MemoryManager *manager, int id);
void resizeMemory(struct MemoryManager *manager, int id, int newSize);
void displayMemoryMap(const struct MemoryManager *manager);
void displayStatistics(const struct MemoryManager *manager);

#endif
