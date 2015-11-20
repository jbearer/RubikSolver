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

import static shared.Shared.*;

public class ImageParser {
	
	///////////////////////////////////////////////////
	// Constants
	///////////////////////////////////////////////////
	
	// Kernels to use in edge finding
	private final int[][] sobelKernelX = { { -1, 0, 1},
									   { -2, 0, 2},
									   { -1, 0, 1}};
	
	private final int[][] sobelKernelY = { { 1, 2, 1},
									   { 0, 0, 0},
									   { -1, -2, -1}};
	
	// Threshold over which a gradient value is considered an edge
	private final double edgeThresh = 50;

	// Square root of percentage of area of a square to look at when determining color
	private final double bufferRatio = 0.75;
	
	///////////////////////////////////////////////////
	// State variables
	///////////////////////////////////////////////////
	private BufferedImage image;
	
	///////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////
	public ImageParser() {
		image = null;
	}
	
	public ImageParser(String fileName) {
		File file = new File(fileName);
		BufferedImage fileImage;
		try {
			fileImage = ImageIO.read(file);
			image = fileImage;
		} catch (IOException e) {
			System.out.println("Unable to read file: " + fileName + "!");
			image = null;
		}
	}
	
	///////////////////////////////////////////////////
	// Public Interface
	///////////////////////////////////////////////////

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
	
	public int[][] gradients() {
		int[][] gradients = new int[image.getHeight()][image.getWidth()];
		for (int x = 0; x < image.getWidth(); x++) {
			for (int y = 0; y < image.getHeight(); y++) {
				int gradient = (int)getGradient(x, y);
				gradients[y][x] = gradient;
			}
		}
		return gradients;
	}
	
	///////////////////////////////////////////////////
	// Private member functions
	///////////////////////////////////////////////////
	
	private int getRed(int x, int y) {
		int value = image.getRGB(x, y);
		return (value >> 16) & 0xff;
	}
	
	private int getGreen(int x, int y) {
		int value = image.getRGB(x, y);
		return (value >> 8) & 0xff;
	}

	private int getBlue(int x, int y) {
		int value = image.getRGB(x, y);
		return value & 0xff;
	}
	
