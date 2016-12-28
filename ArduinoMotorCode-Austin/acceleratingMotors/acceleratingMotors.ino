/**
 * @file acceleratingMotors
 * @author Austin Chun
 * @date Oct 17, 2016
 * @brief Hello
 * 
 */

/*
  Accelerating Motors

  Accelerates the stepper motors by gradually decreasing pulse width.
  Otherwise same as constantSpeed5Motor

  Implements two stepping functions, stepTest() and stepFast().
      stepTest() should be used to scramble the Rubiks cube. It is a slower 
  stepping function, and thus less prone to mistakes.
      stepFast() should be used for solving. This is currently the fastest 
  version.
      (Also two turn verions of both functions)

  To specify messUp moves, and solution moves, fill in the global variables
  messUpMoves[] and solution[] with Turn enums (R,Li, D2 etc.).


*/


///////////////////////////////////////////////////////////////////////
////////////////////// Define Constants ///////////////////////////////
///////////////////////////////////////////////////////////////////////

// Define Arduino pins
const int dirPin = 0;
const int microStep = 1;
const int stepPin0 = 2;
const int sleepPin0 = 3;
const int stepPin1 = 4;
const int sleepPin1 = 5;
const int stepPin2 = 6;
const int sleepPin2 = 7;
const int stepPin3 = 8;
const int sleepPin3 = 9;
const int stepPin4 = 10;
const int sleepPin4 = 11;
const int stepPin5 = 12;
const int sleepPin5 = 13;

const int randMoveNum = 40;
int randFaces[randMoveNum];
int randDir[randMoveNum];
int randSteps[randMoveNum];
int randType[randMoveNum];



// Define some structs for easy function returns

// Struct for switch-case statement for pin selection
typedef struct {
  int stepPin;
  int sleepPin;
} pins;

// Struct for switch-case statement converting Turns to turnInfo
typedef struct{
  int motor;
  int dir;
  int turns;
} turnInfo;

// Enum of Turns (consistent with Daniel's naming)
enum Turn{
  F, R, B, L, U, D,
  F2, R2, B2, L2, U2, D2,
  Fi, Ri, Bi, Li, Ui, Di
};

enum Function{
  SLOW, FASTISH, FASTISHER, MIDLOCK, FAST, STEPSYM
};

///////////////////////////////////////////////////////////////////////
////////////////////// Define Move Lists //////////////////////////////
///////////////////////////////////////////////////////////////////////

// Store the moves to mess up the cube (as Turn[])
const int messUpLength = 29;
Turn messUpMoves[messUpLength] = 
{F2, D2, L, D2, L, D2, Ri, U2, Ri, D2, 
  R, U2, F2, U2, L, D2, Fi, L, Ui, Di, 
  R, F, Ri, Di, Ui, L, Ri, D, R};
// Initialize turnInfo for mess up moves
turnInfo messUpInfo[messUpLength];

// Store the solution
const int solutionLength = 29;
Turn solution[solutionLength] = 
{Ri, Di, R, Li, U, D, R, Fi, Ri, D, U, 
  Li, F, D2, Li, U2, F2, U2, Ri, D2, R, 
  U2, R, D2, Li, D2, Li, D2, F2};
// Initialize turnInfo for solution
turnInfo solutionInfo[solutionLength];

///////////////////////////////////////////////////////////////////////
////////////////////// Create Random Moves ////////////////////////////
///////////////////////////////////////////////////////////////////////

void generateRandomMoves(){
  int prevMotor = -1;
  for(int i = 0; i < randMoveNum; ++i){
    // Choose random motor (not the same as previous)
    randFaces[i] = (prevMotor + random(5) + 1) % 6;
    prevMotor = randFaces[i];
    // Choose random direction and step number
    randDir[i] = random(2);
    randSteps[i] = random(2) + 1;
  }

  // Determine types for each move
  for(int i = 0; i < randMoveNum; ++i){
    // Last move
    if(i == randMoveNum-1) randType[i] = 2; 
    // Next move is opposite motor
    else if (randFaces[i] == (randFaces[i+1]+3)%6) randType[i] = 2;
    // All other moves
    else randType[i] = (randDir[i] == randDir[i+1]);
  }
}

