#include <stdio.h>
#include "employee.h"

int displayRecursive(struct Employee *emp, int depth) {
    if (emp == NULL) return 0;  
    for (int i = 0;i < depth;i++) {
        printf("  "); 
    }
    printf("%d %s %s\n", emp->Employee_ID, emp->name, emp->Designation);
    
    int count = 1; 
    for (int i = 0;i < emp->reportCount;i++) {
        count += displayRecursive(emp->Reports[i], depth + 1);
    }
    return count;
}

void displayEmployeeHierarchy(struct Employee **employees, int employeeCount) {
    int searchID;
    printf("Enter Employee ID: ");
    scanf("%d", &searchID);
    
    for (int i = 0; i < employeeCount; i++) {
        if (employees[i]->Employee_ID == searchID) {
            printf("\nDisplay Hierarchy of Employee %d\n", searchID);
            int total = displayRecursive(employees[i], 0);
            printf("\nTotal Employees Under %d: %d\n", searchID, total - 1);
            return;
        }
    }
    printf("Employee not found.\n");
}
void displayCompleteHierarchy(struct Employee **employees, int employeeCount) {
    printf("\n===== ORGANIZATION HIERARCHY =====\n");
    int totalOrgCount = 0;
    
    for (int i = 0;i < employeeCount;i++) {
        //Top-level employee has no reporting manager
        if (employees[i]->Reporting_Manager == NULL) {
            totalOrgCount += displayRecursive(employees[i], 0);
        }
    }
    printf("\nTotal Employees: %d\n", totalOrgCount);
}