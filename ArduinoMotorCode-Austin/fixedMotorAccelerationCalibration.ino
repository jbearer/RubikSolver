/*
   Custom Acceleration Stationayr Motors

   two functions, step1 and step2, that correspond to
   quarter and half rotations

   custom "acceleration" is achieved by shortening the
   duration of the pulses (ie. smaller 'deay' = faster
   motor)

   Only for three motors now, but easilly scalable to six

*/

////////////////////// Define Constants ///////////////////////////////

// Define Arduino pins
const int stepPin1 = 2;
const int sleepPin1 = 3;
const int stepPin2 = 4;
const int sleepPin2 = 5;
const int stepPin3 = 6;
const int sleepPin3 = 7;
const int stepPin4 = 8;
const int sleepPin4 = 9;
const int stepPin5 = 10;
const int sleepPin5 = 11;
const int stepPin6 = 12;
const int sleepPin6 = 13;
const int dirPin = 0;

// int deay = 0; // duration of pulses (dictates speed of motor)

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
Turn solution[solutionLength] = {Ri, Di, R, Li, U, D, R, Fi, 
  Ri, D, U, Li, F, D2, Li, U2, F2, U2, Ri, D2, R, U2, R, 
  D2, Li, D2, Li, D2, F2};
// Initialize turnInfo for solution
turnInfo solutionInfo[solutionLength];


// Helper Function, switch-case statement
pins choosePins(int motor) {
  int stepPin;
  int sleepPin;
  // Define pins to use
  switch (motor) {
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
    case 6:
      stepPin = stepPin6; sleepPin = sleepPin6;
      break;
    default:
      stepPin = 1; sleepPin = 1; // Undefined
      break;
  }
  pins p {stepPin, sleepPin};
  return p;
}

///////////////////////////// step1 (quarter rotation)  ///////////////////////////////////////

void step1(int motor, int dir) {

  const int maxDeay = 425;  // Starting delay time
  const int stage1 = 350;   // from -10 to -5
//  const int minDeay = 140;  // Fastest (capped) speed

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  digitalWrite(sleepPin, HIGH);
  deay = maxDeay;             // Always reset deay when function called (important)
  delay(1);
  digitalWrite(dirPin, dir);  // Define Direction
  ////// Accelerating
  for (int counter = 0; counter < 40; counter++) {
    digitalWrite(stepPin, HIGH); delayMicroseconds(deay); // Pulse HIGH then LOW
    digitalWrite(stepPin, LOW); delayMicroseconds(deay); // This is where 'speed' is determined
    if (deay > stage1) {  // Update deay when in phase 1
      deay = deay - 10;
    }
    else {                // Update deay when in "phase 2" (slower acceleration)
      deay = deay - 5;
    }
    // No need minDeay b/c never gets to max speed (minDeay)
  }

  ////// Deceleration
  for (int counter = 0; counter < 10; counter++) {
    digitalWrite(stepPin, HIGH); delayMicroseconds(deay);
    digitalWrite(stepPin, LOW); delayMicroseconds(deay);
    deay = deay + 20; // Just decelerated linearly (w.r.t deay)
  }
  digitalWrite(sleepPin, LOW);
}

///////////////////////////// step2 (half rotation) ///////////////////////////////////////

void step2(int motor, int dir) {

  const int maxDeay = 425;  // slowest speed
  const int stage1 = 350;   // from -10 to -5
  const int stage2 = 150;   // from -5 to -1
  const int minDeay = 140;  // fastest speed

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, dir);
  deay = maxDeay;
  delay(1);
  ////// Accelerating
  for (int counter = 0; counter < 85; counter++) { // Counters experimentally determined
    digitalWrite(stepPin, HIGH); delayMicroseconds(deay);
    digitalWrite(stepPin, LOW); delayMicroseconds(deay);
    if (deay > stage1) { // Update deay based on speed (stage 1)
      deay = deay - 10;
    }
    else if (deay > stage2) { // Stage 2 decreases acceleation
      deay = deay - 5;
    }
    else if (deay > minDeay) {
      deay = deay - 1;
    }
    // Afterwards, speed plateaus at minDeay, max speed
  }

  ////// Deceleration
  for (int counter = 0; counter < 15; counter++) {
    digitalWrite(stepPin, HIGH); delayMicroseconds(deay);
    digitalWrite(stepPin, LOW); delayMicroseconds(deay);
    deay = deay + 20;
  }
  digitalWrite(sleepPin, LOW);
}


////////////////////////////////////////////////////////////////////////////
////////////////////////// Test Function ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Notes
// Put quite a bit of pressure for it
// Squeeze centers
// Max Speed w/ cube = 140 deay
// Max decelleration w/ cube = + 20
// Max acceleration w/ cube = (-5) 150
// Max acceleration w/ cube = (-10) 220