void randomMixUp(Function fn){
  int nextMotor = -1;

  for(int i = 0; i < randMoveNum; ++i){
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
      default:
        break;
    }
    // stepSlow(randFaces[i], randDir[i], randSteps[i]);
  }
}

void randomSolve(Function fn){
  int nextMotor = -1;

  for(int i = randMoveNum - 1; i >= 0; --i){
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
        else                   nextMotor = randFaces[i+1];
        stepMidLock   (randFaces[i], (randDir[i]+1)%2, randType[i], nextMotor); break;
      default:
        break;
    }
    // stepSlow(randFaces[i], (randDir[i]+1)%2 , randSteps[i]);
  }
}

void printRandomMoves(){
  for(int i = 0; i < randMoveNum; ++i){
    Serial.print(randFaces[i]);
    Serial.print(", ");
  } 
  Serial.print("\n");
  // for(int i = 0; i < randMoveNum; ++i){
  //   Serial.print(randDir[i],", ");
  // } 
  // Serial.print("\n");
  // for(int i = 0; i < randMoveNum; ++i){
  //   Serial.print(randDir[i],", ");
  // } 

  for(int i = randMoveNum - 1; i >= 0; --i){
  // for(int i = 0; i < randMoveNum; ++i){
    Serial.print(randFaces[i]);
    Serial.print(", ");
  }
  Serial.print("\n");


}

///////////////////////////////////////////////////////////////////////
//////////////////////////// Methods //////////////////////////////////
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
  pins p {stepPin, sleepPin};
  return p;
}

/**
 * @brief      executes the moves specified in the messUp moves array
 *             uses stepTest(), which is a slow turning function 
 *             (deay = 600 vs 500)
 */
void stepList(){
  // Change enum Turn to struct w/ useful information
  convertMessUp();
  // next indicates the biasing of a turn (overshoot vs undershoot vs exact)
  int next;

  // Loop through listed moves
  for(int i = 0; i < messUpLength; ++i){
    // If last move, must be exact
    if(i == messUpLength-1) next = 2;
    // If next move is opposite face, must be exact
    else if (messUpInfo[i].motor%3 == messUpInfo[i+1].motor%3) next = 2;
    // Over/under bias turn based on next direction
    else next = (messUpInfo[i].dir == messUpInfo[i+1].dir); 

    // Step the motor
    stepTest(messUpInfo[i].motor, messUpInfo[i].dir, next, messUpInfo[i].turns);
  }
}


/**
 * @brief     executes the moves specified in the solution array
 *            (mimics stepList(), except now with solution instead of messUp)
 *            uses stepFast instead of stepTest
 */
void solve(){
  // Convert enum Turns to struct turnInfo
  convertSolution();
  // next indicates the biasing of a turn (overshoot vs undershoot vs exact)
  int next;

  // Loop through moves
  for(int i = 0; i < solutionLength; ++i){
    // If last move, must be exact
    if(i == solutionLength-1) next = 2;
    // If next move is opposite face, must be exact
    else if (solutionInfo[i].motor%3 == solutionInfo[i+1].motor%3) next = 2;
    // Over/under bias turn based on next direction
    else next = (solutionInfo[i].dir == solutionInfo[i+1].dir);

    // Step the motor
   // stepFast(solutionInfo[i].motor, solutionInfo[i].dir, next, solutionInfo[i].turns);
    Serial.println(next);
  }
}


/**
 * @brief     Constructs and returns a struct turnInfo with motor#, direction, and 
 *            turn data. Big ole switch-case statement
 *
 * @param[in]  t     turn
 *
 * @return     turnInfo with motor number, direction, and "next" value
 */
