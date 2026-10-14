#include <stdio.h>

struct student{
    int roll_number;
    char name[50];
    int mark1;
    int mark2;
    int mark3;
};

int calculateTotal(int mark1, int mark2, int mark3){
    return mark1 + mark2 + mark3;
}

float calculateAverage(int total){
    return (float)total / 3;
}

char calculateGrade(float average){
    if (average >= 85) return 'A';
    else if (average >= 70) return 'B';
    else if (average >= 50) return 'C';
    else if (average >= 35) return 'D';
    else return 'F';
}

void printRollNumber(int currentRollNumber, int n){
    if (currentRollNumber > n) return;
    printf("%d ", currentRollNumber);
    printRollNumber(currentRollNumber + 1, n);
}

int main(){
    int n;
    printf("Enter the number of students: ");
    scanf("%d", &n);
    if (n < 1 || n>100) {
        printf("Invalid number of students.\n");
        return 1;
    }
    struct student students[100];
    for(int i=0; i<n; i++){ 
        printf("Enter details for student %d: ", i + 1);
        scanf("%d", &students[i].roll_number);
        scanf("%49s", students[i].name);
        scanf("%d", &students[i].mark1);
        scanf("%d", &students[i].mark2);
        scanf("%d", &students[i].mark3);
        if (students[i].mark1 < 0 || students[i].mark1 > 100 ||
            students[i].mark2 < 0 || students[i].mark2 > 100 ||
            students[i].mark3 < 0 || students[i].mark3 > 100) {
            printf("Invalid marks entered. Please enter marks between 0 and 100.\n");
            return 1;
        }
        printf("\n");

        int total = calculateTotal(students[i].mark1, students[i].mark2, students[i].mark3);
        float average = calculateAverage(total);
        char grade = calculateGrade(average);

        printf("Roll: %d\n", students[i].roll_number);
        printf("Name: %s\n", students[i].name);
        printf("Total: %d\n", total);
        printf("Average: %.2f\n", average);
        printf("Grade: %c\n", grade);

        int stars;
        if (grade == 'A') stars = 5;
        else if (grade == 'B') stars = 4;
        else if (grade == 'C') stars = 3;
        else if (grade == 'D') stars = 2;

        if (grade=='F'){
            continue;
        }
        else{
            printf("Performance: ");
            for (int j = 0; j < stars; j++) {
            printf("*");
          }
          printf("\n\n");
        }

    }

    printf("List of Roll Numbers (via recursion): ");
    printRollNumber(1, n);
    return 0;
}

