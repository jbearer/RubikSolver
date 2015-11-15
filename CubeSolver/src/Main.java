import java.util.ArrayList;

public class Main {

	
	public static void main(String args[]) {
		int[][] front = new int[3][3];
		int[][] back = new int[3][3];
		int[][] left = new int[3][3];
		int[][] right = new int[3][3];
		int[][] up = new int[3][3];
		int[][] down = new int[3][3];
		
		ImageParser.parseCube("res/Cube2/front.png", "res/Cube2/back.png", "res/Cube2/left.png", "res/Cube2/right.png", "res/Cube2/up.png", "res/Cube2/down.png",
				front, back, left, right, up, down);
		
		front[1][0] = 3;
		right[1][0] = 3;
		right[2][0] = 3;
		
		Cube myCube = new Cube(front, back, left, right, up, down);
		solve(myCube);
		//printCube(myCube);
	}
	
	
	public static void solve(Cube cube){
		
		System.out.println("step 1");
		step1(cube);
		//printCube(cube);
		
		System.out.println("step 2");
		step2(cube);
		//printCube(cube);
		
		System.out.println("step 3");
		step3(cube);
		//printCube(cube);
		
		System.out.println("step 4");
		step4(cube);
		//printCube(cube);
		
		System.out.println("step 5");
		step5(cube);
		//printCube(cube);
		
		System.out.println("step 6");
		step6(cube);
		//printCube(cube);
		
		step7(cube);
		//printCube(cube);
		
		step8(cube);
		//printCube(cube);
		
		System.out.println(cube.steps);
		System.out.println(cube.steps.size());
	}

	// /////////////// step 1, the daisy /////////////////

	public static int[] corrTopSpot(int row, int col, int face) {

		if (face == Cube.FRONT) {
			int[] result = { row, col };
			return result;
		} else if (face == Cube.BACK) {
			int[] result = { 2 - row, 2 - col };
			return result;
		} else if (face == Cube.LEFT) {
			if (row == 1) {
				int[] result = { col, row };
				return result;
			} else {
				int[] result = { 2 - col, 2 - row };
				return result;
			}
		} else if (face == Cube.RIGHT) {
			int[] result = { 2 - col, row };
			return result;
		} else if (face == Cube.DOWN) {
			if (row == 1) {
				int[] result = { row, col };
				return result;
			} else {
				int[] result = { 2 - row, col };
				return result;
			}
		} else {
			int[] result = { -1, -1 };
			return result;
		}
	}


public static void step1(Cube cube){
		while (!solved1(cube)) {
			
			// white square on top face
			for (int face = 0; face < 4; face++) {
				
				if (cube.data[face][0][1] == Cube.WHITE) {
					cube.turnFront(1, face);
					cube.turnUp(3, face);
					cube.turnRight(1, face);
				}
			}

			// white squares in other three positions (and no obstructions)
			for (int face = 0; face < 4; face++) {
				
				if (cube.data[face][1][0] == Cube.WHITE) {
					int[] corrTopSpot = corrTopSpot(1, 0, face);
					while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
						cube.turnUp(1);
						face = leftFace(face);
					}
					cube.turnLeft(3, face);

				} else if (cube.data[face][1][2] == Cube.WHITE) {
					int[] corrTopSpot = corrTopSpot(1, 2, face);
					while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
						cube.turnUp(1);
						face = leftFace(face);
					}
					cube.turnRight(1, face);
				} else if (cube.data[face][2][1] == Cube.WHITE) {
					int[] corrTopSpot = corrTopSpot(2, 1, face);
					while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
						cube.turnUp(1, face);
						face = leftFace(face);
					}
					cube.turnDown(1, face);
					cube.turnRight(1, face);
					cube.turnFront(3, face);
					cube.turnRight(3, face);
				}
			}

			// White squares on the bottom
			int face = 0;
			if (cube.down[0][1] == Cube.WHITE) {
				int[] corrTopSpot = corrTopSpot(0, 1, Cube.DOWN);
				while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
					cube.turnUp(1, face);
//					face = leftFace(face);
					
				}
				cube.turnFront(2, face);
				face = 0;
			} else if (cube.down[1][0] == Cube.WHITE) {
				int[] corrTopSpot = corrTopSpot(1, 0, 5);
				while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
					cube.turnUp(1, face);
//					face = leftFace(face);
					
				}
				cube.turnLeft(2, face);
				
				face = 0;
			} else if (cube.down[1][2] == Cube.WHITE) {
				int[] corrTopSpot = corrTopSpot(1, 2, 5);
				while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
					cube.turnUp(1, face);
//					face = leftFace(face);
					
				}
				cube.turnRight(2, face);
				
				face = 0;
			} else if (cube.down[2][1] == Cube.WHITE) {
				int[] corrTopSpot = corrTopSpot(2, 1, 5);
				while (cube.up[corrTopSpot[0]][corrTopSpot[1]] == Cube.WHITE) {
					cube.turnUp(1, face);
//					face = leftFace(face);
					
				}
				cube.turnBack(2, face);
			}
			
		}
	}

	public static boolean solved1(Cube cube) {
		return cube.up[0][1] == Cube.WHITE && cube.up[1][0] == Cube.WHITE
				&& cube.up[1][2] == Cube.WHITE && cube.up[2][1] == Cube.WHITE;
	}

	// /////////////// step 2, the cross /////////////////

	public static void step2(Cube cube) {
		while (!solved2(cube)) {
			if (cube.data[Cube.FRONT][0][1] == cube.data[Cube.FRONT][1][1]
					&& cube.data[Cube.UP][2][1] == Cube.WHITE) {
				cube.turnFront(2, 0);
			} else if (cube.data[Cube.BACK][0][1] == cube.data[Cube.BACK][1][1]
					&& cube.data[Cube.UP][0][1] == Cube.WHITE) {
				cube.turnFront(2, 1);
			} else if (cube.data[Cube.LEFT][0][1] == cube.data[Cube.LEFT][1][1]
					&& cube.data[Cube.UP][1][0] == Cube.WHITE) {
				cube.turnFront(2, 2);
			} else if (cube.data[Cube.RIGHT][0][1] == cube.data[Cube.RIGHT][1][1]
					&& cube.data[Cube.UP][1][2] == Cube.WHITE) {
				cube.turnFront(2, 3);
			}
			cube.turnUp(1);
			//printCube(cube);
		}
	}

