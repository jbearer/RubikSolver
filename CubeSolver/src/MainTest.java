import static org.junit.Assert.*;

import org.junit.Test;


public class MainTest {
	
	
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
	public void test8() {
		Cube cube = Cube.makeStep8Test();
		printCube(cube);
		
		Main.step8(cube);
		printCube(cube);
	}
	
	//@Test
	public void test7() {
		Cube cube = Cube.makeStep7Test();
		printCube(cube);
		
		Main.step7(cube);
		printCube(cube);
	}

	@Test
	public void test78() {
		Cube cube = Cube.makeStep7Test();
		printCube(cube);
		
		Main.step7(cube);
		
		printCube(cube);
		Main.step8(cube);
		
		printCube(cube);
		
		Cube solved = Cube.makeTest();
		
		printCube(solved);
		assertArrayEquals(solved.data,cube.data);
	}
}
