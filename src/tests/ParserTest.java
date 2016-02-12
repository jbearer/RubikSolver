package tests;

import mainPkg.Cube;
import mainPkg.CubeParser;

public class ParserTest {
	
	public static void main(String[] args) {
		System.out.println(testCube1_fullParse());
	}
	
	/**
	 * With 6 images of the six faces of a real cube, test the full parsing
	 * pipeline. Run ImageParser on these images to get an array of FaceColors,
	 * and then run CubeParser on the result to get a valid cube.
	 */
	public static String testCube1_fullParse() {
		int[][][] correctArray = {
				// Front face
				{ { Cube.ORANGE, Cube.BLUE, Cube.WHITE }, { Cube.WHITE, Cube.YELLOW, Cube.RED },
						{ Cube.BLUE, Cube.GREEN, Cube.WHITE } },

				// Back face
				{ { Cube.YELLOW, Cube.WHITE, Cube.RED }, { Cube.ORANGE, Cube.WHITE, Cube.BLUE },
						{ Cube.RED, Cube.GREEN, Cube.WHITE } },

				// Left face
				{ { Cube.GREEN, Cube.YELLOW, Cube.WHITE }, { Cube.RED, Cube.ORANGE, Cube.BLUE },
						{ Cube.RED, Cube.YELLOW, Cube.YELLOW } },

				// Right face
				{ { Cube.RED, Cube.RED, Cube.ORANGE }, { Cube.YELLOW, Cube.RED, Cube.YELLOW },
						{ Cube.ORANGE, Cube.WHITE, Cube.YELLOW } },

				// Up face
				{ { Cube.BLUE, Cube.ORANGE, Cube.GREEN }, { Cube.GREEN, Cube.GREEN, Cube.BLUE },
						{ Cube.GREEN, Cube.ORANGE, Cube.YELLOW } },

				// Down face
				{ { Cube.BLUE, Cube.ORANGE, Cube.GREEN }, { Cube.RED, Cube.BLUE, Cube.GREEN },
						{ Cube.BLUE, Cube.WHITE, Cube.ORANGE } } };

		Cube correctCube = new Cube(correctArray[0], correctArray[1], correctArray[2], correctArray[3], correctArray[4],
				correctArray[5]);
		if (!correctCube.isValid())
			return "Unable to run test: testCube1_fullParse. Given invalid correct cube:\n" + correctCube;

		CubeParser parser = new CubeParser("res/Cube1/front.png", "res/Cube1/back.png", "res/Cube1/left.png",
				"res/Cube1/right.png", "res/Cube1/up.png", "res/Cube1/down.png");
		Cube test = parser.getCube();
		if (!test.isValid())
			return "Failed test: testCube1_fullParse. Invalid resulting cube:\n" + test;
		else if (!test.equals(correctCube))
			return "Failed test: testCube1_fullParse.\n\tExpected cube:\n" + correctCube + "\n\tActual cube:\n" + test;
		else
			return "Passed test: testCube1_fullParse.";
	}
}

