#pragma once

typedef struct QueueNode {
    int distForCapital;
    int city;
    int numGoverment;
} QueueNode;

typedef struct Heap {
    QueueNode* data;
    int size;
    int capacity;
} Heap;

void heapInit(Heap* heap, int capacity);
void swap(QueueNode* a, QueueNode* b);
void heapPush(Heap* heap, int dist, int city, int state);
void heapUp(Heap* heap, int idx);
void heapDown(Heap* heap, int idx);
QueueNode heapPop(Heap* heap);
void heapFree(Heap* heap);
int heapEmpty(Heap* heap);
