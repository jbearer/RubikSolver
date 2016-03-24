/**
 * \file main.cpp
 * Uses the Cube class and the serial protocol to solve a scrambled cube and
 * send the solution to the serial port (where it should be received by an
 * Arduino).
 */

#include "MotorControl/src/arduino-incl/comm-protocol.h"
#include "SerialComm/src/serialstream.h"
#include "Cube Solver 2/Cube Solver 2/cube.h"
#include <vector>

using namespace CommProtocol;
using namespace SerialComm;

int main() {

	#define stringize(x) #x
    const char* PORT = stringize(COM_PORT);

    // TODO change this to init the real cube once we have image processing
    Cube::initChoose();
    // TODO load a map or something???

    // TODO initialize this somehow???
    Cube cube;

    std::vector<MoveInstruction> solution = Cube::solve(cube);

    serialstream serial(PORT);
    for (auto turn : solution) {
        serial << turn;
    }

    return 0;
}
