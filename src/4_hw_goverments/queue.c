#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void heapInit(Heap* heap, int capacity)
{
    heap->data = (QueueNode*)malloc(capacity * sizeof(QueueNode));
    heap->size = 0;
    heap->capacity = capacity;
}

void swap(QueueNode* a, QueueNode* b)
{
    QueueNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapUp(Heap* heap, int idx)
{
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[parent].distForCapital <= heap->data[idx].distForCapital) {
            break;
        }
        swap(&heap->data[parent], &heap->data[idx]);
        idx = parent;
    }
}

void heapPush(Heap* heap, int dist, int city, int state)
{
    if (heap->size == heap->capacity) {
        size_t newCapacity = heap->capacity * 2;
        // Используем временную переменную, чтобы избежать утечки при ошибке realloc
        QueueNode* newData = (QueueNode*)realloc(heap->data, newCapacity * sizeof(QueueNode));
        if (newData == NULL) {
            return;
        }
        heap->data = newData;
        heap->capacity = newCapacity;
    }
    int idx = heap->size++;
    heap->data[idx].distForCapital = dist;
    heap->data[idx].city = city;
    heap->data[idx].numGoverment = state;
    heapUp(heap, idx);
}

void heapDown(Heap* heap, int idx)
{
    int size = heap->size;
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        if (left < size && heap->data[left].distForCapital < heap->data[smallest].distForCapital)
            smallest = left;
        if (right < size && heap->data[right].distForCapital < heap->data[smallest].distForCapital)
            smallest = right;
        if (smallest == idx)
            break;
        swap(&heap->data[idx], &heap->data[smallest]);
        idx = smallest;
    }
}

QueueNode heapPop(Heap* heap)
{
    QueueNode min = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapDown(heap, 0);
    return min;
}

void heapFree(Heap* heap)
{
    free(heap->data);
}

int heapEmpty(Heap* heap)
{
    return heap->size == 0;
}
