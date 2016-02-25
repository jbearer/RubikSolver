/**
  * Test the functionality of the motors using a predetermined sequence of turns that involves all motors.
  */
 
// determines the speed of the motor
const int deay = 500; 

// initializes motor pins
const int stepPinR = 1; const int dirPinR = 2;
const int stepPinL = 3; const int dirPinL = 4;
const int stepPinU = 5; const int dirPinU = 6;
const int stepPinD = 7; const int dirPinD = 8;
const int stepPinF = 9; const int dirPinF = 10;
const int stepPinB = 11; const int dirPinB = 12;

// takes in solution
char moves[] = "rludfbRLUDFB2r2L2B";
const int numberOfSteps = 15;

/* stepX
 *  inputs: motor- character representing motor to turn ('R', 'L'...)
 *          turns- integer number of 90 deg turns
 *          dir- determines direction, HIGH = cw, LOW = ccw
 */
void stepX(char motor, int turns, int dir ){
  int stepPin = 0;
  int dirPin = 0;
  // determine motor, and set step & dir pins to that motor
  if      (motor == 'R'){ stepPin = stepPinR; dirPin = dirPinR;}
  else if (motor == 'L'){ stepPin = stepPinL; dirPin = dirPinL;}
  else if (motor == 'U'){ stepPin = stepPinU; dirPin = dirPinU;}
  else if (motor == 'D'){ stepPin = stepPinD; dirPin = dirPinD;}
  else if (motor == 'F'){ stepPin = stepPinF; dirPin = dirPinF;}
  else if (motor == 'B'){ stepPin = stepPinB; dirPin = dirPinB;}
  // steps the motor
  digitalWrite(dirPin, dir);
  for (int counter = 0; counter < turns*100; counter++){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(deay); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(deay);
  }
}

/* solve
 *  solves the cube given a list of moves
 *  rotates the corresponding motor based on 
 *  solution list
 */
void solve( char moves[]){
  // loop through list of steps
  int i = 0;
  while (i<numberOfSteps){
    // Deals with clockwise turns
    if      (moves[i]=='r'){stepX('R',1,HIGH); i++;}
    else if (moves[i]=='l'){stepX('L',1,HIGH); i++;}
    else if (moves[i]=='u'){stepX('U',1,HIGH); i++;}
    else if (moves[i]=='d'){stepX('D',1,HIGH); i++;}
    else if (moves[i]=='f'){stepX('F',1,HIGH); i++;}
    else if (moves[i]=='b'){stepX('B',1,HIGH); i++;}
    // Deals with ccw turns
    else if (moves[i]=='R'){stepX('R',1,LOW); i++;}
    else if (moves[i]=='L'){stepX('L',1,LOW); i++;}
    else if (moves[i]=='U'){stepX('U',1,LOW); i++;}
    else if (moves[i]=='D'){stepX('D',1,LOW); i++;}
    else if (moves[i]=='F'){stepX('F',1,LOW); i++;}
    else if (moves[i]=='B'){stepX('B',1,LOW); i++;}
    // Deals with 180 deg turns
    else if (moves[i]=='2'){
      if      (moves[i+1]=='r'){stepX('R',2,HIGH); i+=2;}
      else if (moves[i+1]=='l'){stepX('L',2,HIGH); i+=2;}
      else if (moves[i+1]=='u'){stepX('U',2,HIGH); i+=2;}
      else if (moves[i+1]=='d'){stepX('D',2,HIGH); i+=2;}
      else if (moves[i+1]=='f'){stepX('F',2,HIGH); i+=2;}
      else if (moves[i+1]=='b'){stepX('B',2,HIGH); i+=2;}
    } 
  }
}


/* setup
 *  establishes pins as outputs
 */
void setup() {
  pinMode(stepPinR,OUTPUT); pinMode(dirPinR,OUTPUT);
  pinMode(stepPinL,OUTPUT); pinMode(dirPinL,OUTPUT);
  pinMode(stepPinU,OUTPUT); pinMode(dirPinU,OUTPUT);
  pinMode(stepPinD,OUTPUT); pinMode(dirPinD,OUTPUT);
  pinMode(stepPinF,OUTPUT); pinMode(dirPinF,OUTPUT);
  pinMode(stepPinB,OUTPUT); pinMode(dirPinB,OUTPUT);
}

/* loop
 *  solves the cube
 */
void loop() {
  delay(3000);
  solve(moves);
}
