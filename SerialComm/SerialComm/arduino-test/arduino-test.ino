#include "test-constants.h"
#include <Servo.h>

#define STEP_ANGLE 1
#define STEP_TIME 10

using namespace testConstants;

Servo motor;
bool isBlinking = false;

void blinkOn() {
  isBlinking = true;
}

void blinkOff() {
  isBlinking = false;
}

void blink() {
  if (isBlinking) {
    motor.write(motor.read() + STEP_ANGLE);
    delay(STEP_TIME);
  }
}

void setup() {
  motor.attach(A0);
  Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
        // read the incoming byte:
        char incomingByte = Serial.read();

       if (incomingByte = BLINK) {
          blinkOn();
        }
        if (incomingByte = CONTINUE) {
          blinkOff();
        }

        // say what you got:*/
        //Serial.print("I received: ");
        //Serial.println(incomingByte);

        //Serial.println(CONTINUE);
    }

    blink();
}
