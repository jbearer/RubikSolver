import java.util.ArrayList;

public class Cube {
	
	public static final int GREEN = 0;
	public static final int BLUE = 1;
	public static final int RED = 2;
	public static final int ORANGE = 3;
	public static final int YELLOW = 4;
	public static final int WHITE = 5;
	
	public static final int FRONT = 0;
	public static final int BACK = 1;
	public static final int LEFT = 2;
	public static final int RIGHT = 3;
	public static final int UP = 4;
	public static final int DOWN = 5;
	
	
	
	public ArrayList<String> steps;
	
	public int[][] front;
	public int[][] back;
	public int[][] left;
	public int[][] right;
	public int[][] up;
	public int[][] down;
	public int[][][] data;
	
	public Cube(int[][] front, int[][] back, int[][] left, int[][] right, int[][] up, int[][] down) {
		this.front = front;
		this.back = back;
		this.left = left;
		this.right = right;
		this.up = up;
		this.down = down;
		
		int[][][] myData = {this.front, this.back, this.left, this.right, this.up, this.down};
		this.data = myData;
		
		this.steps = new ArrayList<String>();
	}
	
	public static Cube makeTest(){
		
		final int[][] front = {{GREEN, GREEN, GREEN}, {GREEN, GREEN, GREEN}, {GREEN, GREEN, GREEN}};
		final int[][] back = {{BLUE,BLUE,BLUE},{BLUE,BLUE,BLUE},{BLUE,BLUE,BLUE}};
		final int[][] right = {{ORANGE, ORANGE, ORANGE},{ORANGE, ORANGE, ORANGE},{ORANGE, ORANGE, ORANGE}};
		final int[][] left = {{RED, RED, RED}, {RED, RED, RED},{RED, RED, RED}};
		final int[][] up = {{YELLOW, YELLOW, YELLOW},{YELLOW, YELLOW, YELLOW},{YELLOW, YELLOW, YELLOW}};
		final int[][] down = {{WHITE, WHITE, WHITE},{WHITE, WHITE, WHITE},{WHITE, WHITE, WHITE}};
		
		return new Cube(front, back, left, right, up, down);
	}
	
	
	//////////////////// PRIVATE METHODS ///////////////////////
	
	/////////////////// ADJACENT COLORS ///////////////////////
	
	
	// ///////////////// ADJACENT COLORS ///////////////////////

