// stepInput.c


#include "StepperMotorController.h"
#include <stdlib.h>

int main (int argc, char * argv[])
{
    if(argc != 4){
        printf("ERROR: stepInput must have exactly 3 arguments: motor, direction, steps\n");
        printf("\tEx: ./stepInput 1 0 50");
        return 0;
    }
    int motor = atoi(argv[1]);
    int dir = atoi(argv[2]);
    int stepNum = atoi(argv[3]);

    // Setup
    setup();
    printf("Setup Done\n");
    delayMillis(1000);
    printf("moving\n");

    // Stepo the motors
    digitalWrite(sleepPin, HIGH);
    stepE(motor, dir, stepNum);
    digitalWrite(sleepPin, LOW);

    printf("done\n");
    
    return 0;
}
