/**
 * Austin Chun
 * Stepper Mootor control with Raspberry Pi
 * 
 * Can be used for Rubik's Cube or Self-driving car 
 * 
 */

/*
g++ -g -std=c++11 -Wall -Wextra camera-SlowStepperMotorControl.cpp  -c -o camera-SlowStepperMotorControl.o -I/usr/local/include/opencv2 -I/usr/local/include; g++ -o Camera-SlowStepperMotorControl.exe camera-SlowStepperMotorControl.o -I/usr/local/include/opencv2 -I/usr/local/include  -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs
*/
/*
g++ -g -std=c++11 -Wall -Wextra camera-SlowStepperMotorControl.cpp  -c -o camera-SlowStepperMotorControl.o; 
g++ -o Camera-SlowStepperMotorControl.exe camera-SlowStepperMotorControl.o -I/usr/local/include/opencv2
*/

extern "C"{
  #include "StepperMotorController.h"
}

#include <stdio.h>
#include <unistd.h>
// #include "ImageProcess/train.hpp"

void stepSlow(int motor, int dir, int turns);

///////////////////////////////////////////////////////////////////////
////////////////////// Create Random Moves ////////////////////////////
///////////////////////////////////////////////////////////////////////
#define randMoveNum 40
int randFaces[randMoveNum];
int randDir[randMoveNum];
int randSteps[randMoveNum];

void generateRandomMoves(){
  int i = 0; // Cause C is stupid
  int prevMotor = -1;
  for(i = 0; i < randMoveNum; ++i){
    // Choose random motor (not the same as previous)
    randFaces[i] = (prevMotor + rand()%5 + 1) % 6;
    prevMotor = randFaces[i];
    // Choose random direction and step number
    randDir[i] = rand()%2;
    randSteps[i] = rand()%2 + 1;
  }
}

void randomMixUp(){
  int i = 0; 
  for(i = 0; i < randMoveNum; ++i){
    stepSlow(randFaces[i], randDir[i], randSteps[i]);
  }
}

void randomSolve(){
  int i = 0; 
  for(i = randMoveNum - 1; i >= 0; --i){
    stepSlow(randFaces[i], (randDir[i]+1)%2, randSteps[i]);
  }
}




/**
 * @brief      Slow stepping function (should always work)
 *
 * @param[in]  motor  The motor
 * @param[in]  dir    The dir
 * @param[in]  turns  The turns
 * 
 * @note ~0.1s per quarter turn
 */
void stepSlow(int motor, int dir, int turns) {
  int i = 0; // Cause C is stupid

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int disablePin = p.disablePin;
  int stepNum;
  int accelSteps;

  // Define num steps and num accelSteps
  if(turns == 1) {
    stepNum = 50;
    accelSteps = 40;
  }
  else{
    stepNum = 100;
    accelSteps = 85;
  }
  
  // Wake up motor
  digitalWrite(disablePin, LOW);
  // Set the direction
  digitalWrite(dirPin, dir);
  delayMillis(1);

  // Play with this
  int deay = 100;

  // Pulse motor stepNum times
  for (i = 0; i < stepNum*32 - 4; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros(deay);
    digitalWrite(stepPin, LOW);  delayMicros(deay);

    if(deay > 25 && i < accelSteps*32){
      deay = deay - 1;
    }
    else if(deay < 100 && i > accelSteps*32){
      deay = deay + 2;
    }
    // printf("%d\n", deay);
  }

  delayMillis(1);

  // Turn off motor(s)  
  digitalWrite(disablePin, HIGH);
}





/////////////////////////////////////////////////////////////
//////////////////////// Main ///////////////////////////////
/////////////////////////////////////////////////////////////

int main(void){
    // Initialize GPIO
    pioInit();
    // Setup pins
    setup();

    // // Setup Video ports
    // VideoCapture v1(PORT1);
    // VideoCapture v2(PORT2);

    // printf("Flushing camera\n");
    // int i = 0;
    // for(i = 0; i < 10; i++){
    //   printf("0.%d\n", i);
    //   delayMillis(500);
    //   snap(100+i-4, v1, v2);
    //   delayMillis(1500);
    // }

    // for(i = 0; i < 4; i++){
    //   printf("%d\n", 10+i);
    //   delayMillis(500);
    //   snap(106+i, v1, v2);
    //   delayMillis(1500);
    // }

    // for(i = 0; i < 10; i++){
    //   printf("1.%d\n", i);
    //   delayMillis(500);
    //   snap(900+i, v1, v2);
    //   delayMillis(1500);
    // }


    printf("3...\n");
    sleep(1);
    printf("2...\n");
    sleep(1);
    printf("1...\n");
    sleep(1);
    printf("Moving\n");

    digitalWrite(sleepPin, HIGH); // Turn motors on

    // // int i = 0;
    // int count = 0;
    // int rm, rd, rs;
    // int prevMotor = 0;
    // // char key;
    srand(time(NULL));


    // // int numMoves = 20;

    // // for(count = 0; count < numMoves; count ++){
    // while(true){

    //   rm = rand();
    //   rd = rand();
    //   rs = rand();
    //   printf("%d: Motor: %d, Dir: %d, Turns: %d\n",count, (rm%5 + prevMotor + 1)%6, rd%2, (rs%2)+1);

    //   stepSlow((rm%5+prevMotor+1)%6, rd%2, (rs%2)+1);

    //   prevMotor = (rm%5 + prevMotor + 1)%6;
    //   // printf("Turn NOW\n");
    //   delayMillis(200);
    //   // snap(count-4, v1, v2);
    //   // delayMillis(250);
    //   count++;

    // }

    generateRandomMoves();
    randomMixUp();
    delayMillis(1000);
    randomSolve();

    // for(i = 0; i < 4; i++){
    //   printf("%d\n", numMoves+i);
    //   delayMillis(250);
    //   // snap(numMoves - 4 + i, v1, v2);
    //   delayMillis(250);
    // }


    // printf("Count: %d\n", count);
    // Must KEEP (saves power and sounds better)
    digitalWrite(sleepPin, LOW);

    return 0;
}



