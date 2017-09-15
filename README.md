# RubikSolver
5C Hackathon Project, Fall 2015

Interface with a robot designed to turn the faces of a cube. Analyze images of the cube, generate a solution, and solve the physical cube using the robot.
Uses Kociemba's Two-Phase algorithm to solve the Rubik's Cube
# External dependencies

* boost/serialization, boost/archive  </br>
  Used by `end_maps.cpp` and `turn_tables.cpp` so that it doesn't need to recalculate the maps each time

* OpenCV
  Used by Image Processing.
