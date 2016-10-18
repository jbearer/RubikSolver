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
    stepFast(solutionInfo[i].motor, solutionInfo[i].dir, next, solutionInfo[i].turns);
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
 * @brief      Fastest (current) version of single/double steps
 *
 * @param[in]  motor  The motor number
 * @param[in]  dir    The direction
 * @param[in]  next   A boolean used for over/under biasing turn
 */
void stepFast(int motor, int dir, int next, int turns) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;
  int stepNum;

  int accelTurns;
  int decelTurns;
 
  // Determine appropriate stepNum
  if (next == 1){ // Same direction = overturn
    if(turns == 1){
      //stepNum = 47; // Single turn
      accelTurns = 35;
      decelTurns = 10;
    }
    else{
      //stepNum = 95;           // Double turn
    }
  }
  else if (next == 0){ // Diff direction = underturn
    if(turns == 1) stepNum = 39;
    else stepNum = 90;
  }
  else { // Exact turn
    if(turns == 1) stepNum = 46;
    else stepNum = 91;
  }

  // Turn motor on
  digitalWrite(sleepPin, HIGH);
  // Set direction
  digitalWrite(dirPin, dir);
  delay(1);
  int deay = 500;

  // Acceleration phase
  for (int i = 0; i < stepNum; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);  delayMicroseconds(deay);
    // Accelerate (gradually decrease pulse width)
    if(deay > 400){
      deay = deay - 20;
    }
    else if (deay > 220){
      deay = deay - 2;
    }
    else if (deay > 200){
      deay = deay - 1;
    }
  }
  
  // Deceleration phase
   for (int counter = 0; counter < 10; counter++){
     digitalWrite(stepPin,HIGH); delayMicroseconds(deay);
     digitalWrite(stepPin,LOW); delayMicroseconds(deay);
     if (deay < 250){
       deay = deay + 5;
     }
     else if (deay < 300){
       deay = deay + 10;
     }
    // else if (deay < 320){
      // deay = deay + 20;
     //}
   }


  // Turn off motor
  digitalWrite(sleepPin, LOW);
}


void stepTopSpeed(int motor, int dir) {

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int sleepPin = p.sleepPin;

  // Turn on the motor
  digitalWrite(sleepPin, HIGH);
  // Set the direction
  digitalWrite(dirPin, dir);
  delay(1);

  int deay = 500;
  // Find max speed
  for (int i = 0; i < 400; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deay);

    if(deay > 135) deay = deay - 1;
  }

  // // Slowly decelerate
  // for (int i = 0; i < 500; i++) {
  //   digitalWrite(stepPin, HIGH);
  //   delayMicroseconds(deay);
  //   digitalWrite(stepPin, LOW);
  //   delayMicroseconds(deay);

  //   if(deay < 600) deay = deay + 1;
  // }

  // Turn off motor
  digitalWrite(sleepPin, LOW);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// Execution ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief      Initializes pins
 */
void setup() {
  // Allow serial print (to serial monitor) of time
  Serial.begin(9600);
  // Set all pins as outputs
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

  // Start with motor off
  digitalWrite(sleepPin1, LOW);
  digitalWrite(sleepPin2, LOW);
  digitalWrite(sleepPin3, LOW);
  digitalWrite(sleepPin4, LOW);
  digitalWrite(sleepPin5, LOW);
  digitalWrite(sleepPin6, LOW);
}

/**
 * @brief      Loops continuously on Arduino
 */
void loop() {
  delay(6000);
  // stepTest(4, HIGH, 1, 1);
  // stepTest(5, HIGH, 0, 1);
  // stepTest(6, LOW, 1, 1);
  stepTopSpeed(2, HIGH );
  delay(3000);

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


