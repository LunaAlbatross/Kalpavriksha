#include <stdio.h>
#include "headers.h"

int main(void)
{
    struct MemoryManager manager = {0};
    int poolSize;
    int selectedStrategy;
    int choice;
    int size;
    int id;
    int result;

    result = readInteger("Enter memory pool size (1 to 1000000 bytes): ", &poolSize);
    if (result != 1 || !initializeMemory(&manager, poolSize)) {
        return 1;
    }

    result = readInteger("Choose strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): ", &selectedStrategy);
    if (result != 1 || !setStrategy(&manager, selectedStrategy)) {
        freeMemory(&manager);
        return 1;
    }

    while (1) {
        printf("\nMenu:\n");
        printf("1. Allocate memory\n");
        printf("2. Release memory\n");
        printf("3. Resize an allocation\n");
        printf("4. Display memory map\n");
        printf("5. Display memory statistics\n");
        printf("6. Change allocation strategy\n");
        printf("7. Exit\n");

        result = readInteger("Choice: ", &choice);
        if (result == 0) {
            break;
        }
        if (result < 0) {
            continue;
        }
        if (choice == 1) {
            if (readInteger("Requested size: ", &size) == 1) {
                allocateMemory(&manager, size);
            }
        } else if (choice == 2) {
            if (readInteger("Allocation ID: ", &id) == 1) {
                releaseMemory(&manager, id);
            }
        } else if (choice == 3) {
            if (readInteger("Allocation ID: ", &id) == 1 &&
                readInteger("New size: ", &size) == 1) {
                resizeMemory(&manager, id, size);
            }
        } else if (choice == 4) {
            displayMemoryMap(&manager);
        } else if (choice == 5) {
            displayStatistics(&manager);
        } else if (choice == 6) {
            if (readInteger(
                    "Choose strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): ",
                    &selectedStrategy) == 1) {
                setStrategy(&manager, selectedStrategy);
            }
        } else if (choice == 7) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    freeMemory(&manager);
    return 0;
}
