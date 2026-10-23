#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "employee.h"

void addEmployee(struct Employee ***employees, int *employeeCount) {
    int newID, managerID;
    
    printf("Enter Employee ID: ");
    if (scanf("%d", &newID) != 1) {
    printf("Invalid Employee ID. Please enter a number.\n");

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    free(*employees);
    return;
}
    
    //Employee IDs must be unique
    for (int i = 0;i < *employeeCount;i++) {
        if ((*employees)[i]->Employee_ID == newID) {
            printf("Employee ID already exists.\n");
            return;
        }
    }
    
    //Dynamically allocate memory for the new employee
    struct Employee *newEmployee = (struct Employee *)malloc(sizeof(struct Employee));
    newEmployee->Employee_ID =newID;
    newEmployee->Reporting_Manager =NULL;
    newEmployee->Reports =NULL;
    newEmployee->reportCount =0;
    
    printf("Enter Name: ");
    scanf("%19s", newEmployee->name);
    
    printf("Enter Designation: ");
    int c; while ((c = getchar()) != '\n' && c != EOF); //clear input buffer
    fgets(newEmployee->Designation, 20, stdin);
    newEmployee->Designation[strcspn(newEmployee->Designation, "\n")] = '\0';
    
    printf("Enter Reporting Manager ID (0 for no manager): ");
    if (scanf("%d", &managerID) != 1) {
        printf("Invalid manager ID. Please enter a number.\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        free(newEmployee);
        return;
    }
    
    if (managerID == newID) {
        printf("An employee cannot report to themselves.\n");
        free(newEmployee);
        return;
    }
    
    if (managerID != 0) {
        int foundManager = 0;
        for (int i=0;i < *employeeCount;i++) {
            if ((*employees)[i]->Employee_ID == managerID) {
                struct Employee *manager = (*employees)[i];
                newEmployee->Reporting_Manager = manager;
                
                //Dynamically expand the manager's Reports array
                manager->Reports = (struct Employee **)realloc(manager->Reports, (manager->reportCount + 1) * sizeof(struct Employee *));
                manager->Reports[manager->reportCount++] = newEmployee;
                
                foundManager = 1;
                break;
            }
        }
        if (!foundManager) {
            printf("Manager ID not found.\n");
            free(newEmployee);
            return;
        }
    }
    
    //Add the new employee to the main array dynamically
    *employees = (struct Employee **)realloc(*employees, (*employeeCount + 1) * sizeof(struct Employee *));
    (*employees)[*employeeCount] = newEmployee;
    (*employeeCount)++;
}
