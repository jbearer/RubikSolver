#include "StepperMotorController.h"

int main(void)
{
    setup();

    digitalWrite(disablePin0, HIGH);
    digitalWrite(disablePin1, HIGH);
    digitalWrite(disablePin2, HIGH);
    digitalWrite(disablePin3, HIGH);
    digitalWrite(disablePin4, HIGH);
    digitalWrite(disablePin5, HIGH);

    digitalWrite(sleepPin, LOW);

    return 0;
}