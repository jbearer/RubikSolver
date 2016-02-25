/**
 * \file motor-control.h
 * \brief Defines constants and functions used by Arduino sketches that operate
 *  the motor. Defines a protocol for serial communication between such a sketch
 *  and a PC program.
 */

 #include "motor-driver.h"

namespace MotorControl {

    typedef char SerialMessage;

    /**
     * \enum MoveInstruction
     * \brief Constants representing a single cube action.
     */
    enum MoveInstruction : SerialMessage {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        FRONT,
        BACK,

        LEFT_2,
        RIGHT_2,
        UP_2,
        DOWN_2,
        FRONT_2,
        BACK_2,

        LEFT_INVERTED,
        RIGHT_INVERTED,
        UP_INVERTED,
        DOWN_INVERTED,
        FRONT_INVERTED,
        BACK_INVERTED
    }

    typedef (void)(*FaceTurn)(MotorDriver);

    /// Functions that activate the motors.
    const FaceTurn LEFT;
    const FaceTurn RIGHT;
    const FaceTurn UP;
    const FaceTurn DOWN;
    const FaceTurn FRONT;
    const FaceTurn BACK;

    const FaceTurn LEFT_2;
    const FaceTurn RIGHT_2;
    const FaceTurn UP_2;
    const FaceTurn DOWN_2;
    const FaceTurn FRONT_2;
    const FaceTurn BACK_2;

    const FaceTurn LEFT_INVERTED;
    const FaceTurn RIGHT_INVERTED;
    const FaceTurn UP_INVERTED;
    const FaceTurn DOWN_INVERTED;
    const FaceTurn FRONT_INVERTED;
    const FaceTurn BACK_INVERTED;

    /**
     * \brief Get the function associated with a MoveInstruction code.
     */
    FaceTurn getAction(MoveInstruction);
}
