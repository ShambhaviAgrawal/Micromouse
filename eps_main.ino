#include <stdio.h>
#include "solver.h"
#include "API.h"
#include "queue.h"
//#include "solver.c"
//#include "API.c"
//#include "queue.c"
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define IR1 18
#define IR2 5
#define IR3 19

const int c1 = 2;
const int c2 = 3;
const int c3 = 4;
const int c4 = 5;

//Inputs to motor driver, outputs from the board
const int in1 = 8;    
const int in2 = 9;
const int in3 = 10;
const int in4 = 11;

Action nextMove;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(c1,   INPUT);
  pinMode(c2,   INPUT);
  pinMode(c3,   INPUT);
  pinMode(c4,   INPUT);

  digitalWrite(in1,  LOW);
  digitalWrite(in2,  LOW);
  digitalWrite(in3,  LOW);
  digitalWrite(in4,  LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print("Sensing: ");
  // if(digitalRead(pin)==HIGH)
  //   Serial.println(1);
  // else
  //   Serial.println(0);
  // delay(500);
  nextMove = solver();  // Get the next action from the solver
  switch(nextMove) {
    case FORWARD:
      debug_log("Moving forward");
      if (API_moveForward() == 0) {  // Check for failure (e.g., collision)
        debug_log("Move forward failed.");
      }
      break;
    case LEFT:
      debug_log("Turning Left");
      API_turnLeft();
      break;
    case RIGHT:
      debug_log("Turning Right");
      API_turnRight();
      break;
    default:
      break;
  }
}
