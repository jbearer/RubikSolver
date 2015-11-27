package mainPkg;
import java.util.ArrayList;
import java.util.Arrays;


public class Main {
	
	private static final int ANY = -1;
	private static final int[] ANY_ROW = {ANY, ANY, ANY};
	private static final int[][] ANY_FACE = {ANY_ROW, ANY_ROW, ANY_ROW};
	
	public static final int NUM_FACES = 6;

	public static void main(String args[]) {
		int[][] front = new int[3][3];
		int[][] right = new int[3][3];
		int[][] back = new int[3][3];
		int[][] left = new int[3][3];
		int[][] up = new int[3][3];
		int[][] down = new int[3][3];

		CubeParser cubeToParse = new CubeParser("res/Cube1/front.png", "res/Cube1/back.png",
				"res/Cube1/left.png", "res/Cube1/right.png",
				"res/Cube1/up.png", "res/Cube1/down.png");

		Cube myCube = cubeToParse.getCube();
		
		printCube(myCube);
		
		front[1][0] = Cube.ORANGE;
		right[1][0] = Cube.ORANGE;
		right[2][0] = Cube.ORANGE;

		//printCube(myCube);

		//solve(myCube);

	}

	public static void solve(Cube cube) {

		System.out.println("step 1");
		step1(cube);
		// printCube(cube);

		System.out.println("step 2");
		step2(cube);
		// printCube(cube);

		System.out.println("step 3");
		step3(cube);
		// printCube(cube);

		System.out.println("step 4");
		step4(cube);
		// printCube(cube);

		System.out.println("step 5");
		step5(cube);
		// printCube(cube);

		System.out.println("step 6");
		step6(cube);
		// printCube(cube);

		System.out.println("step 7");
		step7(cube);
		// printCube(cube);

		System.out.println("step 8");
		step8(cube);
		
		printCube(cube);
		
		System.out.println(cube.steps);
		

		Step.condense(cube.steps);

		System.out.println(cube.steps);
		System.out.println(cube.steps.size() + " steps");
	}

	// //////////////// step 1, the daisy /////////////////

	// finds the corresponding piece on the up face of the cube
//	public static int[] corrTopSpot(int row, int col, int face) {
//
//		if (face == Cube.FRONT) {
//			int[] result = { row, col };
//			return result;
//		} else if (face == Cube.BACK) {
//			int[] result = { 2 - row, 2 - col };
//			return result;
//		} else if (face == Cube.LEFT) {
//			if (row == 1) {
//				int[] result = { col, row };
//				return result;
//			} else {
//				int[] result = { 2 - col, 2 - row };
//				return result;
//			}
//		} else if (face == Cube.RIGHT) {
//			int[] result = { 2 - col, row };
//			return result;
//		} else if (face == Cube.DOWN) {
//			if (row == 1) {
//				int[] result = { row, col };
//				return result;
//			} else {
//				int[] result = { 2 - row, col };
//				return result;
//			}
//		} else {
//			int[] result = { -1, -1 };
//			return result;
//		}
//	}

