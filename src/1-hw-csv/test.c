#include "csv.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Создание тестовых данных
static char** testDataCreate(int numLines, const char* lines[])
{
    char** data = malloc(numLines * sizeof(char*));
    if (data == NULL) {
        return NULL;
    }
    for (int i = 0; i < numLines; ++i) {
        size_t len = strlen(lines[i]);
        data[i] = malloc(len + 1);
        if (!data[i]) {
            // освобождаем уже выделенное
            for (int j = 0; j < i; ++j)
                free(data[j]);
            free(data);
            return NULL;
        }
        strncpy(data[i], lines[i], len);
        data[i][len] = '\0';
    }
    return data;
}

// Освобождение тестовых данных
static void testDataFree(char** data, int numLines)
{
    if (data == NULL)
        return;
    for (int i = 0; i < numLines; ++i)
        free(data[i]);
    free(data);
}

// Тесты

void testIsNumber(void)
{
    printf("Testing isNumber...\n");
    double result = 0;

    // корректные числа
    assert(isNumber("123", &result) == true && result == 123.0);
    assert(isNumber("-45.674956", &result) == true && result == -45.674956);
    assert(isNumber("0", &result) == true && result == 0.0);

    // некорректные
    assert(isNumber("abc", &result) == false);
    assert(isNumber("123abc", &result) == false);
    assert(isNumber("12.34.56", &result) == false);
    assert(isNumber(" ", &result) == false);

    printf("OK\n\n");
}

void testCountColumns(void)
{
    printf("Testing countColumns...\n");
    const char* lines[] = {
        "Name,Age,City",
        "Gosha;25;New York",
        "Karim Benzema|30|London\tCountry",
        "Simple line"
    };

    char** data = testDataCreate(4, lines);
    assert(data != NULL);

    assert(countColumns(data, 1) == 3);
    assert(countColumns(data, 2) == 3);
    assert(countColumns(data, 4) == 3);

    // пустые данные
    assert(countColumns(NULL, 0) == 0);
    assert(countColumns(data, 0) == 0);

    testDataFree(data, 4);
    printf("OK\n\n");
}

void testCalculateColumnWidths(void)
{
    printf("Testing calculateColumnWidths...\n");

    const char* lines[] = {
        "Short,Mediummm,Longestt",
        "A,BB,CCC",
        "VeryLongWord,Short,Medium"
    };

    char** data = testDataCreate(3, lines);
    assert(data != NULL);

    int* widths = calculateColumnWidths(data, 3, 3);
    assert(widths != NULL);
    assert(widths[0] == 12);
    assert(widths[1] == 8);
    assert(widths[2] == 8);

    free(widths);
    testDataFree(data, 3);
    printf("OK\n\n");
}

void testSplitLine(void)
{
    printf("Testing splitLine...\n");
    char line[] = "Gosha,25,New York";
    int numCols = 3;

    char** words = splitLine(line, numCols);
    assert(words != NULL);
    assert(strcmp(words[0], "Gosha") == 0);
    assert(strcmp(words[1], "25") == 0);
    assert(strcmp(words[2], "New York") == 0);
    freeWords(words, numCols);

    // другие разделители
    char line2[] = "Karim Benzema;30;London|Country";
    words = splitLine(line2, 4);
    assert(words != NULL);
    assert(strcmp(words[0], "Karim Benzema") == 0);
    assert(strcmp(words[1], "30") == 0);
    assert(strcmp(words[2], "London") == 0);
    assert(strcmp(words[3], "Country") == 0);
    freeWords(words, 4);

    printf("OK\n\n");
}

void testPrintRow(void)
{
    printf("Testing printRow...\n");

    FILE* temp = tmpfile();
    assert(temp != NULL);

    char* words[] = { "Gosha", "25", "New York" };
    int widths[] = { 6, 2, 8 };

    printRow(temp, words, widths, 3, 0);
    rewind(temp);

    char buffer[256];
    fgets(buffer, sizeof(buffer), temp);
    assert(strstr(buffer, "|") != NULL);
    assert(strstr(buffer, "Gosha") != NULL);
    assert(strstr(buffer, "25") != NULL);
    assert(strstr(buffer, "New York") != NULL);

    fclose(temp);
    printf("OK\n\n");
}

//  Интеграционный тест с реальным файлом
void testIntegration(void)
{
    printf("Integration test...\n");

    FILE* testInput = fopen("testInput.csv", "w");
    assert(testInput != NULL);
    fprintf(testInput, "Name,Age,Salary,City\n");
    fprintf(testInput, "Gosha,25,55555,New York\n");
    fprintf(testInput, "Karim Benzema,30,222222,London\n");
    fprintf(testInput, "Lev,35,22222,Paris\n");
    fclose(testInput);

    FILE* file = fopen("testInput.csv", "r");
    assert(file != NULL);

    char** data = malloc(10 * sizeof(char*));
    assert(data != NULL);
    int linesRead = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        size_t len = strlen(buffer);
        data[linesRead] = malloc(len + 1);
        assert(data[linesRead] != NULL);
        strncpy(data[linesRead], buffer, len);
        data[linesRead][len] = '\0';
        ++linesRead;
    }
    fclose(file);

    assert(linesRead == 4);
    int cols = countColumns(data, linesRead);
    assert(cols == 4);

    int* widths = calculateColumnWidths(data, linesRead, cols);
    assert(widths != NULL);
    assert(widths[0] == 13);
    assert(widths[1] == 3);
    assert(widths[2] == 6);
    assert(widths[3] == 8);

    free(widths);
    for (int i = 0; i < linesRead; ++i)
        free(data[i]);
    free(data);
    remove("testInput.csv");
    printf("OK\n\n");
}

// Пустая таблица
void testEmptyTable(void)
{
    printf("Testing empty table...\n");
    assert(countColumns(NULL, 0) == 0);
    int* widths = calculateColumnWidths(NULL, 0, 5);
    assert(widths == NULL);

    char emptyLine[] = "";
    char** words = splitLine(emptyLine, 3);

    if (words != NULL) {
        for (int i = 0; i < 3; ++i) {
            assert(words[i] == NULL);
        }
        freeWords(words, 3);
    }

    FILE* devNull = fopen("/dev/null", "w");
    if (devNull != NULL) {
        int emptyWidths[] = {};
        printRow(devNull, NULL, emptyWidths, 0, 0);
        fclose(devNull);
    }

    char* dummyData[] = { NULL };
    widths = calculateColumnWidths(dummyData, 0, 3);
    assert(widths == NULL);
    printf("OK\n\n");
}

// ========== Главная функция ==========
int main(void)
{
    printf("RUNNING TESTS FOR CSV.C\n");
    printf("========================================\n\n");

    testIsNumber();
    testCountColumns();
    testCalculateColumnWidths();
    testSplitLine();
    testPrintRow();
    testIntegration();
    testEmptyTable();

    printf("========================================\n");
    printf("ALL TESTS PASSED SUCCESSFULLY!\n");
    return 0;
}