turnInfo enumToTurnInfo(Turn t){
  int motor;
  int dir;
  int turns;
  switch(t){
    case Fi: motor = 3; dir = HIGH; turns = 1; break;
    case Bi: motor = 0; dir = HIGH; turns = 1; break;
    case Ri: motor = 2; dir = HIGH; turns = 1; break;
    case Li: motor = 5; dir = HIGH; turns = 1; break;
    case Ui: motor = 1; dir = HIGH; turns = 1; break;
    case Di: motor = 4; dir = HIGH; turns = 1; break;

    case F: motor = 3; dir = LOW; turns = 1; break;
    case B: motor = 0; dir = LOW; turns = 1; break;
    case R: motor = 2; dir = LOW; turns = 1; break;
    case L: motor = 5; dir = LOW; turns = 1; break;
    case U: motor = 1; dir = LOW; turns = 1; break;
    case D: motor = 4; dir = LOW; turns = 1; break;

    case F2: motor = 3; dir = LOW; turns = 2; break;
    case B2: motor = 0; dir = LOW; turns = 2; break;
    case R2: motor = 2; dir = LOW; turns = 2; break;
    case L2: motor = 5; dir = LOW; turns = 2; break;
    case U2: motor = 1; dir = LOW; turns = 2; break;
    case D2: motor = 4; dir = LOW; turns = 2; break;

    default: printf("error: Invalid direction input");
      motor = -1; dir = -1; turns = 0; break;
  }
  turnInfo result {motor,dir,turns};
  return result;
}


/**
 * @brief      Converts messUp[] (Turns) to messUpInfo[] (turnInfo)
 */
void convertMessUp(){
  for(int i = 0; i < messUpLength; i++){
    messUpInfo[i] = enumToTurnInfo(messUpMoves[i]);
  }
}

/**
 * @brief      Converts solution[] (Turns) to solutionInfo[] (turnInfo)
 */
void convertSolution(){
  for(int i = 0; i < solutionLength; i++){
    solutionInfo[i] = enumToTurnInfo(solution[i]);
  }
}


/**
 * @brief     Steps the stepper motor 90 degrees, at a slow/controlled rate
 *            considers the next move to appropriately bias current move
 *            (ie, CW->CW or CCW->CCW must bias towards overshooting 90,
 *            while changing direction allows for corner cutting)
 *            Fiddle with:
 *              stepNum- makes smoother or faster transitions
 *              deay- controls the speed of the motor (ie small deay = fast)
 *
 * @param[in]  motor  The motor number
 * @param[in]  dir    The direction
 * @param[in]  next   A boolean used for over/under biasing turn
 * @param[in]  turns  The number of turns (1 or 2)
 */
void stepTest(int motor, int dir, int next, int turns) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;
  int stepNum;

 // Determine appropriate stepNum
  if (next == 1){ // Same direction = overturn
    if(turns == 1) stepNum = 49; // Single turn
    else stepNum = 98;           // Double turn
  }
  else if (next == 0){ // Diff direction = underturn
    if(turns == 1) stepNum = 45;
    else stepNum = 90;
  }
  else { // Exact turn
    if(turns == 1) stepNum = 50;
    else stepNum = 100;
  }

  // Turn on the motor
  digitalWrite(sleepPin, HIGH);
  // Set the direction
  digitalWrite(dirPin, dir);
  delay(1);

  // Play with this
  int deay = 800;

  // Pulse motor stepNum times
  for (int i = 0; i < stepNum; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds(deay);
    // Accelerate (gradually decrease pulse width)
    if(deay > 240){
      deay = deay - 10;
    }
    else if (deay > 220){
      deay = deay - 2;
    }
    else if (deay > 200){
      deay = deay - 1;
    }
  }
  // TODO: FIX THIS
  if(next == 2) delay(100);
  // Turn off motor
  digitalWrite(sleepPin, LOW);
}





/**
 * @brief      test Acceleration function
 *
 * @param[in]  motor  The motor
 * @param[in]  dir    The dir
 */