	public static void step1(Cube cube) {
		
		int face = Cube.FRONT;
		while (!solved1(cube)) {

			// white square on top face
			

			if (cube.data[face][0][1] == Cube.WHITE) {
				cube.turnFront(1, face);
				cube.turnUp(3, face);
				cube.turnRight(1, face);
			}

			if (cube.data[face][1][0] == Cube.WHITE) {
				while (cube.upFrom(1, 0, face) == Cube.WHITE) {
					cube.turnUp(1);
				}
				cube.turnLeft(3, face);
			}

			if (cube.data[face][1][2] == Cube.WHITE) {
				while (cube.upFrom(1, 2, face) == Cube.WHITE) {
					cube.turnUp(1);
				}
				cube.turnRight(1, face);
			}

			if (cube.data[face][2][1] == Cube.WHITE) {
				while (cube.upFrom(1, 2, face) == Cube.WHITE) {
					cube.turnUp(1);
				}
				cube.turnFront(3, face);
				cube.turnRight(1, face);
			}

			if (cube.downFrom(0, 1, face) == Cube.WHITE) {
				while (cube.upFrom(2, 1, face) == Cube.WHITE) {
					cube.turnUp(1);
				}
				cube.turnFront(2, face);
			}
			
			face = rightFace(face);
			
			// PREVIOUS STEP 1
			// // white squares in other three positions (and no obstructions)
			// for (int face : Cube.SIDES) {
			//
			// if (cube.data[face][1][0] == Cube.WHITE) {
			// int[] corrTopSpot = corrTopSpot(1, 0, face);
			// while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
			// cube.turnUp(1);
			// face = leftFace(face);
			// }
			// cube.turnLeft(3, face);
			//
			// } else if (cube.data[face][1][2] == Cube.WHITE) {
			// int[] corrTopSpot = corrTopSpot(1, 2, face);
			// while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
			// cube.turnUp(1);
			// face = leftFace(face);
			// }
			// cube.turnRight(1, face);
			// } else if (cube.data[face][2][1] == Cube.WHITE) {
			// int[] corrTopSpot = corrTopSpot(2, 1, face);
			// while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
			// cube.turnUp(1, face);
			// face = leftFace(face);
			// }
			// cube.turnDown(1, face);
			// cube.turnRight(1, face);
			// cube.turnFront(3, face);
			// cube.turnRight(3, face);
			// }
			// }
			//
			// // White squares on the bottom
			// int face = 0;
			// if (cube.down[0][1] == Cube.WHITE) {
			// int[] corrTopSpot = corrTopSpot(0, 1, Cube.DOWN);
			// while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
			// cube.turnUp(1, face);
			// // face = leftFace(face);
			//
			// }
			// cube.turnFront(2, face);
			// face = 0;
			// } else if (cube.down[1][0] == Cube.WHITE) {
			// int[] corrTopSpot = corrTopSpot(1, 0, 5);
			// while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
			// cube.turnUp(1, face);
			// // face = leftFace(face);
			//
			// }
			// cube.turnLeft(2, face);
			//
			// face = 0;
			// } else if (cube.down[1][2] == Cube.WHITE) {
			// int[] corrTopSpot = corrTopSpot(1, 2, 5);
			// while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
			// cube.turnUp(1, face);
			// // face = leftFace(face);
			//
			// }
			// cube.turnRight(2, face);
			//
			// face = 0;
			// } else if (cube.down[2][1] == Cube.WHITE) {
			// int[] corrTopSpot = corrTopSpot(2, 1, 5);
			// while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
			// cube.turnUp(1, face);
			// // face = leftFace(face);
			//
			// }
			// cube.turnBack(2, face);
			// }
			//
		}
	}

	public static boolean solved1(Cube cube) {
		// for each face, checks if the bottom left corner on Up is WHITE 
		printCube(cube);
		for(int face : Cube.SIDES){
			if (cube.upFrom(2, 1, face) != Cube.WHITE){
				return false;
			}
		}
		return true;
	}

	// /////////////// step 2, the cross /////////////////

	public static void step2(Cube cube) {
		int face = Cube.FRONT;
		while (!solved2(cube)) {
			if (cube.data[Cube.FRONT][0][1] == Cube.FRONT
					&& cube.data[Cube.UP][2][1] == Cube.WHITE) {
				cube.turnFront(2, face);
				
			} else if (cube.data[Cube.BACK][0][1] == Cube.BACK
					&& cube.data[Cube.UP][0][1] == Cube.WHITE) {
				cube.turnBack(2, face);
				
			} else if (cube.data[Cube.LEFT][0][1] == Cube.LEFT
					&& cube.data[Cube.UP][1][0] == Cube.WHITE) {
				cube.turnLeft(2, face);
				
			} else if (cube.data[Cube.RIGHT][0][1] == Cube.RIGHT
					&& cube.data[Cube.UP][1][2] == Cube.WHITE) {
				cube.turnRight(2, face);
			}
			cube.turnUp(1);
			// printCube(cube);
		}
	}

