#pragma once
#include <stdbool.h>
#include <stdio.h>

int* calculateColumnWidths(char** strings, int numStrings, int numCols);
bool isNumber(const char* str, double* result);
int isDelimiter(char c);
int countColumns(char** data, int linesRead);
void printHorizontalLine(FILE* output, const int* widths, int cols, char c);
char** splitLine(char* line, int numCols);
void freeWords(char** words, int numCols);
void printRow(FILE* output, char** words, const int* widths, int numCols, int isHeader);