void stepAccelerationTest (int motor, int dir, int type) {
  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;
 // int stepNum;
  // pins q = choosePins((motor+3)%6);


  int accelTurns = 0;
  int decelTurns = 0;

  // 10 and 11 indicate first turn
  if(type == 10)      {accelTurns = 33; decelTurns = 0;}
  else if(type == 11) {accelTurns = 33; decelTurns = 0;}
  // 0 and 1 indicate simple direction
  else if(type == 0) {accelTurns = 35; decelTurns = 0;}
  else if(type == 1) {accelTurns = 38; decelTurns = 3;}
  // 2 signifies end
  else if(type == 2) {accelTurns = 40; decelTurns = 10;}
  //stepNum = 400; 


  // Turn motor on
  digitalWrite(sleepPin, HIGH);
  // digitalWrite(q.sleepPin, HIGH); // lock opposite motor too
  // Set direction
  digitalWrite(dirPin, dir);
  delay(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 625; //500; // half width of pulse
  const float plusC_a = 16.0;//20.0; // integration constant, corresponds to initial deay = 500
  const float accel = 3500.0; // [Turns per sec^2]
  const float minDeay = 200.0; //135.0;
  const float maxDeay = 500.0;

  // Acceleration phase
  for (int i = 0; i < accelTurns; i++) {
    // Accelerate up till top speed
    if(deay > minDeay){
      // Calculate appropriate deay value
      // Use recurence relation to find next time step
      t_n_1 = 10000/(accel*pow(10,-6)*t_n + plusC_a) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1; // Update for next cycle
    }
    // Serial.println((int)deay);
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;

  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay
  const float decel = 20000.0;
  
  // Deceleration phase
  for (int counter = 0; counter < decelTurns; counter++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }

    // Serial.println((int)deay);
    digitalWrite(stepPin,HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin,LOW); delayMicroseconds((int)deay);
  }


  // Turn off motor
  if(type == 2) delay(100);
  digitalWrite(sleepPin, LOW);
  // digitalWrite(q.sleepPin, LOW);

}

/**
 * @brief      run motor as fast as possible (find top speed)
 *
 * @param[in]  motor  The motor
 * 
 */
void stepTopSpeed(int motor) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  // Turn on the motor
  digitalWrite(sleepPin, HIGH);
  // Set the direction
  digitalWrite(dirPin, HIGH);
  delay(1);

  int deay = 350;
  int minDeay = 150;
  // Find max speed
  for (int i = 0; i < 200; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);

   if(deay > minDeay) deay = deay - 1;
  }

  for (int i = 0; i < 200; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);

   if(deay > 500) deay = deay + 1;
  }

  // Turn off motor
  delay(50);
  digitalWrite(sleepPin, LOW);
}
void maxStopSpeed(int motor){

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  // Turn on the motor
  digitalWrite(sleepPin, HIGH);
  // Set the direction
  digitalWrite(dirPin, HIGH);
  delay(1);

  int deay = 350;
  int maxStopSpeed = 200;
  // Find max speed
  for (int i = 0; i < 200; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);

   if(deay > maxStopSpeed) deay = deay - 1;
  }

  // Turn off motor
  delay(50);
  digitalWrite(sleepPin, LOW);
}
void maxInitialAcceleration(int motor, int dir, int type){

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  pins q = choosePins((motor+3)%6);

  // Turn on the motor
  digitalWrite(sleepPin, HIGH);
  digitalWrite(q.sleepPin, HIGH);
  // Set the direction
  digitalWrite(dirPin, dir);
  delay(1);

  int numSteps = 0;
  if(type == 0) numSteps = 30;
  else if (type == 1) numSteps = 45;
  else if (type == 2) numSteps = 50;

  // int deay = 400;
  // int deay = 500;
  int deay = 450;

  for (int i = 0; i < numSteps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);

    // if (i == 2) deay = deay - 25;
    // else if (i == 4) deay = deay - 20;
    // else if (i == 6) deay = deay - 15;
    // else if (i == 8) deay = deay - 10;
    // else if (i == 10) deay = deay - 5;
    
    // if (i < 2) deay = deay - 50;
    // if (i == 4) deay = deay - 50;
    // else if (i == 6) deay = deay - 25;
    // else if (i == 8) deay = deay - 25;
    // else if (i == 10) deay = deay - 5;
  }

  // Turn off motor
  if(type == 2) delay(50);
  digitalWrite(sleepPin, LOW);
  digitalWrite(q.sleepPin, LOW);
}