//////////// Test Max Speed //////////////////////////
void stepSpeed(int motor, int dir) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, dir);  // Define Direction
  delay(1);
  deay = 300;
  // Accelerating
  for (int counter = 0; counter < 50; counter++) {
    digitalWrite(stepPin, HIGH); delayMicroseconds(deay);
    digitalWrite(stepPin, LOW); delayMicroseconds(deay);
    // motor closest: 130 abs max, good 140
    // motor left: max 135, good 150
    // motor right: max 135, good 155
    /*
      if (deay > 240){ // increase/decrease to find where threshold value is, ie. -10 stops working
      deay = deay - 10;
      }
      else if (deay > 220){ // 180 works for left, 160 max
      deay = deay - 1;
      }
      //
      else if (deay > 220){
      deay = deay - 2;
      }
      else if (deay > 210){
      deay = deay - 1;
      }
    */
  }
  //  // Deceleration
  //  for (int counter = 0; counter < 10; counter++){
  //    digitalWrite(stepPin,HIGH); delayMicroseconds(deay);
  //    digitalWrite(stepPin,LOW); delayMicroseconds(deay);
  //    if (deay < 250){
  //      deay = deay + 5;
  //    }
  //    else if (deay < 300){
  //      deay = deay + 10;
  //    }
  //   // else if (deay < 320){
  //     // deay = deay + 20;
  //    //}
  //  }
  delay(30);
  digitalWrite(sleepPin, LOW);
}

void stepSlow(int motor, int dir) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, dir);
  delayMicroseconds(1000);
  for (int counter = 0; counter < 41; counter++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  digitalWrite(sleepPin, LOW);
  delayMicroseconds(2000);
}

/**
 * stepList()
 *    executes the moves specified in the messUp moves array
 *    uses stepTest(), which is a slow turning function (deay = 600 vs 500)
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
    // If next direvtion is same (CW->CW or CCW->CCW), set 1. Else set 0
    else next = (messUpInfo[i].dir == messUpInfo[i+1].dir); 

    // Specify single vs. double turn
    if(messUpInfo[i].turns == 1){
      stepTest(messUpInfo[i].motor, messUpInfo[i].dir, next);
    }
    else{
      stepTest2(messUpInfo[i].motor, messUpInfo[i].dir, next);
    }
  }
}

/**
 * solve()
 *    executes the moves specified in the solution array
 *    (mimics stepList(), except now with solution instead of messUp)
 *    uses stepFast instead of stepTest
 */
void solve(){
  // Convert enum Turns to struct turnInfo
  convertSolution();
  int next;
  // Loop through moves
  for(int i = 0; i < solutionLength; ++i){
    // Determine if bias short, or bias long turn
    if(i == solutionLength-1) next = 2;
    else if (solutionInfo[i].motor%3 == solutionInfo[i+1].motor%3) next = 2;
    else next = (solutionInfo[i].dir == solutionInfo[i+1].dir); 
    // single or double turn
    if(solutionInfo[i].turns == 1){
      stepFast(solutionInfo[i].motor, solutionInfo[i].dir, next);
    }
    else{
      stepFast2(solutionInfo[i].motor, solutionInfo[i].dir, next);
    }
  }
}

// HELPER METHODS
/**
 * enumToTurnInfo()
 *    Constructs and returns a struct turnInfo with motor#, direction, and 
 *    turn data. Big ole switch-case statement
 */
turnInfo enumToTurnInfo(Turn t){
  int motor;
  int dir;
  int turns;
  switch(t){
    case Fi: motor = 4; dir = HIGH; turns = 1; break;
    case Bi: motor = 1; dir = HIGH; turns = 1; break;
    case Ri: motor = 3; dir = HIGH; turns = 1; break;
    case Li: motor = 6; dir = HIGH; turns = 1; break;
    case Ui: motor = 2; dir = HIGH; turns = 1; break;
    case Di: motor = 5; dir = HIGH; turns = 1; break;

    case F: motor = 4; dir = LOW; turns = 1; break;
    case B: motor = 1; dir = LOW; turns = 1; break;
    case R: motor = 3; dir = LOW; turns = 1; break;
    case L: motor = 6; dir = LOW; turns = 1; break;
    case U: motor = 2; dir = LOW; turns = 1; break;
    case D: motor = 5; dir = LOW; turns = 1; break;

    case F2: motor = 4; dir = LOW; turns = 2; break;
    case B2: motor = 1; dir = LOW; turns = 2; break;
    case R2: motor = 3; dir = LOW; turns = 2; break;
    case L2: motor = 6; dir = LOW; turns = 2; break;
    case U2: motor = 2; dir = LOW; turns = 2; break;
    case D2: motor = 5; dir = LOW; turns = 2; break;

    default: printf("error: Invalid direction input");
      motor = -1; dir = -1; turns = 0; break;
  }
  turnInfo result {motor,dir,turns};
  return result;
}
/**
 * convertMessUp()
 *    Converts messUp[] (Turns) to messUpInfo[] (turnInfo)
 *    (populates messUpInfo[]))
 */
