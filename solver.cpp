#include "solver.h"
#include "API.h"
#include "queue.h"
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

#define MAZE_SIZE 16  // Assuming a 16x16 maze

Heading current_orientation = NORTH;

int cur_x = -1, cur_y = -1;

void infloodfill();

int min(int x, int y) {
    if(x>y)
        return y;
    else
        return x;
}

typedef struct {
    bool walls[4];  // 0: North, 1: East, 2: South, 3: West
    int value;      // Floodfill value
} Cell;

Cell maze[MAZE_SIZE][MAZE_SIZE];
// maze[x][y] denotes that the mouse is in the xth column from the left and yth row from the bottom

void initializeMaze() {
    API_ackReset();
    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            maze[x][y].value = INT_MAX;  // Initialize with maximum values
            for (int i = 0; i < 4; i++) {
                maze[x][y].walls[i] = false;  // Walls are unknown
            }
            API_setText(x, y, " ");
        }
    }
    // Set the goal cell value and update simulation
    maze[MAZE_SIZE/2][MAZE_SIZE/2].value = 0;
    if(MAZE_SIZE%2==1) {
        debug_log("Setting our middle values for odd");
        maze[MAZE_SIZE/2][MAZE_SIZE/2].value = 0;
        API_setText(MAZE_SIZE/2, MAZE_SIZE/2, "0");
    }
    else {
        debug_log("Setting our middle values for even");
        maze[MAZE_SIZE/2][MAZE_SIZE/2].value = 0;
        maze[MAZE_SIZE/2][MAZE_SIZE/2 - 1].value = 0;
        maze[MAZE_SIZE/2 - 1][MAZE_SIZE/2].value = 0;
        maze[MAZE_SIZE/2 - 1][MAZE_SIZE/2 - 1].value = 0;
        API_setText(MAZE_SIZE/2, MAZE_SIZE/2, "0");
        API_setText(MAZE_SIZE/2, MAZE_SIZE/2 - 1, "0");
        API_setText(MAZE_SIZE/2 - 1, MAZE_SIZE/2, "0");
        API_setText(MAZE_SIZE/2 - 1, MAZE_SIZE/2 - 1, "0");
    }
    infloodfill();  // Pre-compute floodfill from the goal
}

void infloodfill() {
    CoordinateQueue q;
    initializeQueue(&q);
    if(MAZE_SIZE%2==1)
        enqueue(&q, MAZE_SIZE/2, MAZE_SIZE/2);
    else {
        enqueue(&q, MAZE_SIZE/2, MAZE_SIZE/2);
        enqueue(&q, MAZE_SIZE/2, MAZE_SIZE/2 - 1);
        enqueue(&q, MAZE_SIZE/2 - 1, MAZE_SIZE/2);
        enqueue(&q, MAZE_SIZE/2 - 1, MAZE_SIZE/2 - 1);
    }

    while (!isQueueEmpty(&q)) {
        int x, y;
        dequeue(&q, &x, &y);
        updateVisualization(x, y);

        // Check each accessible neighbor and find the minimum value
        if (y < MAZE_SIZE-1 && maze[x][y].value+1 < maze[x][y+1].value) {
            maze[x][y+1].value = maze[x][y].value+1;  // North
            enqueue(&q, x, y+1);
        }
        if (x < MAZE_SIZE-1 && maze[x][y].value+1 < maze[x+1][y].value) { 
            maze[x+1][y].value = maze[x][y].value+1;  // East
            enqueue(&q, x+1, y);
        }
        if (y > 0 && maze[x][y].value+1 < maze[x][y-1].value) { 
            maze[x][y-1].value = maze[x][y].value+1;  // South
            enqueue(&q, x, y-1);
        }
        if (x > 0 && maze[x][y].value+1 < maze[x-1][y].value) { 
            maze[x-1][y].value = maze[x][y].value+1;  // West
            enqueue(&q, x-1, y);
        }
    }
    queueFree(&q);
}

void discoverWalls(int x, int y) {
    if(API_wallFront()) {
        switch (current_orientation)
        {
        case NORTH:
            maze[x][y].walls[0] = true;
            API_setWall(x, y, 'n');
            break;
        case EAST:
            maze[x][y].walls[1] = true;
            API_setWall(x, y, 'e');
            break;
        case SOUTH:
            maze[x][y].walls[2] = true;
            API_setWall(x, y, 's');
            break;
        case WEST:
            maze[x][y].walls[3] = true;
            API_setWall(x, y, 'w');
            break;
        default:
            break;
        }
    }
    if(API_wallLeft()) {
        switch (current_orientation)
        {
        case NORTH:
            maze[x][y].walls[3] = true;
            API_setWall(x, y, 'w');
            break;
        case EAST:
            maze[x][y].walls[0] = true;
            API_setWall(x, y, 'n');
            break;
        case SOUTH:
            maze[x][y].walls[1] = true;
            API_setWall(x, y, 'e');
            break;
        case WEST:
            maze[x][y].walls[2] = true;
            API_setWall(x, y, 's');
            break;
        default:
            break;
        }
    }
    if(API_wallRight()) {
        switch (current_orientation)
        {
        case NORTH:
            maze[x][y].walls[1] = true;
            API_setWall(x, y, 'e');
            break;
        case EAST:
            maze[x][y].walls[2] = true;
            API_setWall(x, y, 's');
            break;
        case SOUTH:
            maze[x][y].walls[3] = true;
            API_setWall(x, y, 'w');
            break;
        case WEST:
            maze[x][y].walls[0] = true;
            API_setWall(x, y, 'n');
            break;
        default:
            break;
        }
    }
}

