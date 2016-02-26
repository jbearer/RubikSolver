# RubikSolver
5C Hackathon Project, Fall 2015

Interface with a robot designed to turn the faces of a cube. Analyze images of the cube, generate a solution, and solve the physical cube using the robot.

# External dependencies

* boost/serialization, boost/archive  </br>
  Used by RubikSolver/Cube Solver 2/Cube Solver 2/cube.cpp for serializing a hashtable for quick storage and retreival between executions.

* Windows OS  </br>
  In SerialComm/src/serial.\*, implementation of serial communication between computer and arduino is currently Windows-dependent
