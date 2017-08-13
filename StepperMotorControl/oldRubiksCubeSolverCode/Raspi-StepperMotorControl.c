/**
 * Austin Chun
 * Stepper Mootor control with Raspberry Pi
 * 
 * Can be used for Rubik's Cube or Self-driving car 
 * 
 */

#include "Raspi-StepperMotorControl.h"

///////////////////////////////////////////////////////////////////////
////////////////////// Create Random Moves ////////////////////////////
///////////////////////////////////////////////////////////////////////

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

  // Determine types for each move
  for(i = 0; i < randMoveNum; ++i){
    // Last move
    if(i == randMoveNum-1) randType[i] = 2; 
    // Next move is opposite motor
    else if (randFaces[i] == (randFaces[i+1]+3)%6) randType[i] = 2;
    // All other moves
    else randType[i] = (randDir[i] == randDir[i+1]);
  }
}

void randomMixUp(enum Function fn){
  int i = 0; // Cause C is stupid

  int nextMotor = -1;

  for(i = 0; i < randMoveNum; ++i){
    switch(fn){
      case SLOW:
        stepSlow      (randFaces[i], randDir[i], randSteps[i]); break;
      case FASTISH:
        stepFastish   (randFaces[i], randDir[i], randType[i]); break;
      case FASTISHER: 
        stepFastisher (randFaces[i], randDir[i], randType[i]); break;
      case STEPSYM:
        stepSym       (randFaces[i], randDir[i], randType[i]); break;
      case FAST:
        stepFast      (randFaces[i], randDir[i], randType[i]); break;
      case MIDLOCK:
        if(i == randMoveNum-1) nextMotor = -1;
        else                   nextMotor = randFaces[i+1];
        stepMidLock   (randFaces[i], randDir[i], randType[i], nextMotor); break;
      case LOCKSYM:
        if(i == randMoveNum-1) nextMotor = -1;
        else                   nextMotor = randFaces[i+1];
        stepLockSym   (randFaces[i], randDir[i], randType[i], nextMotor); break;  
      case LINACCLOCKSYM:
        if(i == randMoveNum-1) nextMotor = -1;
        else                   nextMotor = randFaces[i+1];
        stepLinAccLockSym (randFaces[i], randDir[i], randType[i], nextMotor); break;      
      default:
        break;
    }
  }
}

void randomSolve(enum Function fn){
  int i = 0; // Cause C is stupid

  int nextMotor = -1;

  for(i = randMoveNum - 1; i >= 0; --i){
    switch(fn){
      case SLOW:
        stepSlow      (randFaces[i], (randDir[i]+1)%2, randSteps[i]); break;
      case FASTISH:
        stepFastish   (randFaces[i], (randDir[i]+1)%2, randType[i]); break;
      case FASTISHER: 
        stepFastisher (randFaces[i], (randDir[i]+1)%2, randType[i]); break;
      case STEPSYM:
        stepSym       (randFaces[i], (randDir[i]+1)%2, randType[i]); break;
      case FAST:
        stepFast      (randFaces[i], (randDir[i]+1)%2, randType[i]); break;
      case MIDLOCK:
        if(i == 0) nextMotor = -1;
        else       nextMotor = randFaces[i+1];
        stepMidLock   (randFaces[i], (randDir[i]+1)%2, randType[i], nextMotor); break;
      case LOCKSYM:
        if(i == 0) nextMotor = -1;
        else       nextMotor = randFaces[i+1];
        stepLockSym   (randFaces[i], (randDir[i]+1)%2, randType[i], nextMotor); break; 
      case LINACCLOCKSYM:
        if(i == 0) nextMotor = -1;
        else       nextMotor = randFaces[i+1];
        stepLinAccLockSym(randFaces[i], (randDir[i]+1)%2, randType[i], nextMotor); break; 
      default:
        break;
    }
    // stepSlow(randFaces[i], (randDir[i]+1)%2 , randSteps[i]);
  }
}

