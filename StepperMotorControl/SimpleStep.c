/**
 * Austin Chun
 * Stepper Motor with Raspberry Pi
 * (single motor, simple step)
 * 
 * USed for testing sleep vs enable pins
 */

 // Include Guards
#ifndef SIMPLE_STEP
#define SIMPLE_STEP

#include "EasyPIO.h" // GPIO interfaces

#define LOW 0
#define HIGH 1

#define INPUT 0
#define OUTPUT 1


#define dirPin 22
#define stepPin 27
#define NenablePin 4
#define sleepPin 17

void step(int dir){
    
    // Wake up motor
    digitalWrite(sleepPin,HIGH);

    //Set Direction
    digitalWrite(dirPin, dir);

    delayMillis(1);
    
    int numSteps = 50;
    int i;
    int deay = 500;

    for(i = 0; i < numSteps; ++i){
        digitalWrite(stepPin, HIGH); delayMicros(deay);
        digitalWrite(stepPin, LOW);  delayMicros(deay);
    }

    delayMillis(10);

    digitalWrite(sleepPin, LOW);
}


void testSleep(){
    // Wake up motor
    digitalWrite(sleepPin,HIGH);

    //Set Direction
    digitalWrite(dirPin, HIGH);

    delayMillis(1);

    int numSteps = 1;
    int i;
    int deay = 600;

    for(i = 0; i < numSteps; ++i){
        digitalWrite(stepPin, HIGH); delayMicros(deay);
        digitalWrite(stepPin, LOW);  delayMicros(deay);
    }

    delayMillis(100);

    digitalWrite(sleepPin, LOW);
}

void testEnable(){
    // Wake up motor
    digitalWrite(NenablePin, LOW);

    //Set Direction
    digitalWrite(dirPin, HIGH);

    delayMillis(1);

    int numSteps = 1;
    int i;
    int deay = 600;

    for(i = 0; i < numSteps; ++i){
        digitalWrite(stepPin, HIGH); delayMicros(deay);
        digitalWrite(stepPin, LOW);  delayMicros(deay);
    }

    delayMillis(100);

    digitalWrite(NenablePin, HIGH);
}


void setup(){
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    pinMode(NenablePin, OUTPUT);
    pinMode(sleepPin, OUTPUT);

    // Start with motor off, but enabled
    digitalWrite(sleepPin, LOW);
    digitalWrite(NenablePin, LOW);
}

void main(void){
    pioInit();
    setup();

    delayMillis(3);

    printf("3...\n");
    delayMillis(1000);
    printf("2...\n");
    delayMillis(1000);
    printf("1...\n");
    delayMillis(1000);
    int i;

    step(HIGH);
    // printf("Step 1\n");
    
    //delayMillis(1000);

    // step(HIGH);
    // printf("Done\n");
/*
    for(i = 0; i < 20; i++){
        step(HIGH);
        delayMillis(500);
    }
*/    

/*

    // // Test Sleep
    for(i = 0; i < 50; i++){
        testSleep();
        delayMillis(100);
    } 
    digitalWrite(sleepPin, LOW);
    delayMillis(2000);
       
    digitalWrite(sleepPin, HIGH);
    // Test Enable
    for(i = 0; i < 50; i++){
        testSleep();
        delayMillis(10);
    } 
    digitalWrite(NenablePin, LOW);
    digitalWrite(sleepPin,LOW);
 */
 
    // Test: Mobement when disable -> Can move when disabled and still awake
    /*
    digitalWrite(NenablePin, LOW);
    digitalWrite(sleepPin, HIGH);
    delayMillis(2000);
    
    printf("End Sleep, start Enable test\n");
    
    digitalWrite(NenablePin, HIGH);
    delayMillis(2000);
    
    digitalWrite(NenablePin, LOW);
    digitalWrite(sleepPin, LOW);
    */
    
    // Test: Home Position of Enableing
/*
    digitalWrite(sleepPin, HIGH);   
    digitalWrite(NenablePin, LOW);
     
    for(i = 0; i < 20; ++i){
        digitalWrite(NenablePin, HIGH);
        delayMillis(1000);
        printf("Enable #%d\n", i);
        digitalWrite(NenablePin, LOW);
        delayMillis(1000);
    }
    
    delayMillis(2000);
    
    for(i = 0; i < 20; ++i){
        digitalWrite(sleepPin, HIGH);
        delayMillis(1000);
        printf("Sleep #%d\n", i);
        digitalWrite(sleepPin, LOW);
        delayMillis(1000);
    }
*/
    digitalWrite(sleepPin, LOW);   
    


}



#endif
