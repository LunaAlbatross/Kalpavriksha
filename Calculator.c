#include <stdio.h>
#include <ctype.h>
int main() {
    char str[100];
    int result = 0;
    int term = 0;
    int num = 0;
    char op = '+';
    int expect=1;
    scanf("%99[^\n]", str);
    for (int i = 0; str[i] != '\0'; ) {
        if (isspace(str[i])) {
            i++;
            continue;
        }
        if (isdigit(str[i])) {
            if (expect==0){
                printf("Error: Invalid expression.\n");
                return 1;
            }
            num = 0;
            while (isdigit(str[i])) {
                num = num * 10 + (str[i] - '0');
                i++;
            }
            expect = 0;
            switch (op) {
                case '+':
                    result += term;
                    term = num;
                    break;
                case '-':
                    result += term;
                    term = -num;
                    break;
                case '*':
                    term *= num;
                    break;
                case '/':
                    if (num == 0) {
                        printf("Error: Division by zero.\n");
                        return 1;
                    }
                    term /= num;
                    break;
                default:
                    printf("Error: Invalid operator.\n");
                    break;
            }
            continue;
        }
        if (str[i] == '+' || str[i] == '-' ||
            str[i] == '*' || str[i] == '/') {
            if (expect==1){
                printf("Error: Invalid expression.\n");
                return 1;
            }
            op = str[i];
            expect=1;
            i++;
            continue;
        }
        printf("Error: Invalid expression.\n");
        return 1;
    }
    if (expect==1){
        printf("Error: Invalid expression.\n");
        return 1;
    }
    result += term;
    printf("%d\n", result);
    return 0;
}