public static boolean solved2(Cube cube){
return cube.down[0][1] == Cube.WHITE &&
	cube.down[1][0] == Cube.WHITE &&
	cube.down[1][2] == Cube.WHITE &&
	cube.down[2][1] == Cube.WHITE;
}
	
///////////////// step 3, the bottom face /////////////////
	
	public static void step3(Cube cube){
		while(!solved3(cube)){
			
			for(int face = 0; face < Cube.UP; face++){
				if(cube.data[face][0][0] == Cube.WHITE){
					System.out.println("if 1");
					while(!(cube.isAdjacent(face, 0, 0, leftFace(face)) &&
							cube.isAdjacent(face, 0, 0, face))){
						cube.turnUp(1);
						face = leftFace(face);
					}
					for(int i = 0; i < 5; i++){
						LpUpLU(cube, face);
					}
				}
			}
			
			//printCube(cube);
			
			for(int face = 0; face < Cube.UP; face++){
				if(cube.data[face][0][2] == Cube.WHITE){
					System.out.println("if 2");
					while(!(cube.isAdjacent(face, 0, 2, rightFace(face)) &&
							cube.isAdjacent(face, 0, 2, face))){
						cube.turnUp(3);
						face = rightFace(face);
					}
					face = rightFace(face);
					LpUpLU(cube, face);
				}
			}
			
			for(int face = 0; face < Cube.UP; face++){
				if(cube.data[face][2][0] == Cube.WHITE){
					System.out.println("if 3");
					cube.turnFront(1,face);
					cube.turnUp(1);
					cube.turnFront(3, face);
				}
			}
			
			for(int face = 0; face < Cube.UP; face++){
				if(cube.data[face][2][2] == Cube.WHITE){
					System.out.println("if 4");
					cube.turnFront(3,face);
					cube.turnUp(3);
					cube.turnFront(1, face);
				}
			}
			
			for(int i = 0; i < 4; ++i){
				if(cube.up[2][2] == Cube.WHITE){
					System.out.println("if 5");
					int face = 0;
					while(!(cube.isAdjacent(face, 0, 2, face) && 
							cube.data[face][0][2] == rightFace(face))){
						cube.turnUp(3);
						face = rightFace(face);
					}
					face = rightFace(face);
					
					for(int j=0; j < 3; ++j){
						LpUpLU(cube,face);
					}
				}
			}
		}
	}
	
	public static boolean solved3(Cube cube){
		
		//check bottom face
		for(int row = 0; row < 3; row += 2){
			for(int col = 0; col < 3; col += 2){
				if(cube.down[row][col] != Cube.WHITE){
					return false;
				}
			}
		}
		
		//check rest of bottom layer
		for(int face = 0; face < Cube.UP; face++){
			for(int col = 0; col < 3; col += 2){
				if(cube.data[face][2][col] != face){
					return false;
				}
			}
		}
		
		return true;
	}
	
	// left inverted, up inverted, left, down
	public static void LpUpLU(Cube cube, int face){
		
		cube.turnLeft(3, face);
		cube.turnUp(3, face);
		cube.turnLeft(1, face);
		cube.turnUp(1, face);
	}
	
	/////////////////// Bottom Two Layers ////////////////////
	
	public static void step4(Cube cube){
		while(!solved4(cube)){
			
			for(int face = 0; face < Cube.UP; face++){
				if(cube.data[face][0][1] != Cube.YELLOW && !cube.isAdjacent(face, 0, 1, Cube.YELLOW)){
					
					while(cube.data[face][0][1] != face){
						cube.turnUp(1);
						face = leftFace(face);
					}
					int[] top = cube.corrTopSpot(2, 1, face);
					if(cube.up[top[0]][top[1]] == leftFace(face)){
						leftEdgeFix(cube, face);
					} else {
						rightEdgeFix(cube, face);
					}
				}
			}
			
			// check for bad cases
			for(int face = 0; face < Cube.UP; face++){
				if((cube.data[face][1][0] != Cube.YELLOW && cube.data[face][1][0] != face) &&
						(cube.data[leftFace(face)][1][2] != Cube.YELLOW && cube.data[leftFace(face)][1][2] != leftFace(face))){
					leftEdgeFix(cube, face);
				}
			}
		}
	}
	
	
	
	public static boolean solved4(Cube cube){
		
		for(int face = 0; face < Cube.UP; face++){
			for(int col = 0; col < 3; col += 2){
				if(cube.data[face][1][col] != face){
					return false;
				}
			}
		}
		
		return true;
	}
	
	public static void leftEdgeFix(Cube cube, int face){
		
		cube.turnUp(3, face);
		cube.turnLeft(3, face);
		cube.turnUp(1, face);
		cube.turnLeft(1, face);
		
		cube.turnUp(1, face);
		cube.turnFront(1, face);
		cube.turnUp(3, face);
		cube.turnFront(3, face);
	}
	
	public static void rightEdgeFix(Cube cube, int face){
		
		cube.turnUp(1, face);
		cube.turnRight(1, face);
		cube.turnUp(3, face);
		cube.turnRight(3, face);
		
		cube.turnUp(3, face);
		cube.turnFront(3, face);
		cube.turnUp(1, face);
		cube.turnFront(1, face);
	}
	
