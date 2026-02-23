#pragma once
#include <stdbool.h>
#include <stdio.h>

int* calculateColumnWidths(char** strings, int numStrings, int numCols);
bool isNumber(char* str, double* result);
int isDelimiter(char c);
int countColumns(char** data, int linesRead);
void printHorizontalLine(FILE* output, int* widths, int cols, char c);
char** splitLine(char* line, int numCols);
void freeWords(char** words, int numCols);
void printRow(FILE* output, char** words, int* widths, int numCols, int isHeader);
