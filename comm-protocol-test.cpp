/**
 * \file comm-test.cpp
 * \brief Program to test serial communication between PC and Arduino.
 * \details Sends a predetermined sequence of move commands over the serial
 *  port. These are sent in the form of MoveInstructions (defined in
 *  comm-protocol.h) which are then decoded by the Arduino.
 */

#include "MotorControl/src/arduino-incl/comm-protocol.h"
#include "SerialComm/src/serialstream.h"
#include <vector>
#include <iostream>

using namespace CommProtocol;
using namespace SerialComm;

int main() {

    #define stringize(x) #x
    #define xstr(x) stringize(x)
    const char* PORT = "COM8";
    std::cout << "Using port: "<< PORT << std::endl;

    std::vector<MoveInstruction> sequence = {
      /* LEFT, RIGHT, UP, DOWN, FRONT, BACK,

        LEFT_INVERTED, RIGHT_INVERTED, UP_INVERTED, 
        DOWN_INVERTED, FRONT_INVERTED, BACK_INVERTED,

        LEFT_2, RIGHT_2, UP_2, DOWN_2, FRONT_2, 
        BACK_2*/
      LEFT, LEFT_INVERTED, LEFT_2
    };

    serialstream serial(PORT);
    for (auto turn : sequence) {
        serial << turn;
    }

    return 0;
}