	public boolean isAdjacent(int face, int row, int col, int color) {

		if (face == FRONT) {
			if (row == 0) {
				if (col == 0) {
					return (color == this.up[2][0] || color == this.left[0][2]);
				} else if (col == 1) {
					return (color == this.up[2][1]);
				} else if (col == 2) {
					return (color == this.up[2][2] || color == this.right[0][0]);
				}
			} else if (row == 1) {
				if (col == 0) {
					return (color == this.left[1][2]);
				} else if (col == 2) {
					return (color == this.right[1][0]);
				}
			} else if (row == 2) {
				if (col == 0) {
					return (color == this.down[0][0] || color == this.left[2][2]);
				} else if (col == 1) {
					return (color == this.down[0][1]);
				} else if (col == 2) {
					return (color == this.down[0][2] || color == this.right[2][0]);
				}
			}
		}

		else if (face == BACK) {
			if (row == 0) {
				if (col == 0) {
					return (color == this.up[0][2] || color == this.right[0][2]);
				} else if (col == 1) {
					return (color == this.up[0][1]);
				} else if (col == 2) {
					return (color == this.up[0][0] || color == this.left[0][0]);
				}
			} else if (row == 1) {
				if (col == 0) {
					return (color == this.right[1][2]);
				} else if (col == 2) {
					return (color == this.left[1][0]);
				}
			} else if (row == 2) {
				if (col == 0) {
					return (color == this.down[2][2] || color == this.right[2][2]);
				} else if (col == 1) {
					return (color == this.down[2][1]);
				} else if (col == 2) {
					return (color == this.down[2][0] || color == this.left[2][0]);
				}
			}
		}

		else if (face == LEFT) {
			if (row == 0) {
				if (col == 0) {
					return (color == this.up[0][0] || color == this.back[0][2]);
				} else if (col == 1) {
					return (color == this.up[1][0]);
				} else if (col == 2) {
					return (color == this.up[2][0] || color == this.front[0][0]);
				}
			} else if (row == 1) {
				if (col == 0) {
					return (color == this.back[1][2]);
				} else if (col == 2) {
					return (color == this.front[1][0]);
				}
			} else if (row == 2) {
				if (col == 0) {
					return (color == this.down[2][0] || color == this.back[2][2]);
				} else if (col == 1) {
					return (color == this.down[1][0]);
				} else if (col == 2) {
					return (color == this.down[0][0] || color == this.front[2][0]);
				}
			}
		}

		else if (face == RIGHT) {
			if (row == 0) {
				if (col == 0) {
					return (color == this.up[2][2] || color == this.front[0][2]);
				} else if (col == 1) {
					return (color == this.up[1][2]);
				} else if (col == 2) {
					return (color == this.up[0][2] || color == this.back[0][0]);
				}
			} else if (row == 1) {
				if (col == 0) {
					return (color == this.front[1][2]);
				} else if (col == 2) {
					return (color == this.back[1][0]);
				}
			} else if (row == 2) {
				if (col == 0) {
					return (color == this.down[0][2] || color == this.front[2][2]);
				} else if (col == 1) {
					return (color == this.down[1][2]);
				} else if (col == 2) {
					return (color == this.down[2][2] || color == this.back[2][0]);
				}
			}
		}

		else if (face == UP) {
			if (row == 0) {
				if (col == 0) {
					return (color == this.back[0][2] || color == this.left[0][0]);
				} else if (col == 1) {
					return (color == this.back[0][0]);
				} else if (col == 2) {
					return (color == this.back[0][0] || color == this.right[0][2]);
				}
			} else if (row == 1) {
				if (col == 0) {
					return (color == this.left[0][1]);
				} else if (col == 2) {
					return (color == this.right[0][1]);
				}
			} else if (row == 2) {
				if (col == 0) {
					return (color == this.front[0][0] || color == this.left[0][2]);
				} else if (col == 1) {
					return (color == this.front[0][1]);
				} else if (col == 2) {
					return (color == this.front[0][2] || color == this.right[0][0]);
				}
			}
		}

		else if (face == DOWN) {
			if (row == 0) {
				if (col == 0) {
					return (color == this.front[2][0] || color == this.left[2][2]);
				} else if (col == 1) {
					return (color == this.front[2][1]);
				} else if (col == 2) {
					return (color == this.front[2][2] || color == this.right[2][0]);
				}
			} else if (row == 1) {
				if (col == 0) {
					return (color == this.left[2][1]);
				} else if (col == 2) {
					return (color == this.right[2][1]);
				}
			} else if (row == 2) {
				if (col == 0) {
					return (color == this.back[2][2] || color == this.left[2][0]);
				} else if (col == 1) {
					return (color == this.back[2][1]);
				} else if (col == 2) {
					return (color == this.front[2][0] || color == this.right[2][2]);
				}
			}
		}

		return false;
	}
	
	
	
	/////////////////// TWO ARGUMENT ROTATE METHODS ///////////////////////
	
	public void turnFront(int times, int face){
		
		switch (face){
		case FRONT: turnFront(times);
			break;
		case LEFT: turnLeft(times);
			break;
		case RIGHT: turnRight(times);
			break;
		case BACK: turnBack(times);
			break;
		}	
	}
	
	public void turnBack(int times, int face){
		
		switch (face){
		case FRONT: turnBack(times);
			break;
		case LEFT: turnRight(times);
			break;
		case RIGHT: turnLeft(times);
			break;
		case BACK: turnFront(times);
			break;
		}	
	}
	
