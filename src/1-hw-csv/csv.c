#include "csv.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int* calculateColumnWidths(char** strings, int numStrings, int numCols)
{
    int* widths = (int*)calloc(numCols, sizeof(int));
    if (!widths) {
        return NULL;
    }
    for (int i = 0; i < numStrings; i++) {
        char line[1024];
        strcpy(line, strings[i]);

        int countCol = 0;
        char* token = strtok(line, ",\t;|");
        while (token != NULL && countCol < numCols) {
            int len = strlen(token);
            if (len > widths[countCol]) {
                widths[countCol] = len;
            }
            countCol++;
            token = strtok(NULL, ",\t;|");
        }
    }
    return widths;
}

bool isNumber(char* str, double* result)
{
    if (str == NULL || *str == '\0')
        return false;

    char* endptr;
    *result = strtod(str, &endptr);
    return (endptr != str && *endptr == '\0');
}

int isDelimiter(char c)
{
    char delimiters[] = ",;\t|";
    for (int i = 0; delimiters[i] != '\0'; i++) {
        if (c == delimiters[i]) {
            return 1;
        }
    }
    return 0;
}

int countColumns(char** data, int linesRead)
{
    if (linesRead == 0) {
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

void printHorizontalLine(FILE* output, int* widths, int cols, char c)
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
    strcpy(temp, line);
    int col = 0;
    char* token = strtok(temp, ",\t;|");
    while (token != NULL && col < numCols) {
        words[col] = malloc(strlen(token) + 1);
        strcpy(words[col], token);
        col++;
        token = strtok(NULL, ",\t;|");
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

void printRow(FILE* output, char** words, int* widths, int numCols, int isHeader)
{
    fprintf(output, "|");
    for (int j = 0; j < numCols; j++) {
        int len = strlen(words[j]);
        int padding = widths[j] - len;
        fprintf(output, " ");
        double value;
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