	// Alternate step 2, much less efficient
	// public static void step2(Cube cube) {
	// for( int face : Cube.SIDES){
	// for(int i = 0; i < 4; ++i){
	// if(cube.data[face][0][1] == face){
	// cube.turnFront(2, face);
	// }
	// cube.turnUp(1, face);
	// face = leftFace(face);
	// }
	// }
	// }

	public static boolean solved2(Cube cube) {
		printCube(cube);
		return cube.down[0][1] == Cube.WHITE 
				&& cube.down[1][0] == Cube.WHITE
				&& cube.down[1][2] == Cube.WHITE
				&& cube.down[2][1] == Cube.WHITE;
	}

	// /////////////// step 3, the bottom face /////////////////

	public static void step3(Cube cube) {
		while (!solved3(cube)) {

			// white in left corner of face. Rotate piece until its in the right
			// place
			// then do the algorithm
			for (int face : Cube.SIDES) {
				if (cube.data[face][0][0] == Cube.WHITE) {
					while (!(cube.isAdjacent(face, 0, 0, leftFace(face)) && cube
							.isAdjacent(face, 0, 0, face))) {
						cube.turnUp(1);
						face = leftFace(face);
					}
					// for(int i = 0; i < 5; i++){
					// LpUpLU(cube, face);
					// }
					cube.turnFront(1, face);
					cube.turnUp(1, face);
					cube.turnFront(3, face);
				}
			}

			// white in top right corner
			for (int face : Cube.SIDES) {
				if (cube.data[face][0][2] == Cube.WHITE) {

					while (!(cube.isAdjacent(face, 0, 2, rightFace(face)) && cube
							.isAdjacent(face, 0, 2, face))) {
						cube.turnUp(3);
						face = rightFace(face);
					}
					// position the cube correctly
					face = rightFace(face);

					LpUpLU(cube, face);
				}
			}
			// white in bottom left corner: pop it out
			for (int face : Cube.SIDES) {
				if (cube.data[face][2][0] == Cube.WHITE) {
					cube.turnFront(1, face);
					cube.turnUp(1);
					cube.turnFront(3, face);
				}
			}
			// white in bottom right corner: pop it out
			for (int face : Cube.SIDES) {
				if (cube.data[face][2][2] == Cube.WHITE) {
					cube.turnFront(3, face);
					cube.turnUp(3);
					cube.turnFront(1, face);
				}
			}

			// white on up face: bring it to the right face, do algorithm
			for (int i = 0; i < 4; ++i) {
				if (cube.up[2][2] == Cube.WHITE) {
					int face = 0;
					while (!(cube.isAdjacent(face, 0, 2, face) && cube.data[face][0][2] == rightFace(face))) {
						cube.turnUp(3);
						face = rightFace(face);
					}
					face = rightFace(face);

					for (int j = 0; j < 3; ++j) {
						LpUpLU(cube, face);
					}
				}
			}
		}
	}

	public static boolean solved3(Cube cube) {

		// check bottom face
		for (int row = 0; row < 3; row += 2) {
			for (int col = 0; col < 3; col += 2) {
				if (cube.down[row][col] != Cube.WHITE) {
					return false;
				}
			}
		}

		// check rest of bottom layer. pop up a piece of its incorrect
		for (int face : Cube.SIDES) {
			for (int col = 0; col < 3; col += 2) {
				if (cube.data[face][2][0] != face) {
					LpUpLU(cube, face);
					return false;
				}
			}
		}

		return true;
	}

	// left inverted, up inverted, left, down
	public static void LpUpLU(Cube cube, int face) {

		cube.turnLeft(3, face);
		cube.turnUp(3, face);
		cube.turnLeft(1, face);
		cube.turnUp(1, face);
	}

