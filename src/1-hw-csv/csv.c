#include "csv.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define DIVIDERS ",\t;|"

int* calculateColumnWidths(char** strings, int numStrings, int numCols)
{
    if (strings == NULL || numStrings == 0) {
        return NULL;
    }
    int* widths = (int*)calloc(numCols, sizeof(int));
    if (widths == NULL) {
        return NULL;
    }
    for (int i = 0; i < numStrings; i++) {
        char line[MAX_LINE_LENGTH];
        strncpy(line, strings[i], sizeof(line) - 1);
        line[sizeof(line) - 1] = '\0';

        int countCol = 0;
        char* token = strtok(line, DIVIDERS);
        while (token != NULL && countCol < numCols) {
            int len = (int)strlen(token);
            if (len > widths[countCol]) {
                widths[countCol] = len;
            }
            countCol++;
            token = strtok(NULL, DIVIDERS);
        }
    }
    return widths;
}

bool isNumber(const char* str, double* result)
{
    if (str == NULL || *str == '\0')
        return false;

    char* endptr;
    *result = strtod(str, &endptr);
    return (endptr != str && *endptr == '\0');
}

int isDelimiter(char c)
{
    return strchr(DIVIDERS, c) != NULL;
}

int countColumns(char** data, int linesRead)
{
    if (linesRead == 0 || data == NULL) {
        return 0;
    }
    int count = 1;
    for (int j = 0; data[0][j] != '\0'; j++) {
        if (isDelimiter(data[0][j])) {
            count++;
        }
    }
    return count;
}

void printHorizontalLine(FILE* output, const int* widths, int cols, char c)
{
    fprintf(output, "+");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            fprintf(output, "%c", c);
        }
        fprintf(output, "+");
    }
    fprintf(output, "\n");
}

// Разбиение строки на массив слов
char** splitLine(char* line, int numCols)
{
    char** words = malloc(sizeof(char*) * numCols);
    if (words == NULL) {
        return NULL;
    }
    char temp[1024];
    strncpy(temp, line, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    int col = 0;
    char* token = strtok(temp, DIVIDERS);
    while (token != NULL && col < numCols) {
        words[col] = malloc(strlen(token) + 1);
        if (words[col] == NULL) {
            return NULL;
        }
        strncpy(words[col], token, strlen(token) + 1);
        col++;
        token = strtok(NULL, DIVIDERS);
    }
    return words;
}

void freeWords(char** words, int numCols)
{
    for (int i = 0; i < numCols; i++) {
        free(words[i]);
    }
    free(words);
}

void printRow(FILE* output, char** words, const int* widths, int numCols, int isHeader)
{
    fprintf(output, "|");
    for (int j = 0; j < numCols; j++) {
        int len = (int)strlen(words[j]);
        int padding = widths[j] - len;
        fprintf(output, " ");
        double value = 0;
        bool isNum = (!isHeader && isNumber(words[j], &value));

        if (isNum) {
            // числа вправо
            for (int k = 0; k < padding; k++) {
                fprintf(output, " ");
            }
            fprintf(output, "%s", words[j]);
        } else {
            // текст влево
            fprintf(output, "%s", words[j]);
            for (int k = 0; k < padding; k++) {
                fprintf(output, " ");
            }
        }
        fprintf(output, " |");
    }
    fprintf(output, "\n");
}