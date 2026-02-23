#include "csv.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вспомогательная функция для создания тестовых данных
char** createTestData(int numLines, const char* lines[])
{
    char** data = malloc(numLines * sizeof(char*));
    for (int i = 0; i < numLines; i++) {
        data[i] = malloc(strlen(lines[i]) + 1);
        strcpy(data[i], lines[i]);
    }
    return data;
}

// Вспомогательная функция для освобождения тестовых данных
void freeTestData(char** data, int numLines)
{
    for (int i = 0; i < numLines; i++) {
        free(data[i]);
    }
    free(data);
}

// 1 Тестирование isNumber
void test_isNumber()
{
    printf("Тестирование isNumber...\n");
    double result;
    // Корректные входные данные
    assert(isNumber("123", &result) == true && result == 123.0);
    assert(isNumber("-45.674956", &result) == true && result == -45.674956);
    assert(isNumber("0", &result) == true && result == 0.0);
    // Некорректные входные данные
    assert(isNumber("abc", &result) == false);
    assert(isNumber("123abc", &result) == false);
    assert(isNumber("12.34.56", &result) == false);
    assert(isNumber(" ", &result) == false);

    printf("Тест isNumber пройден\n\n");
}

// 2 Тестирование countColumns
void test_countColumns()
{
    printf("Тестирование countColumns...\n");
    const char* lines[] = {
        "Name,Age,City",
        "Gosha;25;New York",
        "Karim Benzema|30|London\tCountry",
        "Simple line"
    };

    char** data = createTestData(4, lines);

    assert(countColumns(data, 1) == 3);
    assert(countColumns(data, 2) == 3);
    assert(countColumns(data, 4) == 3);

    // Пустые данные
    assert(countColumns(NULL, 0) == 0);
    assert(countColumns(data, 0) == 0);

    freeTestData(data, 4);
    printf("Тест countColumns пройден\n\n");
}

// 3 Тестирование calculateColumnWidths
void test_calculateColumnWidths()
{
    printf("Тестирование calculateColumnWidths\n");

    const char* lines[] = {
        "Short,Mediummm,Longestt",
        "A,BB,CCC",
        "VeryLongWord,Short,Medium"
    };

    char** data = createTestData(3, lines);
    int* widths = calculateColumnWidths(data, 3, 3);
    assert(widths != NULL);
    assert(widths[0] == 12);
    assert(widths[1] == 8);
    assert(widths[2] == 8);

    free(widths);
    freeTestData(data, 3);
    printf("Тест calculateColumnWidths пройден\n\n");
}

// 4 Тестирование splitLine
void test_splitLine()
{
    printf("Тестирование splitLine\n");
    char line[] = "Gosha,25,New York";
    int numCols = 3;

    char** words = splitLine(line, numCols);
    assert(words != NULL);

    assert(strcmp(words[0], "Gosha") == 0);
    assert(strcmp(words[1], "25") == 0);
    assert(strcmp(words[2], "New York") == 0);

    freeWords(words, numCols);
    // Тест с другими разделителями
    char line2[] = "Karim Benzema;30;London|Country";
    words = splitLine(line2, 4);
    assert(words != NULL);

    assert(strcmp(words[0], "Karim Benzema") == 0);
    assert(strcmp(words[1], "30") == 0);
    assert(strcmp(words[2], "London") == 0);
    assert(strcmp(words[3], "Country") == 0);

    freeWords(words, 4);
    printf("Тест splitLine пройден\n\n");
}

//  5 Тестирование printRow
void test_printRow()
{
    printf("Тестирование printRow...\n");

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
    printf("Тест printRow пройден\n\n");
}

// 6 Тест с реальным файлом
void test_integration()
{
    printf("Интеграционное тестирование\n");

    FILE* test_input = fopen("test_input.csv", "w");
    assert(test_input != NULL);

    fprintf(test_input, "Name,Age,Salary,City\n");
    fprintf(test_input, "Gosha,25,55555,New York\n");
    fprintf(test_input, "Karim Benzema,30,222222,London\n");
    fprintf(test_input, "Lev,35,22222,Paris\n");

    fclose(test_input);

    FILE* file = fopen("test_input.csv", "r");
    assert(file != NULL);

    char** data = malloc(10 * sizeof(char*));
    int linesRead = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        data[linesRead] = malloc(strlen(buffer) + 1);
        strcpy(data[linesRead], buffer);
        linesRead++;
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

    for (int i = 0; i < linesRead; i++) {
        free(data[i]);
    }
    free(data);
    remove("test_input.csv");
    printf("Интеграционный тест пройден\n\n");
}

int main()
{
    printf("ЗАПУСК ТЕСТОВ ДЛЯ CSV.C\n");
    printf("=====================================\n\n");

    test_isNumber();
    test_countColumns();
    test_calculateColumnWidths();
    test_splitLine();
    test_printRow();
    test_integration();

    printf("========================================\n");
    printf("ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n");
    return 0;
}