///////////////////////////////////////////////////////////////////////
//////////////////////// Helper Methods ///////////////////////////////
///////////////////////////////////////////////////////////////////////

/**
 * @brief      case-switch statement to designate appropriate stepPin and sleepPin
 *
 * @param[in]  motor  The motor number
 *
 * @return     struct with stepPin and sleepPin
 */
pins choosePins(int motor) {
  int stepPin;
  int sleepPin;
  // Define pins to use
  switch (motor) {
    case 0:
      stepPin = stepPin0; sleepPin = sleepPin0;
      break;
    case 1:
      stepPin = stepPin1; sleepPin = sleepPin1;
      break;
    case 2:
      stepPin = stepPin2; sleepPin = sleepPin2;
      break;
    case 3:
      stepPin = stepPin3; sleepPin = sleepPin3;
      break;
    case 4:
      stepPin = stepPin4; sleepPin = sleepPin4;
      break;
    case 5:
      stepPin = stepPin5; sleepPin = sleepPin5;
      break;
    default:
      stepPin = -1; sleepPin = -1; // Undefined
      break;
  }
  pins p = {stepPin, sleepPin};
  return p;
}

/**
 * @brief      Locks specific motors mid-turn.
 *
 * @param[in]  notThisMotor  Not this motor
 * @param[in]  dir           HIGH or LOW
 */
void lockMotorsMidTurn(int motor1, int motor2, int motor3, int state){
  int i = 0; // Cause C is stupid

  int sleepPin = -1;
  // Loop through all "motor indexes"
  for(i = 0; i < 6; ++i){
    // Skip the specified motor
    if(i != motor1 && i != motor2 && i != motor3){
      sleepPin = i*2 + 2 + 1;
      digitalWrite(sleepPin, state);
    }
  }
}

///////////////////////////////////////////////////////////////////////
//////////////////////// Step Functions ///////////////////////////////
///////////////////////////////////////////////////////////////////////

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
  int sleepPin = p.sleepPin;
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
  digitalWrite(sleepPin, HIGH);
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
    else if(deay > 100){
      deay = deay + 2;
    }

  }

  delayMillis(1);

  // Turn off motor(s)  
  digitalWrite(sleepPin, LOW);
}