void convertMessUp(){
  for(int i = 0; i < messUpLength; i++){
    messUpInfo[i] = enumToTurnInfo(messUpMoves[i]);
  }
}
/**
 * convertSolution()
 *    Converts solution[] (Turns) to solutionInfo[] (turnInfo)
 *    (populates solutionInfo[]))
 */
void convertSolution(){
  for(int i = 0; i < solutionLength; i++){
    solutionInfo[i] = enumToTurnInfo(solution[i]);
  }
}


/**
 * stepTest()
 *    Steps the stepper motor 90 degrees, at a slow/controlled rate
 *    considers the next move to appropriately bias current move
 *    (ie, CW->CW or CCW->CCW must bias towards overshooting 90,
 *    while changing direction allows for corner cutting)
 *    Fiddle with:
 *        stepNum- makes smoother or faster transitions
 *        deay- controls the speed of the motor (ie small deay = fast)
 */
void stepTest(int motor, int dir, int next) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  int stepNum;

  // If same direction as last, bias towards after full turn
  // If changing direction, bias to shorter turn
  if (next == 1) stepNum = 49; // Same direction = overturn
  else if (next == 0) stepNum = 45;
  else stepNum = 50; // Need exact, since last

  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, dir);
  delay(1);
  deay = 600;
  for (int i = 0; i < stepNum; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);
  }
  if(next == 2) delay(100);
  // delay(1);
  digitalWrite(sleepPin, LOW);
}

void stepTest2(int motor, int dir, int next) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  int stepNum;

  // If same direction as last, bias towards after full turn
  // If changing direction, bias to shorter turn
  if (next == 1) stepNum = 98; // Same direction = overturn
  else if (next == 0) stepNum = 90;
  else stepNum = 100;

  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, dir);
  delay(1);
  deay = 600;
  for (int i = 0; i < stepNum; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);
  }
  if(next == 2) delay(100);
  digitalWrite(sleepPin, LOW);
}

void stepFast(int motor, int dir, int next) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  int stepNum;

  // If same direction as last, bias towards after full turn
  // If changing direction, bias to shorter turn
  if (next == 1) stepNum = 47; // Same direction = overturn
  else if (next == 0) stepNum = 39;
  else stepNum = 46;

  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, dir);
  delay(1);
  deay = 500;
  for (int i = 0; i < stepNum; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);
  }
  // if(next == 2) delay(100);
  // delay(1);
  digitalWrite(sleepPin, LOW);
}

void stepFast2(int motor, int dir, int next) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  int stepNum;

  // If same direction as last, bias towards after full turn
  // If changing direction, bias to shorter turn
  if (next == 1) stepNum = 95; // Same direction = overturn
  else if (next == 0) stepNum = 90;
  else stepNum = 91;

  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, dir);
  delay(1);
  deay = 500;
  for (int i = 0; i < stepNum; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);
  }
  // if(next == 2) delay(100);
  digitalWrite(sleepPin, LOW);
}

///////////////////////////// Execution ///////////////////////////////////////
void setup() {
  Serial.begin(9600);
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
  pinMode(stepPin6, OUTPUT);
  pinMode(sleepPin6, OUTPUT);

  pinMode(dirPin, OUTPUT);
  //  pinMode(dirPin2,OUTPUT);

  digitalWrite(sleepPin1, LOW);
  digitalWrite(sleepPin2, LOW);
  digitalWrite(sleepPin3, LOW);
  digitalWrite(sleepPin4, LOW);
  digitalWrite(sleepPin5, LOW);
  digitalWrite(sleepPin6, LOW);
}

