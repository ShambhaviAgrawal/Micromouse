#include "queue.h"
#include <stdio.h>

void initializeQueue(CoordinateQueue *queue) {
    queue->front = queue->rear = -1;
}

int isQueueEmpty(CoordinateQueue *queue) {
    return queue->front == -1;
}

int isQueueFull(CoordinateQueue *queue) {
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

void enqueue(CoordinateQueue *queue, int x, int y) {
    if (isQueueFull(queue)) {
        printf("Queue is full.\n");
        return;
    }
    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_SIZE;
    }
    queue->x[queue->rear] = x;
    queue->y[queue->rear] = y;
}

int dequeue(CoordinateQueue *queue, int *x, int *y) {
    if (isQueueEmpty(queue)) {
        return 0;  // Indicates queue is empty
    }
    *x = queue->x[queue->front];
    *y = queue->y[queue->front];
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;  // Reset queue if last item is dequeued
    } else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }
    return 1;  // Success
}

void queueFree(CoordinateQueue *q) {
        q->front = q->rear = -1;
}