/**
 * @brief      Simple stepping function (should always work)
 *
 * @param[in]  motor  The motor
 * @param[in]  dir    The dir
 * @param[in]  turns  The turns
 * 
 * @note ~0.1s per quarter turn
 */
void stepSlow(int motor, int dir, int turns) {

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
  delay(1);

  // Play with this
  int deay = 100;

  // Pulse motor stepNum times
  for (int i = 0; i < stepNum*32 - 4; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds(deay);

    if(deay > 25 && i < accelSteps*32){
      deay = deay - 1;
    }
    else if(deay > 100){
      deay = deay + 2;
    }

  }

  delay(1);

  // Turn off motor(s)  
  digitalWrite(sleepPin, LOW);
}



void stepFastish(int motor, int dir, int type) {
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
  delay(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_a = 16.0;//20.0; // integration constant, corresponds to initial deay = 500
  const float accel = 2500.0; // [Turns per sec^2]
  const float decel = 11000.0;

  const float maxDeay = 600.0;

  // Acceleration phase
  for (int i = 0; i < accelTurns; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(accel*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Serial.println((int)deay);
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;

  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay
  
  // Deceleration phase
  for (int counter = 0; counter < decelTurns; counter++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }

    // Serial.println((int)deay);
    digitalWrite(stepPin,HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin,LOW); delayMicroseconds((int)deay);
  }


  // Turn off motor
  // if(type == 2) delay(100);
  delay(10); //delay(7); // 7 is pretty sketch, but can work
  digitalWrite(sleepPin, LOW);
  digitalWrite(q.sleepPin, LOW);

}


void stepFastisher(int motor, int dir, int type) {
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
  delay(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_s    = 15.0; // integration constant, corresponds to initial deay = 500
  const float startAccel = 2500.0; // [Turns per sec^2]
  const float accel      = 4500.0;
  const float decel      = 11000.0;
  const float maxDeay    = 600.0;

  // Start phase (slow-ish acceleration)
  for (int i = 0; i < startTurns; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(startAccel*pow(10,-6)*t_n + plusC_s) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for acceleration
  t_n = 0;
  const float plusC_a = pow(10,4) / deay; // Calculate integration constant from current deay
  
  // Acceleration phase
  for (int i = 0; i < accelTurns; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(accel*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay

  // Deceleration phase
  for (int counter = 0; counter < decelTurns; counter++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }

    digitalWrite(stepPin,HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin,LOW); delayMicroseconds((int)deay);
  }

  // Turn off motor
  delay(10);
  digitalWrite(sleepPin, LOW);
  digitalWrite(q.sleepPin, LOW);

}

void stepSym(int motor, int dir, int type) {
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
  delay(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_s  = 16.0; // integration constant, corresponds to initial deay = 500
  const float slowA    = 2500.0; // [Turns per sec^2]
  const float fastA    = 4500.0;

  // Start phase (slow-ish acceleration)
  for (int i = 0; i < slow; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(slowA*pow(10,-6)*t_n + plusC_s) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for acceleration
  t_n = 0;
  const float plusC_a = pow(10,4) / deay; // Calculate integration constant from current deay
  
  // Acceleration phase
  for (int i = 0; i < fast; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(fastA*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d1 = pow(10,4) / deay; // Calculate integration constant from current deay

  // Deceleration phase
  for (int counter = 0; counter < fast; counter++){
    t_n_1 = 10000/(-fastA*pow(10,-6)*t_n + plusC_d1) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1;

    digitalWrite(stepPin,HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin,LOW); delayMicroseconds((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d2 = pow(10,4) / deay; // Calculate integration constant from current deay

  // Deceleration phase
  for (int counter = 0; counter < slow; counter++){
    t_n_1 = 10000/(-slowA*pow(10,-6)*t_n + plusC_d2) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1;

    digitalWrite(stepPin,HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin,LOW); delayMicroseconds((int)deay);
  }

  // Turn off motor
  delay(10);
  digitalWrite(sleepPin, LOW);
  digitalWrite(q.sleepPin, LOW);

}


/**
 * @brief      Fastest (current) version of single/double steps
 *
 * @param[in]  motor  The motor number
 * @param[in]  dir    The direction
 * @param[in]  next   A boolean used for over/under biasing turn
 * 
 * Constant acceleration function
 *    Given dw/dt = A
 *    t_n+1 = 10^5 / (A*10^-6*t_n + C) + t_n  (where C = initial TPS, eg 20)
 *    deay = t_n+1 - t_n
 */

void stepFast(int motor, int dir, int type) {
// Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  int startTurns = 0;
  int accelTurns = 0;
  int decelTurns = 0;

  // 0 indicates change in direction, 1 indicates same direction, 2 signifies end
  if(type == 0)      {startTurns = 10; accelTurns = 30; decelTurns = 10;}
  else if(type == 1) {startTurns = 10; accelTurns = 30; decelTurns = 10;}
  else if(type == 2) {startTurns = 10; accelTurns = 30; decelTurns = 10;}

  // Turn motor on
  digitalWrite(sleepPin, HIGH);
  // Set direction
  digitalWrite(dirPin, dir);
  delay(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_s    = 16.0; // integration constant, corresponds to initial deay = 500
  const float startAccel = 2500.0; // [Turns per sec^2]
  const float accel      = 4500.0;
  const float decel      = 12000.0;
  const float maxDeay    = 600.0;

  // Start phase (slow-ish acceleration)
  for (int i = 0; i < startTurns; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(startAccel*pow(10,-6)*t_n + plusC_s) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for acceleration
  t_n = 0;
  const float plusC_a = pow(10,4) / deay; // Calculate integration constant from current deay
  
  // Acceleration phase
  for (int i = 0; i < accelTurns; i++) {
    // Calculate appropriate deay value
    // Use recurence relation to find next time step
    t_n_1 = 10000/(accel*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; // Update for next cycle

    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay

  // Deceleration phase
  for (int counter = 0; counter < decelTurns; counter++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }

    digitalWrite(stepPin,HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin,LOW); delayMicroseconds((int)deay);
  }

  // Turn off motor
  // if(type == 2) delay(100);
  // delay(20);
  digitalWrite(sleepPin, LOW);

}


/**
 * @brief      Locks specific motors mid-turn.
 *
 * @param[in]  notThisMotor  Not this motor
 * @param[in]  dir           HIGH or LOW
 */
void lockMotorsMidTurn(int motor1, int motor2, int motor3, int state){
  int sleepPin = -1;
  // Loop through all "motor indexes"
  for(int i = 0; i < 6; ++i){
    // Skip the specified motor
    if(i != motor1 && i != motor2 && i != motor3){
      sleepPin = i*2 + 2 + 1;
      digitalWrite(sleepPin, state);
    }
  }
}

// Returns current motor
int stepMidLock(int motor, int dir, int type, int next) {
  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  int startTurns = 10; 
  int accelTurns = 20; 
  int decelTurns = 20;
  // 0 indicates change in direction, 1 indicates same direction, 2 signifies end
  // if(type == 0)      {startTurns = 10; accelTurns = 28; decelTurns = 11;}
  // else if(type == 1) {startTurns = 10; accelTurns = 28; decelTurns = 13;}
  // else if(type == 2) {startTurns = 10; accelTurns = 28; decelTurns = 12;}

  // Turn motor on
  digitalWrite(sleepPin, HIGH);  
  // Set direction
  digitalWrite(dirPin, dir);
  delay(1);

  float t_n = 0; // start at 0 s
  float t_n_1;
  float deay = 0; 
  const float plusC_s    = 16.0; // integration constant, corresponds to initial deay = 500
  const float startAccel = 1500.0; // [Turns per sec^2]
  const float accel      = 3000.0;
  const float decel      = 4500.0;
  const float maxDeay    = 900.0;

  /// Start phase (slow-ish acceleration)
  for (int i = 0; i < startTurns; i++) {
    t_n_1 = 10000/(startAccel*pow(10,-6)*t_n + plusC_s) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; 
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // *MOVED TO 20th STEP*  Engage other motors (skip current motor, since already on)
  //lockMotorsMidTurn(-1, motor, HIGH);

  // Reset timer for acceleration
  t_n = 0;
  const float plusC_a = pow(10,4) / deay; // Calculate integration constant from current deay
  
  /// Acceleration phase
  for (int i = 0; i < accelTurns; i++) {
    // Lock all motors
    if(i == 10) lockMotorsMidTurn(motor, -1, -1, HIGH);
    
    t_n_1 = 10000/(accel*pow(10,-6)*t_n + plusC_a) + t_n; 
    deay = t_n_1 - t_n;
    t_n = t_n_1; 
    digitalWrite(stepPin, HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds((int)deay);
  }

  // Reset timer for decceleration
  t_n = 0;
  const float plusC_d = pow(10,4) / deay; // Calculate integration constant from current deay

  /// Deceleration phase
  for (int counter = 0; counter < decelTurns; counter++){
    if(deay < maxDeay){
      t_n_1 = 10000/(-decel*pow(10,-6)*t_n + plusC_d) + t_n; 
      deay = t_n_1 - t_n;
      t_n = t_n_1;
    }
    digitalWrite(stepPin,HIGH); delayMicroseconds((int)deay);
    digitalWrite(stepPin,LOW); delayMicroseconds((int)deay);
  }


  delay(10);

  // Turn all motors off, except current and next motor
  lockMotorsMidTurn(motor, next, (next+3)%6, LOW);

}


/**
 * @brief      Tests a stepping function on given step list
 *
 * @param      motors      The motors
 * @param      directions  The directions
 * @param      repeats     number of cycle repetitions (1 if unique)
 */
void testStepFunction(int* motors, int* directions, int repeats, Function fn){
  int i = 0;
  // Initialize variables
  int motor = -1; 
  int nextMotor = -1;
  int dir = -1; 
  int type = -1;

  // Initialize microstepping for slow turns
  if(fn == SLOW) digitalWrite(microStep, HIGH);

  // Loop through iterations
  for(int j = 0; j < repeats; ++j){
    // Loop through move list
    while(motors[i] != -1){
      // Store motor/dir values
      motor = motors[i];
      dir = directions[i];
      // Set nextMotor
      if(motors[i+1] == -1) nextMotor = motors[0];
      else                  nextMotor = motors[i+1];        
      // check if end of move list or end of total moves
      if(directions[i+1] == -1){
        // End of all moves
        if(j == repeats-1) type = 2;
        // "Loop" back around
        else type = (directions[i] == directions[0]);
        
      }
      // Set under/over biasing
      else{
        type = (directions[i] == directions[i+1]);
      }

      switch(fn){
        case MIDLOCK:
          stepMidLock   (motor, dir, type, nextMotor); break;
        case SLOW:
          stepSlow      (motor, dir, 1); break;
        case FASTISH:
          stepFastish   (motor, dir, type); break;
        case FASTISHER: 
          stepFastisher (motor, dir, type); break;
        case STEPSYM:
          stepSym       (motor, dir, type); break;
        case FAST:
          stepFast      (motor, dir, type); break;
        default:
          break;
      }
      // Increment i 
      ++i;
    }

    // Reset i for next cycle
    i = 0;
  }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////// Execution ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief      Initializes pins
 */
void setup() {
  // Allow serial print (to serial monitor) of time
  // Serial.begin(9600);
  // CAN'T DO IF USING Pin 0 and Pin 1
  
  // Set all pins as outputs
  pinMode(dirPin, OUTPUT);
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
  randomSeed(analogRead(0));
  generateRandomMoves();

}

/**
 * @brief      Loops continuously on Arduino
 */
void loop() {

  delay(3000);

////////////////////////////////
/// Slow, Random Mix & Solve ///
////////////////////////////////

  // // Initialize microstepping
  // digitalWrite(microStep, HIGH);
  // // printRandomMoves();
  // // Random mix and solve
  // randomMixUp(SLOW);
  // delay(5000);
  // randomSolve(SLOW);  

////////////////////////////////
/// Test Slow Turning Speed  ///
//////////////////////////////// 

  // // Initialize microstepping
  // digitalWrite(microStep, HIGH);

  // for(int i = 0; i < 100; ++i){
  //   stepSlow(random(6) , random(2), 1);
  //   stepSlow(4,HIGH,2);
  //   // delay(100);
  // }

////////////////////////////
/// Determine Top Speed  ///
//////////////////////////// 
  
  // Test a motors top speed
  // stepTopSpeed(2);  
  // stepTopSpeed(5);
  // stepTopSpeed(3);
  
  /// Start Speed: 300, when perfect (maybe 350, or 400 if safe...
  /// Top Runnign speed : 150

/////////////////////////////////
/// Determine Max Stop Speed  ///
/////////////////////////////////

  // maxStopSpeed(3);

  /// Max Stop Speed: 200 

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

    // stepFastisher(2, LOW, 1);
    // stepFastisher(1, LOW, 0);
    // stepFastisher(2, HIGH, 1);
    // stepFastisher(1, HIGH, 2);

    // for(int i = 0; i < 6; ++i){
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


/// MidLock Solve
    // for(int i = 0; i < 8; ++i){
    //   stepMidLock(2,HIGH, 1);
    //   // delay(100);
    //   stepMidLock(1,HIGH, 1);
    //   // delay(100);
    // }

    // for(int i = 0; i < 6; ++i){
    //   stepMidLock(2, LOW, 1, 1);
    //   stepMidLock(1, LOW, 0, 2);
    //   stepMidLock(2, HIGH, 1, 1);
    //   stepMidLock(1, HIGH, 0, 2);
    // }
    // digitalWrite(sleepPin1, LOW);
    // digitalWrite(sleepPin2, LOW);
    // digitalWrite(sleepPin5, LOW);


/// Fastest Solve
    // for(int i = 0; i < 10; ++i){
    //   stepFast(2, LOW, 0);
    //   delay(100);
    // }


//////////////////////////////////
/// Test List Wrapper Function ///
//////////////////////////////////

    // int* motors;
    // int* dirs;
    
    // // 1 motor, N cycles (simple test)
    // int numMoves =            1+1;
    // motors = (int[numMoves])  {1, -1};
    // dirs = (int[numMoves])    {LOW, -1};
    // int cycles =              6;

    // // 2 motors, 6 cycles (Final move in solve algo)
    // int numMoves =            4+1;
    // motors = (int[numMoves])  {1, 2, 1, 2, -1};
    // dirs = (int[numMoves])    {LOW, LOW, HIGH, HIGH, -1};
    // int cycles =              6;

    // // 3 motors, 80 cycles (all the same direction)
    // int numMoves =            3+1;
    // motors = (int[numMoves])  {0, 1, 2, -1};
    // dirs = (int[numMoves])    {HIGH, HIGH, HIGH, -1};
    // int cycles =              80;

    // // 5 motors, 60 cycles (Good final test, but only 5 motors)
    // int numMoves =            5+1;
    // motors = (int[numMoves])  {3, 2, 1, 5, 4, -1};
    // dirs = (int[numMoves])    {HIGH, HIGH LOW, HIGH, LOW, -1};
    // int cycles =              60;



    // // SLOW, FASTISH, FASTISHER, MIDLOCK, FAST, STEPSYM
    // testStepFunction(motors, dirs, cycles, SLOW);


////////////////////////////////
/// Solve with Step Function ///
//////////////////////////////// 


  // Random mix and solve
  randomMixUp(MIDLOCK);
  lockMotorsMidTurn(-1, -1, -1, LOW); 
  
  delay(5000);
  
  randomSolve(MIDLOCK);
  lockMotorsMidTurn(-1, -1, -1, LOW);  



//  delay(3000);

  // // FINAL TEST (w/ only 5 motors)

  // delay(6000);
  // // Scrambles the cube
  // stepList();
  // delay(5000);

  // // Setup timer
  // unsigned long time1 = millis();
  
  // // SOLVE THE CUBE (with solution moves)
  // solve();
  
  // // Print the execution time
  // unsigned long time2 = millis();
  // Serial.println(time2-time1);
  // // Turn me off now :)
  // delay(20000);

}