	public void turnRight(int times, int face){
		
		switch (face){
		case FRONT: turnRight(times);
			break;
		case LEFT: turnFront(times);
			break;
		case RIGHT: turnBack(times);
			break;
		case BACK: turnLeft(times);
			break;
		}	
	}
	
	public void turnLeft(int times, int face){
		
		switch (face){
		case FRONT: turnLeft(times);
			break;
		case LEFT: turnBack(times);
			break;
		case RIGHT: turnFront(times);
			break;
		case BACK: turnRight(times);
			break;
		}	
	}
	
	public void turnUp(int times, int face){
		turnUp(times);
	}
	
	public void turnDown(int times, int face){
		turnDown(times);
	}
	
	////////////////// ONE ARGUMENT ROTATE METHODS //////////////////////////
	
	public void turnFront(int times){
		
		for(int i = 0; i < times; ++i){
			this.turnFrontHelp();
		}
		switch (times){
		case 1:	this.steps.add("F");
			break;
		case 2: this.steps.add("F2");
			break;
		case 3: this.steps.add("Fp");
			break;
		}	
	}
	
	public void turnBack(int times){
		
		for(int i = 0; i < times; ++i){
			this.turnBackHelp();
		}
		switch (times){
		case 1:	this.steps.add("B");
			break;
		case 2: this.steps.add("B2");
			break;
		case 3: this.steps.add("Bp");
			break;
		}	
	}
		
	public void turnRight(int times){
			
			for(int i = 0; i < times; ++i){
				this.turnRightHelp();
			}
			switch (times){
			case 1:	this.steps.add("R");
				break;
			case 2: this.steps.add("R2");
				break;
			case 3: this.steps.add("Rp");
				break;
			}	
		}
	
	public void turnLeft(int times){
		
		for(int i = 0; i < times; ++i){
			this.turnLeftHelp();
		}
		switch (times){
		case 1:	this.steps.add("L");
			break;
		case 2: this.steps.add("L2");
			break;
		case 3: this.steps.add("Lp");
			break;
		}	
	}
	
	public void turnUp(int times){
		
		for(int i = 0; i < times; ++i){
			this.turnUpHelp();
		}
		switch (times){
		case 1:	this.steps.add("U");
			break;
		case 2: this.steps.add("U2");
			break;
		case 3: this.steps.add("Up");
			break;
		}	
	}
	
	public void turnDown(int times){
		
		for(int i = 0; i < times; ++i){
			this.turnDownHelp();
		}
		switch (times){
		case 1:	this.steps.add("D");
			break;
		case 2: this.steps.add("D2");
			break;
		case 3: this.steps.add("Dp");
			break;
		}	
	}
	
	
	
	
	/////////////////// ROTATE HELPER METHODS /////////////////////////
	public static void rotateClockwise(int[][] plane) {
		
		int temp = plane[0][0];
		
		plane[0][0] = plane[2][0];
		plane[2][0] = plane[2][2];
		plane[2][2] = plane[0][2];
		plane[0][2] = temp;
		
		temp = plane[0][1];
		
		plane[0][1] = plane[1][0];
		plane[1][0] = plane[2][1];
		plane[2][1] = plane[1][2];
		plane [1][2] = temp;
		
	}
	
	public static void rotateTwice(int[][] plane){
		rotateClockwise(plane);
		rotateClockwise(plane);
	}
	
	public static void rotateCounterClockwise(int[][] plane) {
		
		rotateClockwise(plane);
		rotateClockwise(plane);
		rotateClockwise(plane);
	}
	
