/**
 * \class MotorDriver
 * \brief Interface for the hardware.
 * \details Exposes a method to turn any of the six attached motors. Abstracts
 *  details such as the pins each motor is connected to.
 */

typedef int PinID;

enum MotorID : char {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FRONT,
    BACK
}

enum MotorDirection : bool {
    CLOCKWISE,
    COUNTERCLOCKWISE
}

class MotorDriver {
public:

    /**
     * \brief Create an instance of a MotorDriver and configure it for the
     *  given hardware setup.
     * \param stepPinX The pin for motor X indicating how many turns to make.
     * \param dirPinX The pin for motor X indicating which direction to rotate.
     */
    MotorDriver(PinID stepPinL, PinID dirPinL,
                PinID stepPinR, PinID dirPinR,
                PinID stepPinU, PinID dirPinU,
                PinID stepPinD, PinID dirPinD,
                PinID stepPinF, PinID dirPinF,
                PinID stepPinB, PinID dirPinB);

    /**
     * \brief Turn a motor.
     * \param motor ID of the motor to turn.
     * \param turns The number of 90 degree turns to make.
     * \param dir The direction of the turns.
     */
    void step(MotorID motor, int turns, MotorDirection dir);

private:

    /// Determines the speed of the motor.
    static const int DELAY = 500; 

    /// Hardware connections.
    PinID stepPinL_;
    PinID dirPinL_;

    PinID stepPinR_;
    PinID dirPinR_;

    PinID stepPinU_;
    PinID dirPinU_;

    PinID stepPinD_;
    PinID dirPinD_;

    PinID stepPinF_;
    PinID dirPinF_;

    PinID stepPinB_;
    PinID dirPinB_;
}