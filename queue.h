#define MAX_SIZE 256  // Adjust size as needed

typedef struct {
    int x[MAX_SIZE];  // x coordinates
    int y[MAX_SIZE];  // y coordinates
    int front, rear;
} CoordinateQueue;

void initializeQueue(CoordinateQueue *q);
void enqueue(CoordinateQueue *q, int x, int y);
int dequeue(CoordinateQueue *q, int *x, int *y);
int isQueueEmpty(CoordinateQueue *q);
void queueFree(CoordinateQueue *q);