	public void turnFrontHelp(){
		
		rotateClockwise(this.front);
		
		int[] temp = {this.up[2][0], this.up[2][1], this.up[2][2]};
		
		//fix up face
		this.up[2][0] = this.left[2][2];
		this.up[2][1] = this.left[1][2];
		this.up[2][2] = this.left[0][2];
		
		//fix left face
		this.left[0][2] = this.down[0][0];
		this.left[1][2] = this.down[0][1];
		this.left[2][2] = this.down[0][2];
		
		//fix down face
		this.down[0][0] = this.right[2][0];
		this.down[0][1] = this.right[1][0];
		this.down[0][2] = this.right[0][0];
		
		//fix right face
		this.right[0][0] = temp[0];
		this.right[1][0] = temp[1];
		this.right[2][0] = temp[2];

	}
	
	public void turnRightHelp(){
		
		//FIXED
		
		rotateClockwise(this.right);
		
		int[] temp = {this.front[0][2], this.front[1][2], this.front[2][2]};
		
		colToCol(this.down, this.front, 2);
		
		//fix down
		this.down[2][2] = this.back[0][0];
		this.down[1][2] = this.back[1][0];
		this.down[0][2] = this.back[2][0];
		
		//fix back
		this.back[0][0] = this.up[2][2];
		this.back[1][0] = this.up[1][2];
		this.back[2][0] = this.up[0][2];
		
		//fix up
		
		this.up[0][2] = temp[0];
		this.up[1][2] = temp[1];
		this.up[2][2] = temp[2];
	}
	
	public void turnLeftHelp(){
		//FIXED
		rotateClockwise(this.left);
		
		int[] temp = {this.front[0][0], this.front[1][0], this.front[2][0]};
		
		colToCol(this.up, this.front, 0);
		
		this.up[0][0] = this.back[2][2];
		this.up[1][0] = this.back[1][2];
		this.up[2][0] = this.back[0][2];
		
		this.back[2][2] = this.down[0][0];
		this.back[1][2] = this.down[1][0];
		this.back[0][2] = this.down[2][0];
		
		this.down[0][0] = temp[0];
		this.down[1][0] = temp[1];
		this.down[2][0] = temp[2];
		
	}
	
	public void turnUpHelp(){
		rotateClockwise(this.up);
		
		int[] temp = {this.front[0][0], this.front[0][1], this.front[0][2]};
		
		rowToRow(this.right, this.front, 0);
		rowToRow(this.back, this.right, 0);
		rowToRow(this.left, this.back, 0);
		
		this.left[0][0] = temp[0];
		this.left[0][1] = temp[1];
		this.left[0][2] = temp[2];
	}
	
	public void turnDownHelp(){
		rotateClockwise(this.down);
		
		int[] temp = {this.front[2][0], this.front[2][1], this.front[2][2]};
		
		rowToRow(this.left, this.front, 2);
		rowToRow(this.back, this.left, 2);
		rowToRow(this.right, this.back, 2);
		
		//fix right face
		this.right[2][0] = temp[0];
		this.right[2][1] = temp[1];
		this.right[2][2] = temp[2];
	}
	
	public void turnBackHelp(){
		rotateClockwise(this.back);
		
		int[] temp = {this.up[0][0], this.up[0][1], this.up[0][2]};
		
		colToRow(this.right, this.up, 2, 0);
		
		this.right[0][2] = this.down[2][2];
		this.right[1][2] = this.down[2][1];
		this.right[2][2] = this.down[2][0];
		
		this.down[2][0] = this.left[0][0];
		this.down[2][1] = this.left[1][0];
		this.down[2][2] = this.left[2][0];
		
		this.left[2][0] = temp[0];
		this.left[1][0] = temp[1];
		this.left[0][0] = temp[2];
	}
	
	
	public void colToCol(int[][] start, int[][] end, int col){
		end[0][col] = start[0][col];
		end[1][col] = start[1][col];
		end[2][col] = start[2][col];
	}
	
	public void rowToRow(int[][] start, int[][] end, int row){
		end[row][0] = start[row][0];
		end[row][1] = start[row][1];
		end[row][2] = start[row][2];
	}
	
