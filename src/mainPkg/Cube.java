package mainPkg;

import java.util.ArrayList;

public class Cube {
	
	public static final int YELLOW = 1;
	public static final int GREEN = 2;
	public static final int WHITE = 3;
	public static final int BLUE = 4;
	public static final int RED = 5;
	public static final int ORANGE = 6;
	
	public static int FRONT = 1;
	public static int BACK = 2;
	public static int LEFT = 3;
	public static int RIGHT = 4;
	
	int x;
	
	public ArrayList<String> steps;
	
	public int[][] front;
	public int[][] back;
	public int[][] left;
	public int[][] right;
	public int[][] up;
	public int[][] down;
	
	public Cube(int[][] front, int[][] back, int[][] left, int[][] right, int[][] up, int[][] down) {
		this.front = front;
	}
	
	// dir: > 0 for forward rotation, < 0 for backwards rotation
	public void turnFront(int dir, int face) {
		
	}
	
	public boolean isAdjacentToColor(int face, int row, int col, int color) {
		
		return false;
	}
	
}
