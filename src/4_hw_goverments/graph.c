#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

void addEdge(Node** listSmew, int city1, int city2, int len)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Error can not allacote memory\n");
        return;
    }
    newNode->lenRoad = len;
    newNode->numberNeidorCity = city2;
    newNode->next = listSmew[city1];
    listSmew[city1] = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Error can not allocate memory\n");
        return;
    }
    newNode->lenRoad = len;
    newNode->numberNeidorCity = city1;
    newNode->next = listSmew[city2];
    listSmew[city2] = newNode;
}

void freeGragh(Node** listSmew, int countCity)
{
    for (int i = 1; i <= countCity; i++) {
        Node* current = listSmew[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(listSmew);
}