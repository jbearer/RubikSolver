// File: ImageParser.java
// Parse a single image of one side of a Rubik's cube
// Convert an image into a 3x3 array of colorsd

package mainPkg;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import javax.imageio.ImageIO;

import mainPkg.Cube;

public class ImageParser {
	
	// Kernels to use in edge finding
	private final int[][] sobelKernelX = { { -1, 0, 1},
									   { -2, 0, 2},
									   { -1, 0, 1}};
	
	private final int[][] sobelKernelY = { { 1, 2, 1},
									   { 0, 0, 0},
									   { -1, -2, -1}};
	
	// Threshold over which a gradient value is considered an edge
	private final double edgeThresh = 100;
	
	class FaceColor {
		FaceColor(int r, int g, int b) {
			red = r;
			green = g;
			blue = b;
		}
		int red;
		int green;
		int blue;
		
		
	}
	
	// Color definitions (RGB)
	private final FaceColor RED = new FaceColor(180, 30, 20);
	private final FaceColor GREEN = new FaceColor(21, 108, 0);
	private final FaceColor BLUE = new FaceColor(37, 115, 160);
	private final FaceColor ORANGE = new FaceColor(200, 40, 15);
	private final FaceColor WHITE = new FaceColor(175, 150, 100);
	private final FaceColor YELLOW = new FaceColor(22, 150, 50);

	// Square root of percentage of area of a square to look at when determining color
	private final double bufferRatio = 0.75;
	
	private BufferedImage image;
	
	public ImageParser() {
		image = null;
	}
	
	public ImageParser(String fileName) {
		File file = new File(fileName);
		BufferedImage fileImage;
		try {
			fileImage = ImageIO.read(file);
			image = fileImage;
		}
		catch (IOException e) {
			System.out.println("Unable to read file: " + fileName + "!");
			image = null;
		}
	}
	
	public int getRed(int x, int y) {
		int value = image.getRGB(x, y);
		return (value >> 16) & 0xff;
	}
	
	public int getGreen(int x, int y) {
		int value = image.getRGB(x, y);
		return (value >> 8) & 0xff;
	}

	public int getBlue(int x, int y) {
		int value = image.getRGB(x, y);
		return value & 0xff;
	}
	
	public int getLuminosity(BufferedImage image, int x, int y) {
		return (int)(0.21 * getRed(x, y) + 0.72 * getGreen(x, y) + 0.07 * getBlue(x, y));
	}
	
	private int convolution(int x, int y, int[][] kernel) {
		int accum = 0;
		
		for (int row = y - 1; row <= y + 1; row ++) {
			
			// Get the row wrt the origin of the kernel
			int kRow = row - y + 1;
			
			// Get the row wrt the pixel
			int pRow = row;
			
			// Make sure we don't go off the edge of the image
			if (pRow < 0) pRow = 0;
			else if (pRow >= image.getHeight()) pRow = image.getHeight() - 1;
			
			for (int col = x - 1; col <= x + 1; col++) {
				// Get the col wrt the origin of the kernel
				int kCol = col - x + 1;
				
				// Get the col wrt the pixel
				int pCol = col;
				
				// Make sure we don't go off the edge of the image
				if (pCol < 0) pCol = 0;
				if (pCol >= image.getWidth()) pCol = image.getWidth() - 1;
				
				accum += getLuminosity(image, pRow, pCol) * kernel[kRow][kCol];
			}
		}
		
		return accum;
	}
	
	// Output is not necessarily between 0 and 255
	public double getGradient(int x, int y) {
		int xGrad = convolution(x, y, sobelKernelX);
		int yGrad = convolution(x, y, sobelKernelY);
		
		return Math.sqrt(xGrad*xGrad + yGrad*yGrad);
	}
	
	// Determine whether the given gradient value indicates the presence of an edge
	private boolean isEdge(double gradient) {
		return gradient > edgeThresh;
	}
	
