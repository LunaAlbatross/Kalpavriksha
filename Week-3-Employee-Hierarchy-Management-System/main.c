#include <stdio.h>
#include <stdlib.h>
#include "employee.h"

int main() {
    struct Employee **employees=NULL;
    int employeeCount =0;
    int choice;
    
    do {
        printf("\n===== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Display Complete Hierarchy\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF); //Clear input buffer
            continue;
        }
        switch (choice) {
            case 1:
                addEmployee(&employees, &employeeCount);
                break;
            case 2:
                removeEmployee(&employees, &employeeCount);
                break;
            case 3:
                displayEmployeeHierarchy(employees, employeeCount);
                break;
            case 4:
                displayCompleteHierarchy(employees, employeeCount);
                break;
            case 5:    
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 5);
    
    //Free all remaining dynamically allocated memory before exiting
    for (int i=00;i < employeeCount;i++) {
        free(employees[i]->Reports);
        free(employees[i]);
    }
    free(employees);
    return 0;
}