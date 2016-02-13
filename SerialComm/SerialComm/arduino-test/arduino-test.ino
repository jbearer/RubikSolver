#include "test-constants.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
}

void loop() {
    if (Serial.available() > 0) {
        // read the incoming byte:
        incomingByte = Serial.read();

        if (incomingByte = BLINK) {
          digitalWrite(13, HIGH);
        }
        if (incomingByte = CONTINUE) {
          digitalWrite(13, LOW);
        }

        // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte);

        Serial.println(CONTINUE);
    }
}
