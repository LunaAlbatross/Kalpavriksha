#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"

int readInteger(const char *prompt, int *value)
{
    char line[100];
    char *end;
    long number;
    int character;

    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }
    if (strchr(line, '\n') == NULL && !feof(stdin)) {
        while ((character = getchar()) != '\n' && character != EOF) {
        }
        printf("Please enter a valid whole number.\n");
        return -1;
    }
    errno = 0;
    number = strtol(line, &end, 10);
    if (end == line || errno == ERANGE ||
        number < INT_MIN || number > INT_MAX) {
        printf("Please enter a valid whole number.\n");
        return -1;
    }
    while (isspace((unsigned char)*end)) {
        end++;
    }
    if (*end != '\0') {
        printf("Please enter a valid whole number.\n");
        return -1;
    }
    *value = (int)number;
    return 1;
}
