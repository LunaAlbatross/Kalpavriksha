#include <stdio.h>
#include <stdlib.h>
#include "employee.h"

void removeEmployee(struct Employee ***employees, int *employeeCount) {
    if (*employeeCount == 0) {
        printf("No employees to remove.\n");
        return;
    }

    int removeID;
    printf("Enter the Employee ID to remove: ");
    if (scanf("%d", &removeID) != 1) {
    printf("Invalid Employee ID. Please enter a number.\n");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    return;
}

    int indexToRemove = -1;
    for (int i = 0;i < *employeeCount;i++) {
        if ((*employees)[i]->Employee_ID == removeID) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        printf("Employee not found.\n");
        return;
    }

    struct Employee *removedEmployee = (*employees)[indexToRemove];
    struct Employee *manager = removedEmployee->Reporting_Manager;

    //Remove the employee from their current manager's Reports array
    if (manager != NULL) {
        for (int i = 0;i < manager->reportCount;i++) {
            if (manager->Reports[i] == removedEmployee) {
                for (int j = i;j < manager->reportCount - 1;j++) {
                    manager->Reports[j] = manager->Reports[j + 1];
                }
                manager->reportCount--;
                if (manager->reportCount == 0) {
                    free(manager->Reports);
                    manager->Reports = NULL;
                } else {
                    manager->Reports = (struct Employee **)realloc(manager->Reports, manager->reportCount * sizeof(struct Employee *));
                }
                break;
            }
        }
    }
    
    //Reassign the removed employee's reports to their manager
    for (int i = 0;i < removedEmployee->reportCount;i++) {
        struct Employee *report = removedEmployee->Reports[i];
        report->Reporting_Manager = manager;
        
        if (manager != NULL) {
            manager->Reports = (struct Employee **)realloc(manager->Reports, (manager->reportCount + 1) * sizeof(struct Employee *));
            manager->Reports[manager->reportCount++] = report;
        }
    }
    
    //Free the removed employee's dynamically allocated memory
    free(removedEmployee->Reports);
    free(removedEmployee);
    
    //Shift the main employees array to close the gap and resize
    for (int i = indexToRemove;i < *employeeCount - 1;i++) {
        (*employees)[i] = (*employees)[i + 1];
    }
    (*employeeCount)--;
    
    if (*employeeCount == 0) {
        free(*employees);
        *employees = NULL;
    } else {
        *employees = (struct Employee **)realloc(*employees, (*employeeCount) * sizeof(struct Employee *));
    }
}   