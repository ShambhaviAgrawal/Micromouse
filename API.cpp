#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arduino.h"

#define BUFFER_SIZE 32
#define IR1 18
#define IR2 5
#define IR3 19

const int in1 = 8;    
const int in2 = 9;
const int in3 = 10;
const int in4 = 11;

#define MAZE_SIZE 16
int getInteger(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int value = atoi(response);
    return value;
}

int getBoolean(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int value = (strcmp(response, "true\n") == 0);
    return value;
}

int getAck(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int success = (strcmp(response, "ack\n") == 0);
    return success;
}

int API_mazeWidth() {
    return MAZE_SIZE;
}

int API_mazeHeight() {
    return MAZE_SIZE;
}

int API_wallFront() {
    if(digitalRead(IR2)==LOW)
      return 1;
    else
      return 0;
}

int API_wallRight() {
    if(digitalRead(IR3)==LOW)
      return 1;
    else
      return 0;
}

int API_wallLeft() {
    if(digitalRead(IR1)==LOW)
      return 1;
    else
      return 0;
}

void API_moveForward() {
    if(API_wallRight() and API_wallLeft())
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
}

void API_turnRight() {
    if(API_wallFront() and API_wallLeft())
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      delay(5000);
}

void API_turnLeft() {
    if(API_wallFront() and API_wallRight())
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      delay(5000);
}

void API_setWall(int x, int y, char direction) {
    return;
}

void API_clearWall(int x, int y, char direction) {
    return;
}

void API_setColor(int x, int y, char color) {
    return;
}

void API_clearColor(int x, int y) {
    return;
}

void API_clearAllColor() {
    return;
}
/// @brief 
/// @param x 
/// @param y 
/// @param text 
void API_setText(int x, int y, char* text) {
    return;
}

void API_clearText(int x, int y) {
    return;
}

void API_clearAllText() {
    return;
}

int API_wasReset() {
    return getBoolean("wasReset");
}

void API_ackReset() {
    getAck("ackReset");
}

void debug_log(char* text) {
    return;
}