	// Find the four outside corners of the cube using edge detection
	// Return:
	//	Rows are (x, y) coordinate pairs representing, in order:
	//		top left corner
	//		top right corner
	//		bottom left corner
	//		bottom right corner
	//
	// Returns null if unable to find four corners
	public int[][] getCorners() {
		int[][] corners = new int[4][2];
		
		// Memoize computation of gradients; maps from (x, y) pairs to previously computed gradients
		HashMap<ArrayList<Integer>, Double> computedGradients = new HashMap<ArrayList<Integer>, Double>();
		
		// Find the top left corner
		ArrayList<Integer> coords = new ArrayList<Integer>(2);
		coords.add(0, 0);
		coords.add(1, 0);
		Integer iterations = 0;
		Double gradient = computedGradients.get(coords);
		if (gradient == null) {
			gradient = getGradient(coords.get(0).intValue(), coords.get(1).intValue());
			computedGradients.put(coords, gradient);
		}
		while(!isEdge(gradient)) {
			// search from the line y=x out, moving parallel to the line y = -x
			if (coords.get(0) > coords.get(1)) {
				// we're above the line, move equidistant below it
				Integer x = coords.get(0);
				Integer y = coords.get(0);
				coords.set(0, x);
				coords.set(1, y);
			}
			else {
				// we're either on or below the line, move one step farther away from it and above
				Integer x = coords.get(1)+ 1;
				Integer y = coords.get(0) - 1;
				coords.set(0, x);
				coords.set(1, y);
			}
			
			if (coords.get(0) < 0 || coords.get(0) >= image.getWidth() || coords.get(1) < 0 || coords.get(1) >= image.getHeight()) {
				// we've hit the edge of the picture, move the line away from the origin
				iterations++;
				coords.set(0, iterations);
				coords.set(1, iterations);
				
				if (iterations >= image.getHeight() || iterations >= image.getWidth()) {
					// We've searched the whole image, no luck
					return null;
				}
			}
			gradient = computedGradients.get(coords);
			if (gradient == null) {
				gradient = getGradient(coords.get(0).intValue(), coords.get(1).intValue());
				computedGradients.put(coords, gradient);
			}
		}
		corners[0][0] = coords.get(0); corners[0][1] = coords.get(1);
		
		// Find the top right corner
		iterations = 0;
		coords.set(0, image.getWidth() - 1);
		coords.set(1, 0);
		gradient = null;//computedGradients.get(coords);
		if (gradient == null) {
			gradient = getGradient(coords.get(0), coords.get(1));
			computedGradients.put(coords, gradient);
		}
		while(!isEdge(gradient)) {
			// search from the line y=-x out, moving parallel to the line y = x
			if (image.getWidth() - 1 - coords.get(0) > coords.get(1)) {
				// we're above the line, move equidistant below it
				Integer x = image.getWidth() - 1 - coords.get(1);
				Integer y = image.getWidth() - 1 - coords.get(0);
				coords.set(0, x);
				coords.set(1, y);
			}
			else {
				// we're either on or below the line, move one step farther away from it and above
				Integer x = image.getWidth() - 1 - coords.get(1) - 1;
				Integer y = image.getWidth() - 1 - coords.get(0) - 1;
				coords.set(0, x);
				coords.set(1, y);
			}
			
			if (coords.get(0) < 0 || coords.get(0) >= image.getWidth() || coords.get(1) < 0 || coords.get(1) >= image.getHeight()) {
				// we've hit the edge of the picture, move the line away from the origin
				iterations++;
				coords.set(0, image.getWidth() - 1 - iterations);
				coords.set(1, iterations);
				
				if (iterations >= image.getHeight() || iterations >= image.getWidth()) {
					// We've searched the whole image, no luck
					return null;
				}
			}
			gradient = computedGradients.get(coords);
			if (gradient == null) {
				gradient = getGradient(coords.get(0), coords.get(1));
				computedGradients.put(coords, gradient);
			}
		}
		corners[1][0] = coords.get(0); corners[1][1] = coords.get(1);
		
		// Find the bottom left corner
		iterations = 0;
		coords.set(0, 0);
		coords.set(1, image.getHeight() - 1);
		gradient = computedGradients.get(coords);
		if (gradient == null) {
			gradient = getGradient(coords.get(0), coords.get(1));
			computedGradients.put(coords, gradient);
		}
		while(!isEdge(gradient)) {
			// search from the line y=-x out, moving parallel to the line y = x
			if (image.getWidth() - 1 - coords.get(0) > coords.get(1)) {
				// we're above the line, move equidistant below it
				Integer x = image.getHeight() - 1 - coords.get(1);
				Integer y = image.getHeight() - 1 - coords.get(0);
				coords.set(0, x);
				coords.set(1, y);
			}
			else {
				// we're either on or below the line, move one step farther away from it and above
				Integer x = image.getHeight() - 1 - coords.get(1) - 1;
				Integer y = image.getHeight() - 1 - coords.get(0) - 1;
				coords.set(0, x);
				coords.set(1, y);
			}
			
			if (coords.get(0) < 0 || coords.get(0) >= image.getWidth() || coords.get(1) < 0 || coords.get(1) >= image.getHeight()) {
				// we've hit the edge of the picture, move the line away from the origin
				iterations++;
				coords.set(0, iterations);
				coords.set(1, image.getHeight() - 1 - iterations);
				
				if (iterations >= image.getHeight() || iterations >= image.getWidth()) {
					// We've searched the whole image, no luck
					return null;
				}
			}
			gradient = computedGradients.get(coords);
			if (gradient == null) {
				gradient = getGradient(coords.get(0), coords.get(1));
				computedGradients.put(coords, gradient);
			}
		}
		corners[2][0] = coords.get(0); corners[2][1] = coords.get(1);
		
		// Find the bottom right corner
		iterations = 0;
		coords.set(0, image.getWidth() - 1);
		coords.set(1, image.getHeight() - 1);	
		gradient = computedGradients.get(coords);
		if (gradient == null) {
			gradient = getGradient(coords.get(0), coords.get(1));
			computedGradients.put(coords, gradient);
		}
		while(!isEdge(gradient)) {
			// search from the line y=x out, moving parallel to the line y = -x
			if (coords.get(0) > coords.get(1)) {
				// we're above the line, move equidistant below it
				Integer x = coords.get(1);
				Integer y = coords.get(0);
				coords.set(0, x);
				coords.set(1, y);
			}
			else {
				// we're either on or below the line, move one step farther away from it and above
				Integer x = coords.get(1) + 1;
				Integer y = coords.get(0) - 1;
				coords.set(0, x);
				coords.set(1, y);
			}
			
			if (coords.get(0) < 0 || coords.get(0) >= image.getWidth() || coords.get(1) < 0 || coords.get(1) >= image.getHeight()) {
				// we've hit the edge of the picture, move the line away from the origin
				iterations++;
				coords.set(0, image.getWidth() - 1 - iterations);
				coords.set(1, image.getHeight() - 1 - iterations);
				
				if (iterations >= image.getHeight() || iterations >= image.getWidth()) {
					// We've searched the whole image, no luck
					return null;
				}
			}
			gradient = computedGradients.get(coords);
			if (gradient == null) {
				gradient = getGradient(coords.get(0), coords.get(1));
				computedGradients.put(coords, gradient);
			}
		}
		corners[3][0] = coords.get(0); corners[3][1] = coords.get(1);
		
		return corners;
	}
	
