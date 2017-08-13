/*
#include "StepperMotorController.h"
int main(int argc, char * argv[])
{
    setup();

    int motor = atoi(argv[1]);
    pins p = choosePins(motor);
    int stepPin = p.stepPin;
    int disablePin = p.disablePin;
    printf("Hello\n");
    //printf("%d, %d\n", stepPin, disablePin);

    digitalWrite(sleepPin, HIGH);
    digitalWrite(disablePin, LOW);
    digitalWrite(stepPin, HIGH);

    delayMillis(1000);
    printf("Press ENTER to continue\n");
    getchar();   

          
    digitalWrite(disablePin,HIGH);
    digitalWrite(sleepPin, LOW);
    printf("End of Function\n");

    return 0;
}
*/

//////////////////////////////
// Breadboard simplie setup //
//////////////////////////////

#include "EasyPIO.h"
#include <time.h>

#define stepPin 21

#define LOW  0
#define HIGH 1


int main(void)
{
    pioInit();

    pinMode(stepPin, OUTPUT);

    int i;

    // sleep and disable pins are wired to HIGH
    digitalWrite(stepPin, HIGH);

    for(i = 0; i < 32*50; i++){
        digitalWrite(stepPin, HIGH);delayMicros(100);
        digitalWrite(stepPin, LOW);delayMicros(100);
    }

}