	// ///////////////// Bottom Two Layers ////////////////////

	public static void step4(Cube cube) {
		while (!solved4(cube)) {

			for (int face : Cube.SIDES) {

				// find pieces on the top layer with no yellow
				if (cube.data[face][0][1] != Cube.YELLOW
						&& cube.upFrom(2, 1, face) != Cube.YELLOW) {

					// move it until it's in the right place, then do the
					// algorithm
					while (cube.data[face][0][1] != face) {
						cube.turnUp(1);
						face = leftFace(face);
					}
					if (cube.isAdjacent(face, 0, 1, leftFace(face))) {
						leftEdgeFix(cube, face);
					} else {
						rightEdgeFix(cube, face);
					}
				}
			}

			// check for bad cases
			for (int face : Cube.SIDES) {
				if ((cube.data[face][1][0] != Cube.YELLOW && cube.data[face][1][0] != face)
						&& (cube.data[leftFace(face)][1][2] != Cube.YELLOW && cube.data[leftFace(face)][1][2] != leftFace(face))) {
					leftEdgeFix(cube, face);
				}
			}
		}
	}

	public static boolean solved4(Cube cube) {
		printCube(cube);
		for (int face : Cube.SIDES) {
			for (int col = 0; col < 3; col += 2) {
				if (cube.data[face][1][col] != face) {
					return false;
				}
			}
		}

		return true;
	}

	public static void leftEdgeFix(Cube cube, int face) {

		cube.turnUp(3, face);
		cube.turnLeft(3, face);
		cube.turnUp(1, face);
		cube.turnLeft(1, face);

		cube.turnUp(1, face);
		cube.turnFront(1, face);
		cube.turnUp(3, face);
		cube.turnFront(3, face);
	}

	public static void rightEdgeFix(Cube cube, int face) {

		cube.turnUp(1, face);
		cube.turnRight(1, face);
		cube.turnUp(3, face);
		cube.turnRight(3, face);

		cube.turnUp(3, face);
		cube.turnFront(3, face);
		cube.turnUp(1, face);
		cube.turnFront(1, face);
	}

	// ///////////////// Cross on Top ////////////////////

	public static void step5(Cube cube) {
		while (!solved5(cube)) {
			cube.turnUp(1);
			int cycles = 0;

			// after three times, this algorithm brings you back to the start
			while (!solved5(cube) && cycles < 3) {
				cube.turnFront(1, Cube.FRONT);
				cube.turnUp(1, Cube.FRONT);
				cube.turnRight(1, Cube.FRONT);
				cube.turnUp(3, Cube.FRONT);
				cube.turnRight(3, Cube.FRONT);
				cube.turnFront(3, Cube.FRONT);
				cycles++;
			}
		}
	}

	public static boolean solved5(Cube cube) {

		return cube.up[0][1] == Cube.YELLOW && cube.up[1][0] == Cube.YELLOW
				&& cube.up[2][1] == Cube.YELLOW && cube.up[1][2] == Cube.YELLOW;
	}

	// ///////////////// step 6, top face ///////////////

	public static void step6(Cube cube) {

		// looks for the fish pattern on top
		while (!solved6(cube)) {
			
			int patternFound = 0;
			
			for(int face : Cube.SIDES){
				if (cube.upFrom(2, 0, face) == Cube.YELLOW
						&& cube.upFrom(2, 2, face) != Cube.YELLOW
						&& cube.upFrom(0, 2, face) != Cube.YELLOW
						&& cube.upFrom(0, 0, face) != Cube.YELLOW){
					patternFound = 1;
					doTheFun(cube, face);
				}
			}

			if( patternFound == 0 ) {
				// do the algorithm with yellow at the top left
				int face = Cube.FRONT;
				while (cube.data[face][0][0] != Cube.YELLOW) {
					face = leftFace(face);
				}
				doTheFun(cube, face);
			}
		}
	}