	public void rowToCol(int[][] start, int[][] end, int startRow, int endCol){
		end[0][endCol] = start[startRow][0];
		end[1][endCol] = start[startRow][1];
		end[2][endCol] = start[startRow][2];
	}
	
	public void colToRow(int[][] start, int[][] end, int startCol, int endRow){
		end[endRow][0] = start[0][startCol];
		end[endRow][1] = start[1][startCol];
		end[endRow][2] = start[2][startCol];
	}
	
	//////////////// TEST CUBES ///////////////////////
	
	public static Cube makeStep8Test(){
		int[][] front = {{GREEN, ORANGE, GREEN},
				{GREEN, GREEN, GREEN},
				{GREEN, GREEN, GREEN}};
		int[][] back = {{BLUE, BLUE, BLUE},
				{BLUE, BLUE, BLUE},
				{BLUE, BLUE, BLUE}};
		int[][] left = {{RED, GREEN, RED},
				{RED, RED, RED},
				{RED, RED, RED}};
		int[][] right = {{ORANGE, RED, ORANGE},
				{ORANGE, ORANGE, ORANGE},
				{ORANGE, ORANGE, ORANGE}};
		int[][] up = {{YELLOW, YELLOW, YELLOW},
				{YELLOW, YELLOW, YELLOW},
				{YELLOW, YELLOW, YELLOW}};
		int [][] down = {{WHITE, WHITE, WHITE},
				{WHITE, WHITE, WHITE},
				{WHITE, WHITE, WHITE}};
		
		Cube cube = new Cube(front, back, left, right, up, down);
		return cube;
	}
	
	public static Cube makeStep7Test(){
		int[][] front = {{GREEN, GREEN, ORANGE},
				{GREEN, GREEN, GREEN},
				{GREEN, GREEN, GREEN}};
		int[][] back = {{RED, BLUE, GREEN},
				{BLUE, BLUE, BLUE},
				{BLUE, BLUE, BLUE}};
		int[][] left = {{ORANGE, RED, RED},
				{RED, RED, RED},
				{RED, RED, RED}};
		int[][] right = {{BLUE, ORANGE, BLUE},
				{ORANGE, ORANGE, ORANGE},
				{ORANGE, ORANGE, ORANGE}};
		int[][] up = {{YELLOW, YELLOW, YELLOW},
				{YELLOW, YELLOW, YELLOW},
				{YELLOW, YELLOW, YELLOW}};
		int [][] down = {{WHITE, WHITE, WHITE},
				{WHITE, WHITE, WHITE},
				{WHITE, WHITE, WHITE}};
		
		Cube cube = new Cube(front, back, left, right, up, down);
		return cube;
	}
	
	public int[] corrTopSpot (int row, int col, int face) {
		
		if (face == Cube.FRONT){
			int[] result = {row, col};
			return result;
		}
		else if (face == Cube.BACK){
			int[] result = {2-row, 2-col};
			return result;
		}
		else if(face == Cube.LEFT){
			if (row == 1){
				int[]result =  {col, row};
				return result;
			}
			else{
				int[] result = {2-col, 2-row};
				return result;
			}
		}
		else if (face == Cube.RIGHT){
			int[] result= {2-col, row};
			return result;
		}
		else if (face == Cube.DOWN){
			if (row == 1){
				int[] result= {row, col};
				return result;
			}
			else{
				int[] result = {2-row, 2-col};
				return result;
			}
		}
		else{
			int[] result = {-1,-1};
			return result;
		}
	}
	
	// returns the true row and column on the up face,
	// given the row and col from the perspective of another face
	public int upFrom(int row, int col, int face){
		if(face == FRONT){
			return this.up[row][col];
		} else if (face == RIGHT){
			return this.up[2-col][row];
		} else if (face == BACK){
			return this.up[2-row][2-col];
		} else {
			return this.up[col][2-row];
		}
	}
}
