#pragma once
#include <stdbool.h>
#include <stdio.h>

// вычисляет ширину столбцов
int* calculateColumnWidths(char** strings, int numStrings, int numCols);
// проверяет является ли строка числом
bool isNumber(const char* str, double* result);
// проверяет является ли символ разделителем
int isDelimiter(char c);
// вычисляет количество столбцов
int countColumns(char** data, int linesRead);
// записывает в фаил строку для псевдографики
void printHorizontalLine(FILE* output, const int* widths, int cols, char c);
// Разбиение строки на массив слов
char** splitLine(char* line, int numCols);
// освобождение памяти
void freeWords(char** words, int numCols);
// записывает в фаил строку с данными
void printRow(FILE* output, char** words, const int* widths, int numCols, int isHeader);