	// solved when the yellow corner pieces are in place
	public static boolean solved6(Cube cube) {

		return cube.up[0][0] == Cube.YELLOW && cube.up[0][2] == Cube.YELLOW
				&& cube.up[2][0] == Cube.YELLOW && cube.up[2][2] == Cube.YELLOW;
	}

	// do the fun to make the up face complete
	public static void doTheFun(Cube cube, int face) {

		cube.turnRight(1, face);
		cube.turnUp(1, face);
		cube.turnRight(3, face);
		cube.turnUp(1, face);
		cube.turnRight(1, face);
		cube.turnUp(2, face);
		cube.turnRight(3, face);

	}

	// ///////////////// step 7, the whatever its called ///////////////////

	public static void step7(Cube cube) {
		while (!solved7(cube)) {

			int face = 0;
			while (face != Cube.UP
					&& (cube.data[face][0][0] != cube.data[face][0][2])) {
				++face;
			}

			int opFace = opFace(face);

			cube.turnRight(3, opFace);
			cube.turnFront(1, opFace);
			cube.turnRight(3, opFace);
			cube.turnBack(2, opFace);

			cube.turnRight(1, opFace);
			cube.turnFront(3, opFace);
			cube.turnRight(3, opFace);
			cube.turnBack(2, opFace);

			cube.turnRight(2, opFace);
		}

		// once done, small adjustment to up before next step
		while (cube.front[0][0] != Cube.GREEN) {
			cube.turnUp(1);
		}
	}

	// checks if corners match each other
	public static boolean solved7(Cube cube) {

		// if both corner's colors don't match the face's color
		for (int face : Cube.SIDES) {
			if (!(cube.data[face][0][0] == cube.data[face][0][2])) {
				return false;
			}
		}
		return true;
	}

	// /////////////// step 8, solve the cube :) ///////////////

	public static void step8(Cube cube) {
		while (!solved8(cube)) {

			int face = 0;
			while (face < Cube.UP && (cube.data[face][0][1] != face)) {
				++face;
			}

			int opFace = opFace(face);

			cube.turnFront(2, opFace);
			cube.turnUp(1, opFace);
			cube.turnRight(3, opFace);
			cube.turnLeft(1, opFace);
			cube.turnFront(2, opFace);
			cube.turnRight(1, opFace);
			cube.turnLeft(3, opFace);
			cube.turnUp(1, opFace);
			cube.turnFront(2, opFace);
		}
	}

	// checks to see if the top two are both green. if they are, returns true
	// otherwise, rotates them
	public static boolean solved8(Cube cube) {

		for (int turns = 0; turns < 4; turns++) {

			if (cube.front[0][1] == Cube.GREEN
					&& cube.front[0][0] == Cube.GREEN
					&& cube.right[0][1] == Cube.ORANGE) {
				System.out.println("SOLVED!!!!!!");
				return true;
			}
		}
		return false;
	}

	// //////////////// print cube ///////////////////////