void loop() {
  // FRONT = 4
  // BACK = 1
  // RIGHT = 3
  // LEFT = 6
  // UP = 2
  // DOWN = 5

// FINAL TEST (w/ only 5 motors)
  // stepList();
  delay(6000);
  stepList();
  delay(5000);

  unsigned long time1 = millis();
  solve();
  unsigned long time2 = millis();
  Serial.println(time2-time1);
  delay(20000);

// Test double turns
  // delay(6000);
  // stepFast2(4,HIGH,1);
  // stepFast2(2,HIGH,2);

  // delay(3000);
  // stepFast2(2,LOW,1);
  // stepFast2(4,LOW,2);

  // delay(3000);

  // for (int i = 0; i < 6; i++){
  //   stepFast2(4, HIGH, 1);
  //   stepFast2(2, HIGH, 0);

  //   stepFast2(4, LOW, 1);
  //   stepFast2(2, LOW, 0);
  // }
  // delay(100);
  // for (int i = 0; i < 6; i++){
  //   stepFast2(3, HIGH, 1);
  //   stepFast2(5, HIGH, 0);

  //   stepFast2(3, LOW, 1);
  //   stepFast2(5, LOW, 0);
  // }

  // F2, Ri, D2, L, U
  // delay(6000);
  // stepTest2(4, HIGH, 1);
  // stepTest(3, HIGH, 1);
  // stepTest2(5, HIGH, 0);
  // stepTest(6, LOW, 1);
  // stepTest(2, LOW, 2);
 
  // delay(1000);

  // stepTest(2, HIGH, 1);
  // stepTest(6, HIGH, 0);
  // stepTest2(5, LOW, 1);
  // stepTest(3, LOW, 1);
  // stepTest2(4, LOW, 2);
  // delay(3000);

  // for(int i = 0; i < 60; i++){
  //   stepTest2(4, HIGH, 1);
  //   stepTest(3, HIGH, 1);
  //   stepTest2(5, HIGH, 0);
  //   stepTest(6, LOW, 1);
  //   stepTest(2, LOW, 0);
  // }

  // delay(6000);
  // stepFast(4,HIGH,0);
  // stepFast2(2,LOW,2);
  // stepFast(5,LOW,0);
  // stepFast2(4,HIGH,0);
  // stepFast(6,LOW, 1);
  // stepFast(2,LOW,1);
  // stepFast(3,LOW,0);
  // stepFast2(5,HIGH,1);
  // stepFast2(6,HIGH,1);
  // stepFast(4,HIGH,0);
  // stepFast(2,LOW,1);
  // stepFast(3,LOW,0);
  // stepFast2(4,HIGH,0);
  // stepFast(6,LOW, 1);
  // stepFast(2,LOW,1);
  // stepFast(3,LOW,0);
  // stepFast2(5,HIGH,1);
  // stepFast2(6,HIGH,2);
  // delay(3000);




  // delay(6000);
  // for(int i = 0; i < 60; i++){
  //   stepTest2(4, HIGH, 1);
  //   stepTest(3, HIGH, 1);
  //   stepTest2(5, HIGH, 0);
  //   stepTest(6, LOW, 1);
  //   if(i%5 == 0){
  //     stepTest(2,LOW,2);
  //     delay(2000);
  //   }
  //   else{
  //     stepTest(2, LOW, 0);
  //   }
  // }
  // delay(3000);
  // int delayBtw = 0;
  // delay(6000);
  // stepTest(4,HIGH,1);
  // stepTest(3,HIGH,0);
  // stepTest(2,LOW,0);
  // stepTest(6,HIGH,0);
  // stepTest(5,LOW,2);
  // delay(1000);

  // stepTest(5,HIGH,0);
  // stepTest(6,LOW,0);
  // stepTest(2,HIGH,0);
  // stepTest(3,LOW,1);
  // stepTest(4,LOW,2);
  // delay(3000);

 // TBottom 3 motors, forwards, then backwards
 //  for (int i = 0; i < 6; i++){
 //    stepTest(4, HIGH, 1);
 //    stepTest(2, HIGH, 0);

 //    stepTest(4, LOW, 1);
 //    stepTest(2, LOW, 0);
 //  }

 //  delay(3000);
 // for (int i = 0; i < 6; i++){
 //    stepTest(3, HIGH, 1);
 //    stepTest(5, HIGH, 0);

 //    stepTest(3, LOW, 1);
 //    stepTest(5, LOW, 0);
 //  }
   // for (int i = 0; i < 80; i++){
   //   stepTest(4,HIGH,1);
   //   stepTest(5,HIGH,1);
   //   stepTest(6,HIGH,1); 
   // }

   // for (int i = 0; i < 2; i++){
   //    stepTest(4,HIGH,1);
   //    stepTest(3,HIGH,0);
   //    stepTest(2,LOW,0);
   //    stepTest(6,HIGH,0);
   //    stepTest(5,LOW,0);
   // }
   // delay(3000);
   // for (int i = 0; i < 2; i++){
   //    stepTest(5,HIGH,0);
   //    stepTest(6,LOW,0);
   //    stepTest(2,HIGH,0);
   //    stepTest(3,LOW,1);
   //    stepTest(4,LOW,0);
   // }
   // delay(3000);

   // A true challenge
   // for (int i = 0; i < 60; i++){
   //    stepTest(4,HIGH,1);
   //    stepTest(3,HIGH,0);
   //    stepTest(2,LOW,0);
   //    stepTest(6,HIGH,0);
   //    stepTest(5,LOW,0);
   // }
   // delay(3000);
}


