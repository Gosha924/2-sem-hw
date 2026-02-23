#include "csv.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE* file = fopen("input.csv", "r");
    if (file == NULL) {
        printf("input file is not found\n");
        return 1;
    }
    int linesRead = 0;
    char** data = NULL;
    int capacity = 50;
    data = malloc(sizeof(char*) * capacity);
    if (data == NULL) {
        printf("Memory allocation ERROR\n");
        fclose(file);
        return 1;
    }
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (linesRead >= capacity) {
            capacity *= 2;
            char** newData = realloc(data, sizeof(char*) * capacity);
            if (newData == NULL) {
                printf("Memory reallocation ERROR\n");
                for (int i = 0; i < linesRead; i++) {
                    free(data[i]);
                }
                free(data);
                fclose(file);
                return 1;
            }
            data = newData;
        }

        data[linesRead] = malloc(strlen(buffer) + 1);
        strcpy(data[linesRead], buffer);
        linesRead++;
    }
    fclose(file);

    FILE* output = fopen("out.txt", "w");
    if (output == NULL) {
        printf("output file is not found\n");
        for (int i = 0; i < linesRead; i++) {
            free(data[i]);
        }
        free(data);
        return 1;
    }
    int col = countColumns(data, linesRead);
    // вычисление ширины колонок
    int* maxWidth = calculateColumnWidths(data, linesRead, col);
    if (maxWidth == NULL) {
        printf("Error calculating column widths\n");
        for (int i = 0; i < linesRead; i++) {
            free(data[i]);
        }
        free(data);
        fclose(output);
        return 1;
    }
    // вывод таблицы
    printHorizontalLine(output, maxWidth, col, '=');
    for (int i = 0; i < linesRead; i++) {
        char** words = splitLine(data[i], col);
        if (words == NULL) {
            printf("Error splitting line %d\n", i);
            continue;
        }
        printRow(output, words, maxWidth, col, (i == 0));
        if (i == 0) {
            printHorizontalLine(output, maxWidth, col, '=');
        } else if (i < linesRead - 1) {
            printHorizontalLine(output, maxWidth, col, '-');
        }
        freeWords(words, col);
    }
    if (linesRead > 1) {
        printHorizontalLine(output, maxWidth, col, '-');
    }
    free(maxWidth);
    for (int i = 0; i < linesRead; i++) {
        free(data[i]);
    }
    free(data);
    fclose(output);
    return 0;
}
