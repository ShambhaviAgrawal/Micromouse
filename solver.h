#ifndef SOLVER_H
#define SOLVER_H

typedef enum Heading {NORTH, EAST, SOUTH, WEST} Heading;
typedef enum Action {LEFT, FORWARD, RIGHT, BACK, STOP,IDLE} Action;

Action leftWallFollower();
void initializeMaze();
void discoverWalls(int x, int y);  // Confirm this is declared
void floodfill();  // This needs to be declared if it's used in solver.c
void updateVisualization(int x, int y);
Action decideNextMove(int x, int y);
Action solver();

#endif