	private int getLuminosity(BufferedImage image, int x, int y) {
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
	private double getGradient(int x, int y) {
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
	private int[][] getCorners() {
		
		// Debugging output showing the paths traced out and the gradients computed in the search for corners
		BufferedImage lines, gradients = null;
		if (DEBUG) {
			lines = new BufferedImage(image.getWidth(), image.getHeight(), BufferedImage.TYPE_INT_RGB);
			gradients = new BufferedImage(image.getWidth(), image.getHeight(), BufferedImage.TYPE_INT_ARGB);
		}
		
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
			gradient = getGradient(coords.get(0), coords.get(1));
			computedGradients.put(coords, gradient);
		}
		while(!isEdge(gradient)) {
			if (DEBUG) {
				lines.setRGB(coords.get(0), coords.get(1), 255 - iterations << 16);
				gradients.setRGB(coords.get(0), coords.get(1), gradient.intValue() << 24);
			}
			
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
				Integer x = coords.get(1)+ 1;
				Integer y = coords.get(0) - 1;
				coords.set(0, x);
				coords.set(1, y);
			}
			
			if (coords.get(0) < 0 || coords.get(0) >= image.getWidth() || coords.get(1) < 0 || coords.get(1) >= image.getHeight()) {
				// we've hit the edge of the picture, move the line away from the origin
				iterations++;
				
				if (iterations % 2 == 0) {
					coords.set(0, iterations / 2);
					coords.set(1, iterations / 2);	
				}
				else {
					// The next row of pixels does not have an exact center
					// Move just above and right of what would be the center
					coords.set(0, iterations / 2 + 1);
					coords.set(1, iterations / 2 );
				}
				
				
				if (iterations*2 >= image.getHeight() || iterations*2 >= image.getWidth()) {
					// We've searched the whole image, no luck	
					if (DEBUG) {
						try {
							File linesOut = new File("testOut/lineTrace.png");
							ImageIO.write(lines, "png", linesOut);
							
							File gradientsOut = new File("testOut/gradients.png");
							ImageIO.write(gradients, "png", gradientsOut);
						} catch (IOException e) {
							return null;
						}
					}
					throw new RuntimeException("Unable to find top left corner");
				}
			}
			gradient = computedGradients.get(coords);
			if (gradient == null) {
				gradient = getGradient(coords.get(0).intValue(), coords.get(1).intValue());
				computedGradients.put(coords, gradient);
			}
		}
		
		// Add 1 to each component so as to not take the pixel on the actual edge
		corners[0][0] = coords.get(0) + 1; corners[0][1] = coords.get(1) + 1;
		
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
			if (DEBUG) lines.setRGB(coords.get(0), coords.get(1), (255 - iterations) << 8);
			
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
				if (iterations % 2 == 0) {
					coords.set(0, image.getWidth() - 1 - iterations/2);
					coords.set(1, iterations/2);
				}
				else {
					// The next row of pixels does not have an exact center
					// Move just above and left of what would be the center
					coords.set(0,  image.getWidth() - 1 - (iterations/2 + 1));
					coords.set(1, iterations/2);	
				}
				
				if (iterations >= image.getHeight() || iterations >= image.getWidth()) {
					// We've searched the whole image, no luck
					if (DEBUG) {
						try {
							File out = new File("testOut/lineTrace.png");
							ImageIO.write(lines, "png", out);
						} catch (IOException e) {
							return null;
						}
					}
					throw new RuntimeException("Unable to find top right corner");
				}
			}
			gradient = computedGradients.get(coords);
			if (gradient == null) {
				gradient = getGradient(coords.get(0), coords.get(1));
				computedGradients.put(coords, gradient);
			}
		}
		corners[1][0] = coords.get(0) - 1; corners[1][1] = coords.get(1) + 1;
		
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
			if(DEBUG) lines.setRGB(coords.get(0), coords.get(1), 255 - iterations);
			
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
				if (iterations % 2 == 0) {
					coords.set(0, iterations/2);
					coords.set(1, image.getHeight() - 1 - iterations/2);
				}
				else {
					// The next row of pixels does not have an exact center
					// Move just above and left of what would be the center
					coords.set(0,  iterations/2);
					coords.set(1, image.getHeight() - 1 - (iterations/2 + 1));
				}
				
				if (iterations >= image.getHeight() || iterations >= image.getWidth()) {
					// We've searched the whole image, no luck
					if (DEBUG) {
						try {
							File out = new File("testOut/lineTrace.png");
							ImageIO.write(lines, "png", out);
						} catch (IOException e) {
							return null;
						}
					}
					throw new RuntimeException("Unable to find bottom left corner");
				}
			}
			gradient = computedGradients.get(coords);
			if (gradient == null) {
				gradient = getGradient(coords.get(0), coords.get(1));
				computedGradients.put(coords, gradient);
			}
		}
		corners[2][0] = coords.get(0) + 1; corners[2][1] = coords.get(1) - 1;
		
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
			if(DEBUG) lines.setRGB(coords.get(0), coords.get(1), ((255 - iterations) << 16) | ((255 - iterations) << 8) | 255 - iterations);
			
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
				if (iterations % 2 == 0) {
					coords.set(0, image.getWidth() - 1 - iterations/2);
					coords.set(1, image.getHeight() - 1 - iterations/2);
				}
				else {
					// The next row of pixels does not have an exact center
					// Move just above and right of what would be the center
					coords.set(0,  image.getWidth() - 1 - iterations/2);
					coords.set(1, image.getHeight() - 1 - (iterations/2 + 1));
				}
				
				if (iterations >= image.getHeight() || iterations >= image.getWidth()) {
					// We've searched the whole image, no luck
					if (DEBUG) {
						try {
							File out = new File("testOut/lineTrace.png");
							ImageIO.write(lines, "png", out);
						} catch (IOException e) {
							return null;
						}
					}
					throw new RuntimeException("Unable to find bottom right corner");
				}
			}
			gradient = computedGradients.get(coords);
			if (gradient == null) {
				gradient = getGradient(coords.get(0), coords.get(1));
				computedGradients.put(coords, gradient);
			}
		}
		corners[3][0] = coords.get(0) - 1; corners[3][1] = coords.get(1) - 1;
		
		if (DEBUG) {
			try {
				File out = new File("testOut/lineTrace.png");
				ImageIO.write(lines, "png", out);
			} catch (IOException e) {
				return null;
			}
		}
		return corners;
	}
	
	private double distance(int x1, int y1, int x2, int y2) {
		return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
	}
	
	// Given an image of a Rubik's cube face, find the square at (row, col)
	// and return the color of that square
	private FaceColor getColor(int[][] corners, int row, int col) {
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
		
		int xStartRow = (int) (xStart + leftOffset + 0.5);
		int yStartRow = (int) (yStart + topOffset + 0.5);
		
		// Move across a column of pixels following the slope of the edge
		while (distance((int)(xStart + leftOffset + 0.5), (int)(yStart + topOffset + 0.5), xStartRow, yStartRow) < (faceHeight/3.0)*bufferRatio) {
		
			
			int x = xStartRow;
			int y = yStartRow;
			log(Integer.toString(row) + ", " + Integer.toString(col) + ":");
			log(y);
			
			// Move across a row of pixels following the slope of the edge
			while (distance((int)(xStartRow + 0.5), (int)(yStartRow + 0.5), x, y) < (faceWidth/3.0)*bufferRatio) {
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
}