	public void printFace(int[][] plane) {
		for (int[] row : plane) {
			for (int square : row) {
				System.out.print(square);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();
	}

	public static void printCube(Cube cube) {

		for (int row = 0; row < 3; ++row) {
			for (int i = 0; i < 8; ++i) {
				System.out.print(" ");
			}
			for (int col = 0; col < 3; ++col) {
				System.out.print(cube.up[row][col]);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();

		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				System.out.print(cube.left[row][col]);
				System.out.print(" ");
			}
			System.out.print("  ");
			for (int col = 0; col < 3; ++col) {
				System.out.print(cube.front[row][col]);
				System.out.print(" ");
			}
			System.out.print("  ");
			for (int col = 0; col < 3; ++col) {
				System.out.print(cube.right[row][col]);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();

		for (int row = 0; row < 3; ++row) {
			for (int i = 0; i < 8; ++i) {
				System.out.print(" ");
			}
			for (int col = 0; col < 3; ++col) {
				System.out.print(cube.down[row][col]);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();

		for (int row = 0; row < 3; ++row) {
			for (int i = 0; i < 8; ++i) {
				System.out.print(" ");
			}
			for (int col = 0; col < 3; ++col) {
				System.out.print(cube.back[row][col]);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();
		System.out.println();
		System.out.println();

	}

	// /////////////// OTHER HELPER METHODS ////////////////////////

	public static int opFace(int face) {
		if (face == Cube.FRONT) {
			return Cube.BACK;
		} else if (face == Cube.BACK) {
			return Cube.FRONT;
		} else if (face == Cube.LEFT) {
			return Cube.RIGHT;
		} else {
			return Cube.LEFT;
		}
	}

	public static int leftFace(int face) {
		if (face == Cube.FRONT) {
			return Cube.LEFT;
		} else if (face == Cube.BACK) {
			return Cube.RIGHT;
		} else if (face == Cube.RIGHT) {
			return Cube.FRONT;
		} else {
			return Cube.BACK;
		}
	}

	public static int rightFace(int face) {
		if (face == Cube.FRONT) {
			return Cube.RIGHT;
		} else if (face == Cube.BACK) {
			return Cube.LEFT;
		} else if (face == Cube.RIGHT) {
			return Cube.BACK;
		} else {
			return Cube.FRONT;
		}
	}
	
	/////////////////////////// PATTERN MATCHING //////////////////////////////
	
	// Returns true if the given pattern can be made to match the cube
	// Given a reference, faceFrom, from which to compare with the pattern's front face
	public static boolean matchesPattern(Cube cube, int[][][] pattern, int faceFrom) {
		
		int patternFace = Cube.FRONT;
		
		// starting from the pattern's "front" and the cube's reference face
		// ensures that the pattern matches the cube at each of the side faces
		for(int i = 0; i < 4; ++i){
			for (int row = 0; row < 3; row++) {
				for (int col = 0; col < 3; col++) {
					if (!matchesTile(cube.data[faceFrom][row][col],
							pattern[patternFace][row][col])) {
						return false;
					}
				}
			}
			faceFrom = rightFace(faceFrom);
			patternFace = rightFace(patternFace);
		}
		
		// faceFrom should be back at its original face
		// Now it checks the top and the bottom,
		// still taking into account the reference face
		
		for(int row = 0; row < 3; row++) {
			for(int col = 0; col < 3; col++) {
				if( !matchesTile(cube.upFrom(row, col, faceFrom), pattern[Cube.UP][row][col])
						|| !matchesTile(cube.downFrom(row, col, faceFrom), pattern[Cube.DOWN][row][col])){
					return false;
				}
			}
		}
		
		return true;
	}
	
	// If the pattern is only on the front face, then call this method to only compare
	// with the front face of the cube
	public static boolean matchesFrontPattern(Cube cube, int[][] frontPattern, int faceFrom) {
		
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				if (!matchesTile(cube.data[faceFrom][row][col],
						frontPattern[row][col])) {
					return false;
				}
			}
		}
		
		return true;
	}
	
	// if the pattern is only on the up face, then call this method to
	// only compare with the up face of the cube
	public static boolean matchesUpPattern(Cube cube, int[][] upPattern, int faceFrom) {
		
		for(int row = 0; row < 3; row++) {
			for(int col = 0; col < 3; col++) {
				if( !matchesTile(cube.upFrom(row, col, faceFrom),
						upPattern[row][col])) {
					return false;
				}
			}
		}
		
		return true;
	}
	
	public static boolean matchesTile(int cubie, int pattern) {
		
		// if the pattern tile represents a color
		// return true if the colors are equal
		if(pattern < NUM_FACES){
			return cubie == pattern;
		}
		
		// if the pattern is an anti-color
		// return true if the colors are not equals
		else if (pattern >= NUM_FACES){
			return cubie != pattern - NUM_FACES;
		}
		
		// otherwise, it must be ANY.  return true
		else {
			return true;
		}
	}
}