	// For a color with the given RGB components, determine the nearest Rubik's cube color
	private int nearestColor(int red, int green, int blue) {
		int redError = (int) (Math.pow(red - RED.red, 2) + Math.pow(green - RED.green, 2) + Math.pow(blue - RED.blue, 2));
		int greenError = (int) (Math.pow(red - GREEN.red, 2) + Math.pow(green - GREEN.green, 2) + Math.pow(blue - GREEN.blue, 2));
		int blueError = (int) (Math.pow(red - BLUE.red, 2) + Math.pow(green - BLUE.green, 2) + Math.pow(blue - BLUE.blue, 2));
		int orangeError = (int) (Math.pow(red - ORANGE.red, 2) + Math.pow(green - ORANGE.green, 2) + Math.pow(blue - ORANGE.blue, 2));
		int whiteError = (int) (Math.pow(red - WHITE.red, 2) + Math.pow(green - WHITE.green, 2) + Math.pow(blue - WHITE.blue, 2));
		int yellowError = (int) (Math.pow(red - YELLOW.red, 2) + Math.pow(green - YELLOW.green, 2) + Math.pow(blue - YELLOW.blue, 2));
		
		if (redError < greenError && redError < blueError && redError < orangeError && redError < whiteError && redError < yellowError) {
			return Cube.RED;
		}
		else if (greenError < blueError && greenError < orangeError && greenError < whiteError && greenError < yellowError) {
			return Cube.GREEN;
		}
		else if (blueError < orangeError && blueError < whiteError && blueError < yellowError) {
			return Cube.BLUE;
		}
		else if (orangeError < whiteError && orangeError < yellowError) {
			return Cube.ORANGE;
		}
		else if (whiteError < yellowError) {
			return Cube.WHITE;
		}
		else {
			return Cube.YELLOW;
		}
		
	}
	
