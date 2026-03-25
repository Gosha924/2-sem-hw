#include "graph.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int* distribute(int countCity, Node** listSmev, int* capitals, int countCapitals)
{
    int* result = (int*)malloc((countCity + 1) * sizeof(int));
    if (result == NULL) {
        printf("Error can not allacote memory\n");
        return NULL;
    }
    for (int i = 1; i <= countCity; i++) {
        result[i] = -1;
    }
    Heap heap;
    heapInit(&heap, countCapitals + countCity);
    for (int i = 0; i < countCapitals; i++) {
        int cap = capitals[i];
        if (result[cap] == -1) {
            result[cap] = i;
            heapPush(&heap, 0, cap, i);
        }
    }
    while (!heapEmpty(&heap)) {
        QueueNode node = heapPop(&heap);
        int dist = node.distForCapital;
        int city = node.city;
        int goverment = node.numGoverment;
        if (result[city] != -1 && result[city] != goverment) {
            continue;
        }
        if (result[city] == -1) {
            result[city] = goverment;
        }
        for (Node* current = listSmev[city]; current; current = current->next) {
            int numberNeidorCity = current->numberNeidorCity;
            int newDist = dist + current->lenRoad;
            if (result[numberNeidorCity] == -1) {
                heapPush(&heap, newDist, numberNeidorCity, goverment);
            }
        }
    }
    heapFree(&heap);
    return result;
}

int main(int arg, char* argv[])
{
    // открытие файла
    if (arg < 2) {
        printf("Input file is not found\n");
        return 1;
    }

    char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File could not be opened\n");
        return 1;
    }
    // чтение из файла
    int countCity, countRoads;
    fscanf(file, "%d", &countCity);
    fscanf(file, "%d", &countRoads);
    Node** listSmew = (Node**)calloc(countCity + 1, sizeof(Node*));
    if (listSmew == NULL) {
        printf("Error can not allacote memory\n");
        fclose(file);
        return 1;
    }
    for (int i = 0; i < countRoads; i++) {
        int city1, city2, len;
        if (fscanf(file, "%d %d %d", &city1, &city2, &len) != 3) {
            printf("Eror wrong recoding in file string %d\n", i + 1);
            freeGragh(listSmew, countCity);
            fclose(file);
            return 1;
        }
        addEdge(listSmew, city1, city2, len);
    }
    int countCapitals;
    fscanf(file, "%d", &countCapitals);
    int* capitals = (int*)malloc(countCapitals * sizeof(int));
    if (capitals == NULL) {
        printf("Error can not allacote memory\n");
        return 1;
    }
    for (int i = 0; i < countCapitals; i++) {
        int capital;
        if (fscanf(file, "%d", &capital) != 1) {
            printf("Eror reading capitals string %d\n", i + 1);
            free(capitals);
            freeGragh(listSmew, countCity);
            fclose(file);
            return 1;
        }
        capitals[i] = capital;
    }
    fclose(file);

    int* result = distribute(countCity, listSmew, capitals, countCapitals);
    for (int s = 0; s < countCapitals; s++) {
        printf("Capital %d has cities:", capitals[s]);
        for (int city = 1; city <= countCity; city++) {
            if (result[city] == s && city != capitals[s]) {
                printf(" %d", city);
            }
        }
        printf("\n");
    }
    freeGragh(listSmew, countCity);
    free(result);
    free(capitals);
    return 0;
}