void upfloodfill(int rx, int ry) {
    int x, y;
    CoordinateQueue q;
    initializeQueue(&q);
    enqueue(&q, rx, ry);
    while (!isQueueEmpty(&q)) {
        dequeue(&q, &x, &y);
        int minValue = INT_MAX;
        // Check each accessible neighbor and find the minimum value
        if (x > 0 && !maze[x][y].walls[3]) minValue = min(minValue, maze[x-1][y].value);  // West
        if (x < MAZE_SIZE-1 && !maze[x][y].walls[1]) minValue = min(minValue, maze[x+1][y].value);  // East
        if (y > 0 && !maze[x][y].walls[2]) minValue = min(minValue, maze[x][y-1].value);  // South
        if (y < MAZE_SIZE-1 && !maze[x][y].walls[0]) minValue = min(minValue, maze[x][y+1].value);  // North

        if(maze[x][y].value != minValue+1) {
            maze[x][y].value = minValue + 1;
            updateVisualization(x, y);
            // Enqueue all accessible neighbors
            if (x > 0 && !maze[x][y].walls[3]) enqueue(&q, x-1, y);
            if (x < MAZE_SIZE-1 && !maze[x][y].walls[1]) enqueue(&q, x+1, y);
            if (y > 0 && !maze[x][y].walls[2]) enqueue(&q, x, y-1);
            if (y < MAZE_SIZE-1 && !maze[x][y].walls[0]) enqueue(&q, x, y+1);
        }
    }
    queueFree(&q);
}

void updateVisualization(int x, int y) {
    char buffer[5];
    sprintf(buffer, "%d", maze[x][y].value);
    API_setText(x, y, buffer);
}

Action decideNextMove(int x, int y) {
    // Decision logic based on the floodfill values
    // Simplified example: always move to the cell with the lowest floodfill value that is accessible
    int minValue = maze[x][y].value;
    Action bestMove = STOP;
    if (y < MAZE_SIZE-1 && !maze[x][y].walls[0] && maze[x][y+1].value < minValue) {
        minValue = maze[x][y+1].value;
        bestMove = (current_orientation==NORTH)?(FORWARD):((current_orientation==EAST)?(LEFT):((current_orientation==SOUTH)?(BACK):RIGHT));  // North
    }
    if (x < MAZE_SIZE-1 && !maze[x][y].walls[1] && maze[x+1][y].value < minValue) {
        minValue = maze[x+1][y].value;
        bestMove = (current_orientation==NORTH)?(RIGHT):((current_orientation==EAST)?(FORWARD):((current_orientation==SOUTH)?(LEFT):BACK));  // East
    }
    if (y > 0 && !maze[x][y].walls[2] && maze[x][y-1].value < minValue) {
        minValue = maze[x][y-1].value;
        bestMove = (current_orientation==NORTH)?(BACK):((current_orientation==EAST)?(RIGHT):((current_orientation==SOUTH)?(FORWARD):LEFT));  // South
    }
    if (x > 0 && !maze[x][y].walls[3] && maze[x-1][y].value < minValue) {
        minValue = maze[x-1][y].value;
        bestMove = (current_orientation==NORTH)?(LEFT):((current_orientation==EAST)?(BACK):((current_orientation==SOUTH)?(RIGHT):FORWARD));  // West
    }
    return bestMove;
}



Action solver() {
    if(cur_x==-1) {
        initializeMaze();
        cur_x = cur_y = 0;
        current_orientation = NORTH;
    }
    int x = cur_x, y = cur_y;  // Starting position at the bottom left corner
    discoverWalls(x, y);
    updateVisualization(x, y);
    if(maze[cur_x][cur_y].value == 0) {
        API_setColor(x, y, 'g');
        return IDLE;
    }
    Action nextMove = decideNextMove(x, y);
    if(nextMove == STOP) {
        upfloodfill(x, y);
        return solver();
    }
    else if(nextMove == BACK || nextMove == RIGHT) {
        switch (current_orientation) {
            case NORTH:
                current_orientation = EAST;
                break;
            case EAST:
                current_orientation = SOUTH;
                break;
            case SOUTH:
                current_orientation = WEST;
                break;
            case WEST:
                current_orientation = NORTH;
                break;
            default:
                break;
        }
        return RIGHT;
    }
    else if(nextMove == LEFT) {
        switch (current_orientation) {
            case NORTH:
                current_orientation = WEST;
                break;
            case EAST:
                current_orientation = NORTH;
                break;
            case SOUTH:
                current_orientation = EAST;
                break;
            case WEST:
                current_orientation = SOUTH;
                break;
            default:
                break;
        }
        return LEFT;
    }
    else if(nextMove == FORWARD) {
        switch (current_orientation) {
            case NORTH:
                cur_y++;
                break;
            case EAST:
                cur_x++;
                break;
            case SOUTH:
                cur_y--;
                break;
            case WEST:
                cur_x--;
                break;
            default:
                break;
        }
        return FORWARD;
    }
    return IDLE;
}