void stepFastish(int motor, int dir, int type) {
    int i = 0; // Cause C is stupid

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;
 // int stepNum;
  pins q = choosePins((motor+3)%6);


  int accelTurns = 0;
  int decelTurns = 0;

  // 0 indicates change in direction, 1 indicates same direction, 2 signifies end
  if(type == 0) {accelTurns = 36; decelTurns = 10;}
  else if(type == 1) {accelTurns = 37; decelTurns = 10;}
  else if(type == 2) {accelTurns = 39; decelTurns = 10;}



  // Turn motor on
  digitalWrite(sleepPin, HIGH);
  digitalWrite(q.sleepPin, HIGH); // lock opposite motor too
  // Set direction
  digitalWrite(dirPin, dir);
  delayMillis(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_a = 16.0;//20.0; // integration constant, corresponds to initial deay = 500
  const float accel = 2500.0; // [Turns per sec^2]
  const float decel = 11000.0;

  const float maxDeay = 600.0;

  // Acceleration phase
  for (i = 0; i < accelTurns; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(accel*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Serial.println((int)deay);
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;

  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay
  
  // Deceleration phase
  for (i = 0; i < decelTurns; i++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }

    // Serial.println((int)deay);
    digitalWrite(stepPin,HIGH); delayMicros((int)deay);
    digitalWrite(stepPin,LOW); delayMicros((int)deay);
  }


  // Turn off motor
  // if(type == 2) delayMillis(100);
  delayMillis(10); //delayMillis(7); // 7 is pretty sketch, but can work
  digitalWrite(sleepPin, LOW);
  digitalWrite(q.sleepPin, LOW);
}

void stepFastisher(int motor, int dir, int type) {
  int i = 0; // Cause C is stupid

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;
  pins q = choosePins((motor+3)%6);

  int startTurns = 0;
  int accelTurns = 0;
  int decelTurns = 0;

  // 0 indicates change in direction, 1 indicates same direction, 2 signifies end
  if(type == 0)      {startTurns = 10; accelTurns = 25; decelTurns = 11;}
  else if(type == 1) {startTurns = 10; accelTurns = 25; decelTurns = 12;}
  else if(type == 2) {startTurns = 10; accelTurns = 27; decelTurns = 13;}

  // Turn motor on
  digitalWrite(sleepPin, HIGH);
  digitalWrite(q.sleepPin, HIGH);
  // Set direction
  digitalWrite(dirPin, dir);
  delayMillis(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_s    = 16.0; // integration constant, corresponds to initial deay = 500
  const float startAccel = 2500.0; // [Turns per sec^2]
  const float accel      = 4500.0;
  const float decel      = 11000.0;
  const float maxDeay    = 600.0;

  // Start phase (slow-ish acceleration)
  for (i = 0; i < startTurns; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(startAccel*pow(10,-6)*t_n + plusC_s) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  // Reset timer for acceleration
  t_n = 0;
  const float plusC_a = pow(10,4) / deay; // Calculate integration constant from current deay
  
  // Acceleration phase
  for (i = 0; i < accelTurns; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(accel*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay

  // Deceleration phase
  for (i = 0; i < decelTurns; i++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }

    digitalWrite(stepPin,HIGH); delayMicros((int)deay);
    digitalWrite(stepPin,LOW); delayMicros((int)deay);
  }

  // Turn off motor
  delayMillis(10);
  digitalWrite(sleepPin, LOW);
  digitalWrite(q.sleepPin, LOW);
}

void stepSym(int motor, int dir, int type) {
    int i = 0; // Cause C is stupid

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;
  pins q = choosePins((motor+3)%6);

  int slow = 0;
  int fast = 0;

  // 0 indicates change in direction, 1 indicates same direction, 2 signifies end
  if(type == 0)      {slow = 10; fast = 15;}
  else if(type == 1) {slow = 10; fast = 15;}
  else if(type == 2) {slow = 10; fast = 15;}

  // Turn motor on
  digitalWrite(sleepPin, HIGH);
  digitalWrite(q.sleepPin, HIGH);
  // Set direction
  digitalWrite(dirPin, dir);
  delayMillis(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_s  = 16.0; // integration constant, corresponds to initial deay = 500
  const float slowA    = 2500.0; // [Turns per sec^2]
  const float fastA    = 4500.0;

  // Start phase (slow-ish acceleration)
  for (i = 0; i < slow; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(slowA*pow(10,-6)*t_n + plusC_s) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  // Reset timer for acceleration
  t_n = 0;
  const float plusC_a = pow(10,4) / deay; // Calculate integration constant from current deay
  
  // Acceleration phase
  for (i = 0; i < fast; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(fastA*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d1 = pow(10,4) / deay; // Calculate integration constant from current deay

  // Deceleration phase
  for (i = 0; i < fast; i++){
    t_n_1 = 10000/(-fastA*pow(10,-6)*t_n + plusC_d1) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1;

    digitalWrite(stepPin,HIGH); delayMicros((int)deay);
    digitalWrite(stepPin,LOW); delayMicros((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d2 = pow(10,4) / deay; // Calculate integration constant from current deay

  // Deceleration phase
  for (i = 0; i < slow; i++){
    t_n_1 = 10000/(-slowA*pow(10,-6)*t_n + plusC_d2) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1;

    digitalWrite(stepPin,HIGH); delayMicros((int)deay);
    digitalWrite(stepPin,LOW); delayMicros((int)deay);
  }

  // Turn off motor
  delayMillis(10);
  digitalWrite(sleepPin, LOW);
  digitalWrite(q.sleepPin, LOW);
}

void stepMidLock(int motor, int dir, int type, int next) {
    int i = 0; // Cause C is stupid

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  int startTurns = 10; 
  int accelTurns = 20; 
  int decelTurns = 20;
  // 0 indicates change in direction, 1 indicates same direction, 2 signifies end
  // if(type == 0)      {startTurns = 10; accelTurns = 25; decelTurns = 11;}
  // else if(type == 1) {startTurns = 10; accelTurns = 25; decelTurns = 12;}
  // else if(type == 2) {startTurns = 10; accelTurns = 25; decelTurns = 12;}

  // Turn motor on
  digitalWrite(sleepPin, HIGH);  
  // Set direction
  digitalWrite(dirPin, dir);
  delayMillis(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_s    = 16.0; // integration constant, corresponds to initial deay = 500
  const float startAccel = 1500.0; // [Turns per sec^2]
  const float accel      = 2500.0;
  const float decel      = 4000.0;
  const float maxDeay    = 600.0;

  /// Start phase (slow-ish acceleration)
  for (i = 0; i < startTurns; i++) {
    t_n_1 = 10000/(startAccel*pow(10,-6)*t_n + plusC_s) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; 
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  // *MOVED TO 20th STEP*  Engage other motors (skip current motor, since already on)
  //lockMotorsMidTurn(-1, motor, HIGH);

  // Reset timer for acceleration
  t_n = 0;
  const float plusC_a = pow(10,4) / deay; // Calculate integration constant from current deay
  
  /// Acceleration phase
  for (i = 0; i < accelTurns; i++) {
    // Lock all motors
    if(i == 5) lockMotorsMidTurn(motor, -1, -1, HIGH);
    
    t_n_1 = 10000/(accel*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; 
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay

  /// Deceleration phase
  for (i = 0; i < decelTurns; i++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }
    digitalWrite(stepPin,HIGH); delayMicros((int)deay);
    digitalWrite(stepPin,LOW); delayMicros((int)deay);
  }


  delayMillis(10);

  // Turn all motors off, except current and next motor
  lockMotorsMidTurn(-1, next, (next+3)%6, LOW);
}

void stepLockSym(int motor, int dir, int type, int next) {
    int i = 0; // Cause C is stupid

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  int accelTurns = 0;
  int accelTurns2 = 0;
  int decelTurns = 0;
  int decelTurns2 = 0;

  if(type == 0)      {accelTurns = 10; accelTurns2 = 15; decelTurns = 25; decelTurns2 = 0;}
  else if(type == 1) {accelTurns = 10; accelTurns2 = 16; decelTurns = 25; decelTurns2 = 0;}
  else if(type == 2) {accelTurns = 10; accelTurns2 = 15; decelTurns = 25; decelTurns2 = 0;}

  // Turn motor on
  digitalWrite(sleepPin, HIGH);  
  // Set direction
  digitalWrite(dirPin, dir);
  // delayMillis(2);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_s    = 16.0; // integration constant, corresponds to initial deay = 500
  const float accel1    = 2000.0; // [Turns per sec^2]
  const float accel2    = 4500.0;
  const float decel1    = 4500.0; // [Turns per sec^2]
  const float decel2    = 2000.0;

  const float maxDeay = 800.0;

  /// Start phase (slow-ish acceleration)
  for (i = 0; i < accelTurns; i++) {
    t_n_1 = 10000/(accel1*pow(10,-6)*t_n + plusC_s) + t_n; 
    deay = t_n_1 - t_n;

    t_n = t_n_1; 
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  // *MOVED TO 20th STEP*  Engage other motors (skip current motor, since already on)
  //lockMotorsMidTurn(-1, motor, HIGH);

  // Reset timer for acceleration
  t_n = 0;
  const float plusC_a = pow(10,4) / deay; // Calculate integration constant from current deay
  t_n_1 = 10000/(accel2*pow(10,-6)*t_n + plusC_a) + t_n; 
  t_n = t_n_1; 

 /// Acceleration phase
  for (i = 0; i < accelTurns2; i++) {
    
    t_n_1 = 10000/(accel2*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; 
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  lockMotorsMidTurn(motor, -1, -1, HIGH);


  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay
  t_n_1 = 10000/(-decel1*pow(10,-6)*t_n + plusC_d) + t_n; 
  t_n = t_n_1;

 /// Deceleration phase
  for (i = 0; i < decelTurns; i++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel1*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }
    digitalWrite(stepPin,HIGH); delayMicros((int)deay);
    digitalWrite(stepPin,LOW); delayMicros((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d2 = pow(10,4) / deay; // Calculate integration constant from current deay
  t_n_1 = 10000/(-decel2*pow(10,-6)*t_n + plusC_d2) + t_n; 
  t_n = t_n_1;

 /// Deceleration phase
  for (i = 0; i < decelTurns2; i++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel2*pow(10,-6)*t_n + plusC_d2) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }
    digitalWrite(stepPin,HIGH); delayMicros((int)deay);
    digitalWrite(stepPin,LOW); delayMicros((int)deay);
  }

  // delayMillis(10);

  // Turn all motors off, except current and next motor
  int oppNext;
  if(next == -1) oppNext = -1;
  else           oppNext = (next+3)%6;
  lockMotorsMidTurn(-1, next, oppNext, LOW);
}

void stepLinAccLockSym(int motor, int dir, int type, int next){
  
  int i = 0; // Cause C is stupid
  
  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, dir);
  delayMillis(1);

 ////////////////////////////////////////////////// 
 /// User Defined Variables!!! ///
 /////////////////////////////////
  int accelTurns = 20;
  int transTurns = 10;
  int decelTurns = 20;

  const float a1 = 0.40    * pow(10,-6);
  const float a2 = -2.9   * pow(10,-6);
  const float a3 = 0.2   * pow(10,-6);

  const float b1 = 10     * pow(10,-6);
  const float c1 = 16.0;

 /////////////////////////////////////////////////

 // Variables
  float t_n = 0;
  float t_n_1 = 0;
  float deay = 0;
  int numTurns = accelTurns + transTurns + decelTurns;

 /// Acc phase
  for (i = 0; i < accelTurns; i++) {
    t_n_1 = 10000/(a1*pow(t_n,2) + b1*t_n + c1) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; 

    // Serial.println(deay);

    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);

    // if(i == 10) lockMotorsMidTurn(motor, -1, -1, HIGH);
  }

  // Serial.println("end 1\n\n");
 /// Trans phase 
  const float b2 = 2*(a1-a2)*t_n + b1;
  const float c2 = (a1-a2)*pow(t_n,2) + (b1-b2)*t_n + c1;

  for (i = 0; i < transTurns; i++) {
    t_n_1 = 10000/(a2*pow(t_n,2) + b2*t_n + c2) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; 
    digitalWrite(stepPin, HIGH); delayMicros((int)deay);
    digitalWrite(stepPin, LOW);  delayMicros((int)deay);

    if(i == 5) lockMotorsMidTurn(motor, -1, -1, HIGH);
  }

 /// Deceleration Phase
  const float b3 = 2*(a2-a3)*t_n + b2;
  const float c3 = (a2-a3)*pow(t_n,2) + (b2-b3)*t_n + c2;

  for (i = 0; i < decelTurns; i++) {
      t_n_1 = 10000/(a3*pow(t_n,2) + b3*t_n +c3) + t_n;
      deay = t_n_1 - t_n;
      t_n = t_n_1;

      // Serial.println(deay);

      digitalWrite(stepPin, HIGH); delayMicros((int)deay);
      digitalWrite(stepPin, LOW);  delayMicros((int)deay);
  }

  if(type == 2) delayMillis(20);
  // else delayMillis(1);
  
  // Turn all motors off, except current and next motor
  int oppNext;
  if(next == -1) oppNext = -1;
  else           oppNext = (next+3)%6;
  lockMotorsMidTurn(-1, next, oppNext, LOW);
  // Serial.println("\nend \n");
}


//////////////////////////////////////////////////////////////
//////////////////////// Setup ///////////////////////////////
//////////////////////////////////////////////////////////////

void setup(){
    // Set all pins as outputs
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(microStep, OUTPUT);

  pinMode(stepPin0, OUTPUT);
  pinMode(sleepPin0, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(sleepPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(sleepPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(sleepPin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(sleepPin4, OUTPUT);
  pinMode(stepPin5, OUTPUT);
  pinMode(sleepPin5, OUTPUT);


  // Start with motor off
  digitalWrite(sleepPin0, LOW);
  digitalWrite(sleepPin1, LOW);
  digitalWrite(sleepPin2, LOW);
  digitalWrite(sleepPin3, LOW);
  digitalWrite(sleepPin4, LOW);
  digitalWrite(sleepPin5, LOW);

  digitalWrite(microStep, LOW);

  // Randomly seed with analog read to unconnected pin
  srand(time(NULL));
  generateRandomMoves();
}


/////////////////////////////////////////////////////////////
//////////////////////// Main ///////////////////////////////
/////////////////////////////////////////////////////////////

void main(void){
    // Initialize GPIO
    pioInit();

    // Setup pins
    setup();

    printf("3...\n");
    delayMillis(1000);
    printf("2...\n");
    delayMillis(1000);
    printf("1...\n");
    delayMillis(1000);
    printf("Moving\n");

 //////////////////////////////////
 /// Test Accelerating Function ///
 //////////////////////////////////


  // 0 = different dirs
  // 1 = same dir

 /// Fast-ish Solve

    // stepFastish(2, LOW, 0);
    // stepFastish(1, HIGH, 1);
    // stepFastish(2, HIGH, 0);
    // stepFastish(1, LOW, 2);



    // for(int i = 0; i < 5; ++i){
    //   stepFastish(2, LOW, 2);
    //   stepFastish(5, LOW, 0);
    //   stepFastish(0, HIGH, 2);
    //   stepFastish(3, HIGH, 0);
    //   stepFastish(1, LOW, 0);
    //   stepFastish(3, HIGH, 0);
    //   stepFastish(4, LOW, 1);
    //   stepFastish(0, LOW, 1);
    // }

    // for(int i = 0; i < 8; ++i){
    //   stepFastish(2, LOW, 0);
    //   delay(100);
    // }

 /// Fast-ish-er solve

    // for(int i = 0; i < 6; ++i){
    //   stepFastisher(2,HIGH, 1);
    //   // delay(100);
    //   stepFastisher(1,HIGH, 1);
    //   // delay(100);
    // }

    // stepFastisher(2, LOW, 1);
    // stepFastisher(1, LOW, 0);
    // stepFastisher(2, HIGH, 1);
    // stepFastisher(1, HIGH, 2);

    // for(int i = 0; i < 12; ++i){
    //   stepFastisher(2, LOW, 1);
    //   stepFastisher(1, LOW, 0);
    //   stepFastisher(2, HIGH, 1);
    //   stepFastisher(1, HIGH, 0);
    // }

    // for(int i = 0; i < 5; ++i){
    //   stepFastisher(2, LOW, 2);
    //   stepFastisher(5, LOW, 0);
    //   stepFastisher(0, HIGH, 2);
    //   stepFastisher(3, HIGH, 0);
    //   stepFastisher(1, LOW, 0);
    //   stepFastisher(3, HIGH, 0);
    //   stepFastisher(4, LOW, 1);
    //   stepFastisher(0, LOW, 1);
    // }

 /// Step Symmetric

    // for (int i = 0; i < 4; i++){
    //   stepSym(0,HIGH,1);
    //   stepSym(1,HIGH,1);
    //   stepSym(2,HIGH,1); 
    // }

    // for(int i = 0; i < 6; ++i){
    //   stepSym(2, LOW, 1);
    //   stepSym(1, LOW, 0);
    //   stepSym(2, HIGH, 1);
    //   stepSym(1, HIGH, 0);
    // }

 /// MidLock Solve
    // for(int i = 0; i < 6; ++i){
    //   stepMidLock(2, HIGH, 1, 1);
    //   // delay(100);
    //   stepMidLock(1, HIGH, 1, 2);
    //   // delay(100);
    // }
    // lockMotorsMidTurn(-1, -1, -1, LOW);

    // for(int i = 0; i < 12; ++i){
    //   stepMidLock(2, LOW, 1, 1);
    //   stepMidLock(1, LOW, 0, 2);
    //   stepMidLock(2, HIGH, 1, 1);
    //   stepMidLock(1, HIGH, 0, 2);
    // }
    // lockMotorsMidTurn(-1, -1, -1, LOW);

 /// Step Lock Sym

    // stepLockSym(1,LOW, 1, 2);
    // stepLockSym(2,LOW, 2, -1);
    // lockMotorsMidTurn(-1,-1,-1,LOW);
    // delay(2000);

    // stepLockSym(1,LOW, 0, 1);
    // stepLockSym(2,HIGH, 2, -1);
    // lockMotorsMidTurn(-1,-1,-1,LOW);

    // for(int i = 0; i < 6; ++i){
    //   stepLockSym(2, LOW, 1, 1);
    //   stepLockSym(1, LOW, 0, 2);
    //   stepLockSym(2, HIGH, 1, 1);
    //   stepLockSym(1, HIGH, 0, 2);
    // }
    // lockMotorsMidTurn(-1, -1, -1, LOW);

    // delay(3000);

    // for(int i = 0; i < 10; ++i){
    //   stepLockSym(0, HIGH, 1, 1);
    //   stepLockSym(1, HIGH, 1, 2);
    //   stepLockSym(2, HIGH, 1, 0);
    // }
    // lockMotorsMidTurn(-1, -1, -1, LOW);

 /// Step LinAccLockSym

    // stepLinAccLockSym(1,LOW, 1, 2);
    // stepLinAccLockSym(2,LOW, 2, -1);
    // lockMotorsMidTurn(-1,-1,-1,LOW);
    // delay(2000);

    // stepLinAccLockSym(1,LOW, 0, 2);
    // stepLinAccLockSym(2,HIGH, 2, -1);
    // lockMotorsMidTurn(-1,-1,-1,LOW);

    // for(int i = 0; i < 6; ++i){
    //   stepLinAccLockSym(2, LOW, 1, 1);
    //   stepLinAccLockSym(1, LOW, 0, 2);
    //   stepLinAccLockSym(2, HIGH, 1, 1);
    //   stepLinAccLockSym(1, HIGH, 0, 2);
    // }
    // lockMotorsMidTurn(-1, -1, -1, LOW);

    // delay(3000);

    // for(int i = 0; i < 10; ++i){
    //   stepLinAccLockSym(0, HIGH, 1, 1);
    //   stepLinAccLockSym(1, HIGH, 1, 2);
    //   stepLinAccLockSym(2, HIGH, 1, 0);
    // }
    // lockMotorsMidTurn(-1, -1, -1, LOW);

 /// Fastest Solve
    // for(int i = 0; i < 10; ++i){
    //   stepFast(2, LOW, 0);
    //   delay(100);
    // }


 ////////////////////////////////
 /// Solve with Step Function ///
 //////////////////////////////// 


    // Random mix and solve
    digitalWrite(microStep, HIGH);
    randomMixUp(SLOW);
    lockMotorsMidTurn(-1, -1, -1, LOW); 

    delay(5000);

    randomSolve(SLOW);
    lockMotorsMidTurn(-1, -1, -1, LOW);  
}

