package mainPkg;

public class FaceColor {
	int red;
	int green;
	int blue;
	
	public static final int[] RED = {180, 30, 20};
	public static final int[] GREEN = {21, 108, 0};
	public static final int[] BLUE = {37, 115, 160};
	public static final int[] ORANGE = {200, 40, 15};
	public static final int[] WHITE = {175, 150, 100};
	public static final int[] YELLOW = {22, 150, 50};
	
	public FaceColor(int r, int g, int b) {
		red = r;
		green = g;
		blue = b;
	}
	
	public FaceColor(int[] colors) {
		red = colors[0];
		green = colors[1];
		blue = colors[2];
	}
	
	// Two implementations of nearest color
	
	// determine the nearest Rubik's cube color by minimizing the squared error for RGB components
	// compared to constant cube color definitions
	private int nearestColor_squaredError() {
		int redError = (int) (Math.pow(red - RED[0], 2) + Math.pow(green - RED[1], 2)
				+ Math.pow(blue - RED[2], 2));
		int greenError = (int) (Math.pow(red - GREEN[0], 2) + Math.pow(green - GREEN[1], 2)
				+ Math.pow(blue - GREEN[2], 2));
		int blueError = (int) (Math.pow(red - BLUE[0], 2) + Math.pow(green - BLUE[1], 2)
				+ Math.pow(blue - BLUE[2], 2));
		int orangeError = (int) (Math.pow(red - ORANGE[0], 2) + Math.pow(green - ORANGE[1], 2)
				+ Math.pow(blue - ORANGE[2], 2));
		int whiteError = (int) (Math.pow(red - WHITE[0], 2) + Math.pow(green - WHITE[1], 2)
				+ Math.pow(blue - WHITE[2], 2));
		int yellowError = (int) (Math.pow(red - YELLOW[0], 2) + Math.pow(green - YELLOW[1], 2)
				+ Math.pow(blue - YELLOW[2], 2));

		if (redError < greenError && redError < blueError && redError < orangeError && redError < whiteError
				&& redError < yellowError) {
			return Cube.RED;
		} else if (greenError < blueError && greenError < orangeError && greenError < whiteError
				&& greenError < yellowError) {
			return Cube.GREEN;
		} else if (blueError < orangeError && blueError < whiteError && blueError < yellowError) {
			return Cube.BLUE;
		} else if (orangeError < whiteError && orangeError < yellowError) {
			return Cube.ORANGE;
		} else if (whiteError < yellowError) {
			return Cube.WHITE;
		} else {
			return Cube.YELLOW;
		}
	}
	
	// determine the nearest Rubik's cube color by finding the cube color with the closest ratio of
	// RGB components
	private int nearestColor_ratio() {
		double redError = ratioError(RED);
		double greenError = ratioError(GREEN);
		double blueError = ratioError(BLUE);
		double orangeError = ratioError(ORANGE);
		double whiteError = ratioError(WHITE);
		double yellowError = ratioError(YELLOW);
		
		if (redError < greenError && redError < blueError && redError < orangeError && redError < whiteError
				&& redError < yellowError) {
			return Cube.RED;
		} else if (greenError < blueError && greenError < orangeError && greenError < whiteError
				&& greenError < yellowError) {
			return Cube.GREEN;
		} else if (blueError < orangeError && blueError < whiteError && blueError < yellowError) {
			return Cube.BLUE;
		} else if (orangeError < whiteError && orangeError < yellowError) {
			return Cube.ORANGE;
		} else if (whiteError < yellowError) {
			return Cube.WHITE;
		} else {
			return Cube.YELLOW;
		}
	}
	
	public int nearestColor() {
		// Change this line to use a different implementation of nearest color
		return nearestColor_ratio();
	}
	
	// compute the deviation from the RGB values of a test color
	// finds the total squared error between the component of this color and testColor
	private double squaredError(int[] testColor) {
		double error = 0;
		int[] myColors = {red, green, blue};
		for (int i = 0; i < myColors.length; i++) {
			error += Math.pow(myColors[i] - testColor[i], 2);
		}
		return error;
	}
	
	private double squaredError(FaceColor other) {
		double error = 0;
		int[] myColors = {red, green, blue};
		int[] testColor = {other.red, other.green, other.blue};
		for (int i = 0; i < myColors.length; i++) {
			error += Math.pow(myColors[i] - testColor[i], 2);
		}
		return error;
	}
	
	// compute the deviation from the RGB ratio of a test color
	// first divides each component of this color by the corresponding component
	// of the test color, then computes the squared error of the result from the average
	// of the resulting components, because two colors with the same ratio should give
	// a constant result
	private double ratioError(int[] testColor) {
		double[] compRatios = new double[3];
		int[] myColors = {red, green, blue};
		for (int i = 0; i < myColors.length; i++) {
			// Avoid dividing by zero
			if (testColor[i] == 0) {
				testColor[i] = 1;
				myColors[i] += 1;
			}
			compRatios[i] = (double)myColors[i] / (double)testColor[i];
		}double mean = (compRatios[0] + compRatios[1] + compRatios[2]) / 3.0;
		return Math.pow(compRatios[0] - mean, 2) + Math.pow(compRatios[1], 2) + Math.pow(compRatios[2], 2);
	}
	private double ratioError(FaceColor other) {
		int[] myColors = {red, green, blue};
		int[] otherColors = {other.red, other.green, other.blue};
		
		double[] compRatios = new double[3];
		for (int i = 0; i < myColors.length; i++) {
			// Avoid dividing by zero
			if (otherColors[i] == 0) {
				otherColors[i] = 1;
				myColors[i] += 1;
			}
			compRatios[i] = (double)myColors[i] / (double)otherColors[i];
		}
		double mean = (compRatios[0] + compRatios[1] + compRatios[2]) / 3.0;
		return Math.pow(compRatios[0] - mean, 2) + Math.pow(compRatios[1] - mean, 2) + Math.pow(compRatios[2] - mean, 2);
	}

	/**
	 * Compute the difference between this color and another color
	 * @param other Another FaceColor object
	 * @return The difference between the two colors, implemented as the squared error of the ratios
	 */
	public double difference(FaceColor other) {
		return ratioError(other);
	}
	
	/**
	 * Compute the difference between this color and a color represented as an array of RGB values
	 * @param color An array of length 3, representing the red, green, and blue components of the color to compare
	 * @return The difference between the two colors, implemented as the squared error of the ratios
	 */
	public double difference(int[] color) {
		return ratioError(color);
	}
	
	/**
	 * Compute the difference between this color and a constant cube color
	 * @param color The color to compare
	 * @return The difference between the two colors, implemented as the squared error of the ratios
	 */
	public double difference(int color) {
		switch(color) {
		case Cube.RED:
			return difference(RED);
		case Cube.GREEN:
			return difference(GREEN);
		case Cube.BLUE:
			return difference(BLUE);
		case Cube.ORANGE:
			return difference(ORANGE);
		case Cube.WHITE:
			return difference(WHITE);
		case Cube.YELLOW:
			return difference(YELLOW);
		default:
			throw new IllegalArgumentException("Unrecognized Cube color: " + color);
		}
	}
	
	public String toString() {
		return "(" + red + ", " + green + ", " + blue + ")";
	}
}

