package tests;

import mainPkg.Cube;
import mainPkg.CubeParser;
import mainPkg.FaceColor;

public class CubeParserTest {
	public static void main(String[] args) {
		System.out.println(testEasy_cubeParse());
		System.out.println(testCube1_cubeParse());
	}
	
	/**
	 * Test CubeParser on a particular cube
	 * @param correctArray An array of ints representing the correct cube in Cube.COLOR form
	 * @param faces An array of FaceColors representing the cube to parse
	 */
	private static String testCubeParse(int[][][] correctArray, FaceColor[][][] faces, String testName) {
		Cube correctCube = new Cube(correctArray[0], correctArray[1], correctArray[2], correctArray[3], correctArray[4], correctArray[5]);
		CubeParser parser = new CubeParser();
		Cube test = parser.getCubeWithFaces(faces);
		if (!test.isValid()) return "Failed test: " + testName + ". Invalid resulting cube:\n" + test;
		else if (!test.equals(correctCube)) return "Failed test: " + testName + ".\n\tExpected cube:\n" + correctCube + "\n\tActual cube:\n" + test;
		else return "Passed test: " + testName + ".";
	}
	
	/**
	 * Test CubeParser on a solved cube. This means that the first
	 * 9 colors CubeParser sees should be in a group, the next 9
	 * in another group, and so on (since CubeParser goes face by
	 * face). If this test passes but others fail, it implies that
	 * the order in which CubeParser encounters the faces affects
	 * the groups they are assigned to. This should not be the case
	 * and indicates a problem with the way CubeParser assigns groups.
	 */
	public static String testEasy_cubeParse() {
		FaceColor[][][] faces = { 
				// Front face
				{ { new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.ORANGE) },
				  { new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.ORANGE) },
				  { new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.ORANGE) } },
				
				// Back face
				{ { new FaceColor(FaceColor.RED), new FaceColor(FaceColor.RED), new FaceColor(FaceColor.RED) },
				  { new FaceColor(FaceColor.RED), new FaceColor(FaceColor.RED), new FaceColor(FaceColor.RED) },
				  { new FaceColor(FaceColor.RED), new FaceColor(FaceColor.RED), new FaceColor(FaceColor.RED) } },
				
				// Left face
				{ { new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.WHITE) },
				  { new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.WHITE) },
				  { new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.WHITE) } },
				
				// Right face
				{ { new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.YELLOW) },
				  { new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.YELLOW) },
				  { new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.YELLOW) } },
				
				// Up face
				{ { new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.GREEN) },
				  { new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.GREEN) },
				  { new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.GREEN) } },
				
				// Down face
				{ { new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.BLUE) },
				  { new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.BLUE) },
				  { new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.BLUE) } }	
		};
		
		int[][][] correctArray = { 
				// Front face
				{ { Cube.ORANGE, Cube.ORANGE, Cube.ORANGE },
				  { Cube.ORANGE, Cube.ORANGE, Cube.ORANGE },
				  { Cube.ORANGE, Cube.ORANGE, Cube.ORANGE } },
				
				// Back face
				{ { Cube.RED, Cube.RED, Cube.RED },
				  { Cube.RED, Cube.RED, Cube.RED },
				  { Cube.RED, Cube.RED, Cube.RED } },
				
				// Left face
				{ { Cube.WHITE, Cube.WHITE, Cube.WHITE },
				  { Cube.WHITE, Cube.WHITE, Cube.WHITE },
				  { Cube.WHITE, Cube.WHITE, Cube.WHITE } },
				
				// Right face
				{ { Cube.YELLOW, Cube.YELLOW, Cube.YELLOW },
				  { Cube.YELLOW, Cube.YELLOW, Cube.YELLOW },
				  { Cube.YELLOW, Cube.YELLOW, Cube.YELLOW } },
				
				// Up face
				{ { Cube.GREEN, Cube.GREEN, Cube.GREEN },
				  { Cube.GREEN, Cube.GREEN, Cube.GREEN },
				  { Cube.GREEN, Cube.GREEN, Cube.GREEN } },
				
				// Down face
				{ { Cube.BLUE, Cube.BLUE, Cube.BLUE },
				  { Cube.BLUE, Cube.BLUE, Cube.BLUE },
				  { Cube.BLUE, Cube.BLUE, Cube.BLUE } }	
		};
		
		return testCubeParse(correctArray, faces, "testEasy_cubeParse");
	}
	
	/**
	 * Test CubeParser on a 3D array of FaceColors representing
	 * a valid cube (the cube depicted by the images in Cube1).
	 */
	public static String testCube1_cubeParse() {
		int[][][] correctArray = { 
				// Front face
				{ { Cube.ORANGE, Cube.BLUE, Cube.WHITE },
				  { Cube.WHITE, Cube.YELLOW, Cube.RED },
				  { Cube.BLUE, Cube.GREEN, Cube.WHITE } },
				
				// Back face
				{ { Cube.YELLOW, Cube.WHITE, Cube.RED },
				  { Cube.ORANGE, Cube.WHITE, Cube.BLUE },
				  { Cube.RED, Cube.GREEN, Cube.WHITE } },
				
				// Left face
				{ { Cube.GREEN, Cube.YELLOW, Cube.WHITE },
				  { Cube.RED, Cube.ORANGE, Cube.BLUE },
				  { Cube.RED, Cube.YELLOW, Cube.YELLOW } },
				
				// Right face
				{ { Cube.RED, Cube.RED, Cube.ORANGE },
				  { Cube.YELLOW, Cube.RED, Cube.YELLOW },
				  { Cube.ORANGE, Cube.WHITE, Cube.YELLOW } },
				
				// Up face
				{ { Cube.BLUE, Cube.ORANGE, Cube.GREEN },
				  { Cube.GREEN, Cube.GREEN, Cube.BLUE },
				  { Cube.GREEN, Cube.ORANGE, Cube.YELLOW } },
				
				// Down face
				{ { Cube.BLUE, Cube.ORANGE, Cube.GREEN },
				  { Cube.RED, Cube.BLUE, Cube.GREEN },
				  { Cube.BLUE, Cube.WHITE, Cube.ORANGE } }	
		};
		
		FaceColor[][][] faces = { 
				// Front face
				{ { new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.WHITE) },
				  { new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.RED) },
				  { new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.WHITE) } },
				
				// Back face
				{ { new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.RED) },
				  { new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.BLUE) },
				  { new FaceColor(FaceColor.RED), new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.WHITE) } },
				
				// Left face
				{ { new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.WHITE) },
				  { new FaceColor(FaceColor.RED), new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.BLUE) },
				  { new FaceColor(FaceColor.RED), new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.YELLOW) } },
				
				// Right face
				{ { new FaceColor(FaceColor.RED), new FaceColor(FaceColor.RED), new FaceColor(FaceColor.ORANGE) },
				  { new FaceColor(FaceColor.YELLOW), new FaceColor(FaceColor.RED), new FaceColor(FaceColor.YELLOW) },
				  { new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.YELLOW) } },
				
				// Up face
				{ { new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.GREEN) },
				  { new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.BLUE) },
				  { new FaceColor(FaceColor.GREEN), new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.YELLOW) } },
				
				// Down face
				{ { new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.ORANGE), new FaceColor(FaceColor.GREEN) },
				  { new FaceColor(FaceColor.RED), new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.GREEN) },
				  { new FaceColor(FaceColor.BLUE), new FaceColor(FaceColor.WHITE), new FaceColor(FaceColor.ORANGE) } }	
		};
		
		return testCubeParse(correctArray, faces, "testCube1_cubeParse");
	}	
}
