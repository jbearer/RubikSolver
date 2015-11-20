package tests;

import mainPkg.FaceColor;
import mainPkg.ImageParser;

public class ImageParserTest {
	public static void main(String args[]) {
		testGradients();
		System.out.println(testFaceColorStraight());
		System.out.println(testFaceColorSkewed());
	}
	
	private static double epsilon = 0.0001;
	
	private static void testGradients() {
		ImageParser test = new ImageParser("res/testCubeStraight.png");
		int[][] gradients = test.gradients();
		for (int[] row : gradients) {
			for (int grad : row) {
				System.out.print(grad + " ");
				for (int i = 0; i < 3 - (grad % 10); i++) System.out.print(" ");
			}
			System.out.println();
		}
	}
	
	private static String testFaceColor(String testFile, String testName, FaceColor[][] correct) {
		ImageParser test = new ImageParser(testFile);

		FaceColor[][] observed = test.getFace();

		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				if (correct[row][col].difference(observed[row][col]) > epsilon) {
					return "Failed test: "+ testName + " at (" + row + ", " + col + ")\n\tExpected: "
							+ correct[row][col] + "\n\tActual: " + observed[row][col] + "\n\tError: " + 
							correct[row][col].difference(observed[row][col]);
				}
			}
		}

		return "Passed test: " + testName;
	}
	
	private static String testFaceColorStraight() {
		FaceColor[][] correct = { {new FaceColor(255, 0, 0), new FaceColor(255, 127, 39), new FaceColor(255, 242, 0) },
								  {new FaceColor(0, 0, 255), new FaceColor(34, 177, 76), new FaceColor(236, 234, 235) },
								  {new FaceColor(0, 162, 232), new FaceColor(255, 255, 255), new FaceColor(55, 142, 55) } };
		
		return testFaceColor("res/testCubeStraight.png", "FaceColorStraight", correct);
	}
	
	private static String testFaceColorSkewed() {
		FaceColor[][] correct = { { new FaceColor(255, 0, 0), new FaceColor(255, 127, 39), new FaceColor(255, 242, 0) },
				{ new FaceColor(0, 0, 255), new FaceColor(34, 177, 76), new FaceColor(236, 234, 235) },
				{ new FaceColor(0, 162, 232), new FaceColor(255, 255, 255), new FaceColor(55, 142, 55) } };

		return testFaceColor("res/testCubeSkew.png", "FaceColorSkewed", correct);
	}
}
