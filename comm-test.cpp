/**
 * \file comm-test.cpp
 * \brief Program to test serial communication between PC and Arduino.
 * \details Sends a predetermined sequence of move commands over the serial
 *  port. These are sent in the form of function pointers of type
 *  MotorControl::FaceTurn, and can thus be evaluated immediately by the
 *  Arduino.
 */

#include "MotorControl/src/motor-control.h"
#include "SerialComm/src/serialstream.h"
#include <vector>

using namespace MotorControl;
using namespace SerialComm;

int main() {

    const char* PORT = "COM4";

    std::vector<FaceTurn> sequence = {
        TURN_LEFT, TURN_RIGHT, TURN_UP, TURN_DOWN, TURN_FRONT, TURN_BACK,

        TURN_LEFT_INVERTED, TURN_RIGHT_INVERTED, TURN_UP_INVERTED, 
        TURN_DOWN_INVERTED, TURN_FRONT_INVERTED, TURN_BACK_INVERTED,

        TURN_LEFT_2, TURN_RIGHT_2, TURN_UP_2, TURN_DOWN_2, TURN_FRONT_2, 
        TURN_BACK_2
    }

    serialstream serial(PORT);
    for (auto turn : sequence) {
        stream << turn;
    }

    return 0;
}