/////////////////// Cross on Top ////////////////////
	
	public static void step5(Cube cube){
		while (!solved5(cube)){
			cube.turnUp(1);
			int cycles = 0;
			
			while(!solved5(cube) && cycles < 3){
				cube.turnFront(1);
				cube.turnUp(1);
				cube.turnRight(1);
				cube.turnUp(3);
				cube.turnRight(3);
				cube.turnFront(3);
				cycles++;
			}
		}
	}
	
	
	public static boolean solved5(Cube cube){
		
		return cube.up[0][1] == Cube.YELLOW &&
				cube.up[1][0] == Cube.YELLOW &&
				cube.up[2][1] == Cube.YELLOW &&
				cube.up[1][2] == Cube.YELLOW;
	}
	
	
	/////////////////// step 6, top face ///////////////
	
	
	public static void step6(Cube cube){
		
		while (!solved6(cube)){
			if(cube.up[2][0] == Cube.YELLOW && cube.up[2][2] != Cube.YELLOW &&
				cube.up[0][2] != Cube.YELLOW && cube.up[0][0] != Cube.YELLOW){
				doTheFun(cube, Cube.FRONT);
			}
			else if (cube.up[2][2] == Cube.YELLOW && cube.up[2][0] != Cube.YELLOW &&
					cube.up[0][2] != Cube.YELLOW && cube.up[0][0] != Cube.YELLOW){
				doTheFun(cube, Cube.RIGHT);
			}
			else if(cube.up[0][2] == Cube.YELLOW && cube.up[2][0] != Cube.YELLOW &&
					cube.up[2][2] != Cube.YELLOW && cube.up[0][0] != Cube.YELLOW){
				doTheFun(cube, Cube.BACK);
			}
			else if(cube.up[0][0] == Cube.YELLOW && cube.up[2][0] != Cube.YELLOW &&
					cube.up[2][2] != Cube.YELLOW && cube.up[0][2] != Cube.YELLOW){
				doTheFun(cube, Cube.LEFT);
			}
			else{
				int face = 0;
				while(cube.data[face][0][0] != Cube.YELLOW){
					face++;
				}
				doTheFun(cube, face);
			}
		}
	}
	
	// solved when the yellow corner pieces are in place
	public static boolean solved6(Cube cube){
		
		return cube.up[0][0] == Cube.YELLOW &&
				cube.up[0][2] == Cube.YELLOW &&
				cube.up[2][0] == Cube.YELLOW &&
				cube.up[2][2] == Cube.YELLOW;
	}
	
	// do the fun to make the up face complete
	public static void doTheFun(Cube cube, int face){
		
		cube.turnRight(1, face);
		cube.turnUp(1, face);
		cube.turnRight(3, face);
		cube.turnUp(1, face);
		cube.turnRight(1, face);
		cube.turnUp(2, face);
		cube.turnRight(3, face);
		
	}
	
	/////////////////// step 7, the whatever its called ///////////////////
	
	public static void step7(Cube cube){
		while(!solved7(cube)){
			
			int face = 0;
			while(face < Cube.UP && (cube.data[face][0][0] != cube.data[face][0][2])){
				++face;
				System.out.println("in while loop");
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
		while(cube.front[0][0] != Cube.GREEN){
			cube.turnUp(1);
		}
	}
	// checks if corners match each other
	public static boolean solved7(Cube cube){
		System.out.println("checking solved");
		// if both corner's colors don't match the face's color
		for(int faceAndCol = 0; faceAndCol < Cube.UP; faceAndCol++){
			if(!(cube.data[faceAndCol][0][0] == cube.data[faceAndCol][0][2])){
				return false;
			}
		}
		return true;
	}
	
	///////////////// step 8, solve the cube :) ///////////////
	
	public static void step8(Cube cube){
		while(!solved8(cube)){
			
			int face = 0;
			while(face < Cube.UP && (cube.data[face][0][1] != face)){
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
	
	
	// checks to see if the top two are both green.  if they are, returns true
	// otherwise, rotates them
	public static boolean solved8(Cube cube){
		
		for(int turns = 0; turns < 4; turns++){
			
			if(cube.front[0][1] == Cube.GREEN && cube.front[0][0] == Cube.GREEN && 
					cube.right[0][1] == Cube.ORANGE){
				return true;
			}
		}
		return false;
	}
	



////////////////// print cube ///////////////////////

	public void printFace(int[][] plane){
		for(int[] row : plane){
			for(int square : row){
				System.out.print(square);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();
	}
	
	public static void printCube(Cube cube){
		
		for(int row = 0; row < 3; ++row){
			for(int i = 0; i < 8; ++i){
				System.out.print(" ");
			}
			for(int col = 0; col < 3; ++col){
				System.out.print(cube.up[row][col]);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();
		
		for(int row = 0; row < 3; ++row){
			for(int col = 0; col < 3; ++col){
				System.out.print(cube.left[row][col]);
				System.out.print(" ");
			}
			System.out.print("  ");
			for(int col = 0; col < 3; ++col){
				System.out.print(cube.front[row][col]);
				System.out.print(" ");
			}
			System.out.print("  ");
			for(int col = 0; col < 3; ++col){
				System.out.print(cube.right[row][col]);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();
		
		for(int row = 0; row < 3; ++row){
			for(int i = 0; i < 8; ++i){
				System.out.print(" ");
			}
			for(int col = 0; col < 3; ++col){
				System.out.print(cube.down[row][col]);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();
		
		for(int row = 0; row < 3; ++row){
			for(int i = 0; i < 8; ++i){
				System.out.print(" ");
			}
			for(int col = 0; col < 3; ++col){
				System.out.print(cube.back[row][col]);
				System.out.print(" ");
			}
			System.out.println();
		}
		System.out.println();
		System.out.println();
		System.out.println();
		
	}
	
	///////////////// OTHER HELPER METHODS ////////////////////////
	
	public static int opFace(int face){
		if (face == Cube.FRONT){
			return Cube.BACK;
		}
		else if (face == Cube.BACK){
			return Cube.FRONT;
		}
		else if (face == Cube.LEFT){
			return Cube.RIGHT;
		}
		else{
			return Cube.LEFT;
		}
	}
	
	public static int leftFace(int face){
		if (face == Cube.FRONT){
			return Cube.LEFT;
		} else if (face == Cube.BACK){
			return Cube.RIGHT;
		} else if (face == Cube.RIGHT){
			return Cube.FRONT;
		} else {
			return Cube.BACK;
		}
	}
	
	public static int rightFace(int face){
		if (face == Cube.FRONT){
			return Cube.RIGHT;
		} else if (face == Cube.BACK){
			return Cube.LEFT;
		} else if (face == Cube.RIGHT){
			return Cube.BACK;
		} else {
			return Cube.FRONT;
		}
	}
}
