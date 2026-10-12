#include <stdio.h>
#include <ctype.h>

//Function declarations
int parseNumber(char str[], int *index);
int applyOperator(char currentOperator, int currentNumber, int *currentTerm, int *result);
int parseExpression(char str[], int *result);

// Main function
int main() {
    char str[100];
    printf("Enter an mathematical expression: ");
    scanf("%99[^\n]", str);
    int result;
    //If parsing the expression is successful, print the result
    if (parseExpression(str, &result)) {
        printf("Result: %d\n", result);
    }
    return 0;
}

// Function to parse and evaluate the expression
int parseExpression(char str[], int *result)
{
    *result = 0;

    int currentTerm = 0;
    int currentNumber = 0;
    char currentOperator = '+';
    int expectingNumber=1;
    for (int i = 0; str[i] != '\0'; ) {
        if (isspace(str[i])) {
            i++;
            continue;
        }
        if (isdigit(str[i])) {
            if (!expectingNumber) {
                printf("Error: Invalid expression.\n");
                return 0;
            }
            currentNumber = parseNumber(str, &i);// Parse the number and update the index
            expectingNumber = 0;

            // Apply the current operator to the current number and update the result and current term
            if (!applyOperator(currentOperator, currentNumber, &currentTerm, result)){
                if (currentOperator == '/' && currentNumber == 0)
                {
                    printf("Error: Division by zero.\n");
                }
                else
                {
                    printf("Error: Invalid expression.\n");
                }
            
                return 0; 
            }
            continue;
        }

        // If the character is an operator, update the current operator and set expectingNumber to true
        if (str[i] == '+' || str[i] == '-' ||
            str[i] == '*' || str[i] == '/') {
            if (expectingNumber) {
                printf("Error: Invalid expression.\n");
                return 0;
            }
            currentOperator = str[i];
            expectingNumber = 1;
            i++;
            continue;
        }
        printf("Error: Invalid expression.\n");
        return 0;
    }
    if (expectingNumber) {
        printf("Error: Invalid expression.\n");
        return 0;
    }
    *result += currentTerm;
    return 1;
}

// Function to parse a number from the string and update the index
int parseNumber(char str[], int *index){
    int number = 0;
    while (isdigit(str[*index])) {
        number = number * 10 + (str[*index] - '0');
        (*index)++;
    }
    return number;
}

// Function to apply the current operator to the current number and update the result and current term
int applyOperator(char currentOperator, int currentNumber,
                  int *currentTerm, int *result)
{
    switch (currentOperator)
    {
        case '+':
            *result += *currentTerm;
            *currentTerm = currentNumber;
            break;

        case '-':
            *result += *currentTerm;
            *currentTerm = -currentNumber;
            break;

        case '*':
            *currentTerm *= currentNumber;
            break;

        case '/':
            if (currentNumber == 0)
            {
                return 0;
            }
            *currentTerm /= currentNumber;
            break;

        default:
            return 0;
    }

    return 1;
}
