import static org.junit.Assert.*;

import java.util.Arrays;

import org.junit.Test;


public class CubeTest {

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
	
	public void printCube(Cube cube){
		
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
	
	//@Test
	public void rotateTest() {
		int[][] x = {{0,1,2},
					{7,8,3},
					{6,5,4}};
		
		printFace(x);
		Cube.rotateClockwise(x);
		printFace(x);
		Cube.rotateCounterClockwise(x);
		printFace(x);
		Cube.rotateTwice(x);
		printFace(x);
		
		
	}
	//@Test
	public void printCubeTest() {
		Cube cube = Cube.makeTest();
		printCube(cube);
	}
	//@Test
	public void turnFrontTest() {
		Cube cube = Cube.makeTest();
		cube.turnFrontHelp();
		printCube(cube);
	}
	
	//@Test
	public void turnRightTest() {
		Cube cube = Cube.makeTest();
		cube.turnRightHelp();
		printCube(cube);
	}
	
	//@Test
	public void turnLeftTest() {
		Cube cube = Cube.makeTest();
		cube.turnLeftHelp();
		printCube(cube);
	}
	
	//@Test
	public void turnUpTest() {
		Cube cube = Cube.makeTest();
		cube.turnUpHelp();
		printCube(cube);
	}
	
	//@Test
	public void turnDownTest() {
		Cube cube = Cube.makeTest();
		cube.turnDownHelp();
		printCube(cube);
	}
	
	//@Test
	public void turnBackTest() {
		Cube cube = Cube.makeTest();
		cube.turnBackHelp();
		printCube(cube);
	}
	
	@Test
	public void turnTest() {
		Cube cube = Cube.makeTest();
		printCube(cube);
		cube.turnFrontHelp();
		printCube(cube);
		cube.turnRightHelp();
		printCube(cube);
		cube.turnLeftHelp();
		printCube(cube);
		cube.turnUpHelp();
		printCube(cube);
		cube.turnDownHelp();
		printCube(cube);
		cube.turnBackHelp();
		printCube(cube);
	}
	
	//@Test
	public void turnFront2Test() {
		Cube cube = Cube.makeTest();
		cube.turnFront(2);
		printCube(cube);
		System.out.println(cube.steps);
		
	}
	
	
}