	private double distance(int x1, int y1, int x2, int y2) {
		return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
	}
	
	// Given an image of a Rubik's cube face, find the square at (row, col)
	// and return the color of that square
	public FaceColor getColor(int[][] corners, int row, int col) {
		double faceWidth = distance(corners[0][0], corners[0][1], corners[1][0], corners[1][1]);
		double faceHeight = distance(corners[0][0], corners[0][1], corners[2][0], corners[2][1]);
		
		// Distance from edge of face to square of interest
		double leftOffset = col*(faceWidth / 3.0) + (faceWidth / 6.0)*(1.0 - bufferRatio);
		double topOffset = row*(faceHeight / 3.0) + (faceHeight / 6.0)*(1.0 - bufferRatio);
		
		// Distance from edge of photo to edge of face
		double xStart = corners[0][0] + (row*(faceHeight / 3.0) * (double)(corners[2][0]-corners[0][0])/distance(corners[0][0], corners[0][1], corners[2][0], corners[2][1]));
		double yStart = corners[0][1] + (col*(faceWidth / 3.0) * (double)(corners[2][0]-corners[0][0])/distance(corners[0][0], corners[0][1], corners[2][0], corners[2][1])); 
		
		double tan = (double)(corners[2][0] - corners[0][0]) / (double)(corners[2][1] - corners[0][1]);
		
		int redAccum = 0;
		int greenAccum = 0;
		int blueAccum = 0;
		int pixelCount = 0;
		
		int xStartRow = (int) (xStart + leftOffset);
		int yStartRow = (int) (yStart + topOffset);
		
		System.out.println("Start");
		// Move across a column of pixels following the slope of the edge
		while (distance((int)(xStart + leftOffset), (int)(yStart + topOffset), xStartRow, yStartRow) < (faceHeight/3.0)*bufferRatio) {
		
			
			int x = xStartRow;
			int y = yStartRow;
			
			// Move across a row of pixels following the slope of the edge
			while (distance((int)xStartRow, (int)yStartRow, x, y) < (faceWidth/3.0)*bufferRatio) {
				redAccum += getRed(x,y);
				greenAccum += getGreen(x,y);
				blueAccum += getBlue(x,y);
				pixelCount ++;
				
				if (Math.abs((x-xStartRow)*tan - (y - yStartRow)) >= 1) {
					y += Math.signum(tan);
				}
				else {
					x += 1;
				}
			}
			
			if (Math.abs((yStartRow - (yStart + topOffset))*tan - (xStartRow - (xStart + leftOffset))) >= 1) {
				xStartRow += Math.signum(tan);
			}
			else {
				yStartRow += 1;
			}
		}
		
		return new FaceColor(redAccum / pixelCount, greenAccum / pixelCount, blueAccum / pixelCount);
	}
	
	// returns a 2D array containing the colors on each square
	public FaceColor[][] getFace() {
		
		int[][] corners = getCorners();
		
		FaceColor[][] face = new FaceColor[3][3];
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				face[row][col] = getColor(corners, row, col);
			}
		}
		return face;
	}
}
