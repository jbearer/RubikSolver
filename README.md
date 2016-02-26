# RubikSolver
5C Hackathon Project, Fall 2015

Interface with a robot designed to turn the faces of a cube. Analyze images of the cube, generate a solution, and solve the physical cube using the robot.

# External dependencies

* boost/serialization, boost/archive  </br>
  Used by `RubikSolver/Cube Solver 2/Cube Solver 2/cube.cpp` for serializing a hashtable for quick storage and retreival between executions.

* Windows OS  </br>
  In `SerialComm\src\serial.\*`, implementation of serial communication between computer and arduino is currently Windows-dependent

* Arduino cores  </br>
  Used for compiling Arduino sketches. To compile the sketches from the makefile (to see compiler output when running make) the variable
  AVR_PATH in the MotorControl makefile needs to be set to the path ending `Arduino/hardware/arduino/avr`. This will depend on where the
  Arduino libraries are installed on a particular PC. This variable is optional, as you can always build sketches from the Arduino IDE
  and use `make silent` when building in MotorControl to prevent immediate compilation.

* AVR  (optional)  </br>
  Used to compile Arduino sketches via makefile. To build without this dependency, use `make silent` when building in MotorControl and
  compile Arduino sketches from the Arduino IDE. An AVR distribution for Windows is available [here](http://andybrown.me.uk/2012/04/28/avr-gcc-4-7-0-and-avr-libc-1-8-0-compiled-for-windows/)
