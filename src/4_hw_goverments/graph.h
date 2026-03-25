#pragma once

typedef struct Node {
    int numberNeidorCity;
    int lenRoad;
    struct Node* next;
} Node;

void addEdge(Node** listSmew, int city1, int city2, int len);
void freeGragh(Node** listSmew, int countCity);