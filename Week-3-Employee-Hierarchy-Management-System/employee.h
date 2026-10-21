#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Employee {
    int Employee_ID;
    char name[20];
    char Designation[20];
    struct Employee *Reporting_Manager;
    struct Employee **Reports; //Dynamically allocated array of pointers
    int reportCount;
};

void addEmployee(struct Employee ***employees, int *employeeCount);
void removeEmployee(struct Employee ***employees, int *employeeCount);
void displayEmployeeHierarchy(struct Employee **employees, int employeeCount);
void displayCompleteHierarchy(struct Employee **employees, int employeeCount);
int displayRecursive(struct Employee *emp, int depth);

#endif 