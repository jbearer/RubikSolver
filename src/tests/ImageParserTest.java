package tests;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import static mainPkg.ImageParser.*;
import mainPkg.Cube;

public class ImageParserTest {
	public static void main(String args[]) {
		System.out.println(testGetRed());
		System.out.println(testGetGreen());
		System.out.println(testGetBlue());
		
		System.out.println(testGetLuminosity());
		System.out.println(testGetGradient());
		
		System.out.println(testGetCorners());
		
		System.out.println(testGetColor());
	}
	
	private static String testGetRed() {
		
		String testRed = "res/red.png";
		String testBlue = "res/blue.png";
		
		File redFile = new File(testRed);
		File blueFile = new File(testBlue);
		
		BufferedImage red, blue;
		
		try {
			red = ImageIO.read(redFile);
			blue = ImageIO.read(blueFile);
		}
		catch(IOException e) {
			return "Unable to run test: testGetRed. Encountered error:" + e.toString();
		}
		
		// Test top left corner
		int redVal = getRed(red, 0, 0);
		if (redVal != 255) {
			return "Failed test: testGetRed.\n\tExpected value: 255\n\tActual value: " + redVal;
		}
		
		// Test middle
		redVal = getRed(red, 32, 32);
		if (redVal != 255) {
			return "Failed test: testGetRed.\n\tExpected value: 255\n\tActual value: " + redVal;
		}
		
		// Test bottom right corner
		redVal = getRed(red, 63, 63);
		if (redVal != 255) {
			return "Failed test: testGetRed.\n\tExpected value: 255\n\tActual value: " + redVal;
		}
		
		// Test top left corner
		int blueVal = getRed(blue, 0, 0);
		if (blueVal != 0) {
			return "Failed test: testGetRed.\n\tExpected value: 0\n\tActual value: " + redVal;
		}
		
		// Test middle
		blueVal = getRed(blue, 32, 32);
		if (blueVal != 0) {
			return "Failed test: testGetRed.\n\tExpected value: 0\n\tActual value: " + redVal;
		}
		
		// Test bottom right corner
		blueVal = getRed(blue, 63, 63);
		if (blueVal != 0) {
			return "Failed test: testGetRed.\n\tExpected value: 0\n\tActual value: " + redVal;
		}
		
		return "Passed test: testGetRed.";
	}
	
	private static String testGetGreen() {
		
		String testGreen = "res/green.png";
		String testBlue = "res/blue.png";
		
		File greenFile = new File(testGreen);
		File blueFile = new File(testBlue);
		
		BufferedImage green, blue;
		
		try {
			green = ImageIO.read(greenFile);
			blue = ImageIO.read(blueFile);
		}
		catch(IOException e) {
			return "Unable to run test: testGetGreen. Encountered error:" + e.toString();
		}
		
		// Test top left corner
		int greenVal = getGreen(green, 0, 0);
		if (greenVal != 255) {
			return "Failed test: testGetGreen.\n\tExpected value: 255\n\tActual value: " + greenVal;
		}
		
		// Test middle
		greenVal = getGreen(green, 32, 32);
		if (greenVal != 255) {
			return "Failed test: testGetGreen.\n\tExpected value: 255\n\tActual value: " + greenVal;
		}
		
		// Test bottom right corner
		greenVal = getGreen(green, 63, 63);
		if (greenVal != 255) {
			return "Failed test: testGetGreen.\n\tExpected value: 255\n\tActual value: " + greenVal;
		}
		
		// Test top left corner
		int blueVal = getGreen(blue, 0, 0);
		if (blueVal != 0) {
			return "Failed test: testGetGreen.\n\tExpected value: 0\n\tActual value: " + greenVal;
		}
		
		// Test middle
		blueVal = getGreen(blue, 32, 32);
		if (blueVal != 0) {
			return "Failed test: testGetGreen.\n\tExpected value: 0\n\tActual value: " + greenVal;
		}
		
		// Test bottom right corner
		blueVal = getGreen(blue, 63, 63);
		if (blueVal != 0) {
			return "Failed test: testGetGreen.\n\tExpected value: 0\n\tActual value: " + greenVal;
		}
		
		return "Passed test: testGetGreen.";
	}

	private static String testGetBlue() {
		
		String testRed = "res/red.png";
		String testBlue = "res/blue.png";
		
		File redFile = new File(testRed);
		File blueFile = new File(testBlue);
		
		BufferedImage red, blue;
		
		try {
			red = ImageIO.read(redFile);
			blue = ImageIO.read(blueFile);
		}
		catch(IOException e) {
			return "Unable to run test: testGetBlue. Encountered error:" + e.toString();
		}
		
		// Test top left corner
		int redVal = getBlue(red, 0, 0);
		if (redVal != 0) {
			return "Failed test: testGetBlue.\n\tExpected value: 0\n\tActual value: " + redVal;
		}
		
		// Test middle
		redVal = getBlue(red, 32, 32);
		if (redVal != 0) {
			return "Failed test: testGetBlue.\n\tExpected value: 0\n\tActual value: " + redVal;
		}
		
		// Test bottom right corner
		redVal = getBlue(red, 63, 63);
		if (redVal != 0) {
			return "Failed test: testGetBlue.\n\tExpected value: 0\n\tActual value: " + redVal;
		}
		
		// Test top left corner
		int blueVal = getBlue(blue, 0, 0);
		if (blueVal != 255) {
			return "Failed test: testGetBlue.\n\tExpected value: 255\n\tActual value: " + redVal;
		}
		
		// Test middle
		blueVal = getBlue(blue, 32, 32);
		if (blueVal != 255) {
			return "Failed test: testGetBlue.\n\tExpected value: 255\n\tActual value: " + redVal;
		}
		
		// Test bottom right corner
		blueVal = getBlue(blue, 63, 63);
		if (blueVal != 255) {
			return "Failed test: testGetBlue.\n\tExpected value: 255\n\tActual value: " + redVal;
		}
		
		return "Passed test: testGetRed.";
	}
	
	private static String testGetLuminosity() {
		String extn = "png";
		String outFilePath = "testOut/";
		
		String simpleTest = "res/simpleEdges.png";
		String curvyTest = "res/curvyEdges.png";
		File simpleFile = new File(simpleTest);
		File curvyFile = new File(curvyTest);
		BufferedImage simple, curvy;
		try {
			simple = ImageIO.read(simpleFile);
			curvy = ImageIO.read(curvyFile);
		}
		catch(IOException e) {
			return "Unable to run test: testGetLuminosity. Encountered error: " + e.toString();
		}
		
		BufferedImage simpleOut = new BufferedImage(64, 64, BufferedImage.TYPE_INT_ARGB);
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				int luminosity = getLuminosity(simple, x, y);
				simpleOut.setRGB(x, y, (luminosity << 24) | (255 << 16) | (255 << 8) | (255));
			}
		}
		
		BufferedImage curvyOut = new BufferedImage(64, 64, BufferedImage.TYPE_INT_ARGB);
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				int luminosity = getLuminosity(curvy, x, y);
				curvyOut.setRGB(x, y, (luminosity << 24) | (255 << 16) | (255 << 8) | (255));
			}
		}
		
		
		try {
			File simpleOutFile = new File(outFilePath + "simpleLuminosityTestOut." + extn);
			ImageIO.write(simpleOut, extn, simpleOutFile);
			
			File curvyOutFile = new File(outFilePath + "curvyLuinosityTestOut." + extn);
			ImageIO.write(curvyOut, extn, curvyOutFile);
		}
		catch (IOException e) {
			return "Unable to run test: testGetLuminosity. Encountered error: " + e.toString();
		}
		
		return "Wrote files:\n\t" + outFilePath + "simpleLuminosityTestOut." + extn + "\n\t" + outFilePath + "curvyLuinosityTestOut." + extn;
	}
	
	private static String testGetGradient() {
		String extn = "png";
		String outFilePath = "testOut/";
		
		String simpleTest = "res/simpleEdges.png";
		String curvyTest = "res/curvyEdges.png";
		File simpleFile = new File(simpleTest);
		File curvyFile = new File(curvyTest);
		BufferedImage simple, curvy;
		try {
			simple = ImageIO.read(simpleFile);
			curvy = ImageIO.read(curvyFile);
		}
		catch(IOException e) {
			return "Unable to run test: testGetGradient. Encountered error: " + e.toString();
		}
		
		double[][] simpleValues = new double[64][64];
		double maxSimpleValue = -1;
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				double gradient = getGradient(simple, x, y);
				if (gradient > maxSimpleValue) maxSimpleValue = gradient;
				simpleValues[x][y] = gradient;
			}
		}
		// Normalize to range 0-255 and write to out image
		BufferedImage simpleOut = new BufferedImage(64, 64, BufferedImage.TYPE_INT_ARGB);
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				simpleValues[x][y] = (int)((double)simpleValues[x][y] * 255.0 / (double)maxSimpleValue);
				simpleOut.setRGB(x, y, ((int)simpleValues[x][y] << 24) | (255 << 16) | (255 << 8) | (255));
			}
		}
		
		double[][] curvyValues = new double[64][64];
		double maxCurvyValue = -1;
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				double gradient = getGradient(curvy, x, y);
				if (gradient > maxCurvyValue) maxCurvyValue = gradient;
				curvyValues[x][y] = gradient;
			}
		}
		
		// Normalize to range 0-255 and write to out image
		BufferedImage curvyOut = new BufferedImage(64, 64, BufferedImage.TYPE_INT_ARGB);
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				curvyValues[x][y] = (int)((double)curvyValues[x][y] * 255.0 / (double)maxCurvyValue);
				curvyOut.setRGB(x, y, ((int)curvyValues[x][y] << 24) | (255 << 16) | (255 << 8) | (255));
			}
		}
		
		
		try {
			File simpleOutFile = new File(outFilePath + "simpleGradientTestOut." + extn);
			ImageIO.write(simpleOut, extn, simpleOutFile);
			
			File curvyOutFile = new File(outFilePath + "curvyGradientTestOut." + extn);
			ImageIO.write(curvyOut, extn, curvyOutFile);
		}
		catch (IOException e) {
			return "Unable to run test: testGetGradient. Encountered error: " + e.toString();
		}
		
		return "Wrote files:\n\t" + outFilePath + "simpleGradientTestOut." + extn + "\n\t" + outFilePath + "curvyGradientTestOut." + extn;
	}
	
	private static String testGetCorners() {
		final int tolerance = 2;
		
		String straightTest = "res/testCubeStraight.png";
		File straightFile = new File(straightTest);
		BufferedImage straight;
		try {
			straight = ImageIO.read(straightFile);
		}
		catch(IOException e) {
			return "Unable to run test: testGetCorners. Encountered error: " + e.toString();
		}
		
		int[][] corners = getCorners(straight);
		if (Math.abs(corners[0][0] - 10) > tolerance || Math.abs(corners[0][1] - 9) > tolerance) {
			return "Failed test: testGetCorners.\n\tExpected (10, 9) for top left corner.\n\tActual value: (" + corners[0][0] + ", " + corners[0][1] + ")";
		}
		if (Math.abs(corners[1][0] - 37) > tolerance || Math.abs(corners[1][1] - 9) > tolerance) {
			return "Failed test: testGetCorners.\n\tExpected (37, 9) for top right corner.\n\tActual value: (" + corners[1][0] + ", " + corners[1][1] + ")";
		}
		if (Math.abs(corners[2][0] - 10) > tolerance|| Math.abs(corners[2][1] - 36) > tolerance) {
			return "Failed test: testGetCorners.\n\tExpected (10, 36) for bottom left corner.\n\tActual value: (" + corners[2][0] + ", " + corners[2][1] + ")";
		}
		if (Math.abs(corners[3][0] - 37) > tolerance || Math.abs(corners[3][1] - 36) > tolerance) {
			return "Failed test: testGetCorners.\n\tExpected (37, 36) for bottom right corner.\n\tActual value: (" + corners[3][0] + ", " + corners[3][1] + ")";
		}
		
		return "Passed test: testGetCorners.";
	}
	
	private static String testGetColor() {
		String straightTest = "res/testCubeSkew.png";
		File straightFile = new File(straightTest);
		BufferedImage straight;
		try {
			straight = ImageIO.read(straightFile);
		}
		catch(IOException e) {
			return "Unable to run test: testGetCorners. Encountered error: " + e.toString();
		}
		
		if (getColor(straight, 0, 0) != Cube.RED) {
			return "Failed test: testGetColor.\n\tExpected red for (0, 0) but got " + getColor(straight, 0, 0);
		}
		if (getColor(straight, 0, 1) != Cube.ORANGE) {
			return "Failed test: testGetColor.\n\tExpected orange for (0, 1) but got " + getColor(straight, 0, 1);
		}
		if (getColor(straight, 0, 2) != Cube.YELLOW) {
			return "Failed test: testGetColor.\n\tExpected yellow for (0, 2) but got " + getColor(straight, 0, 2);
		}
		if (getColor(straight, 1, 0) != Cube.BLUE) {
			return "Failed test: testGetColor.\n\tExpected blue for (1, 0) but got " + getColor(straight, 1, 0);
		}
		if (getColor(straight, 1, 1) != Cube.GREEN) {
			return "Failed test: testGetColor.\n\tExpected green for (1, 1) but got " + getColor(straight, 1, 1);
		}
		/*if (getColor(straight, 1, 2) != Cube.WHITE) {
			return "Failed test: testGetColor.\n\tExpected white for (1, 2) but got " + getColor(straight, 1, 2);
		}*/
		if (getColor(straight, 2, 0) != Cube.BLUE) {
			return "Failed test: testGetColor.\n\tExpected blue for (2, 0) but got " + getColor(straight, 2, 0);
		}
		if (getColor(straight, 2, 1) != Cube.WHITE) {
			return "Failed test: testGetColor.\n\tExpected white for (2, 1) but got " + getColor(straight, 2, 1);
		}
		/*if (getColor(straight, 2, 2) != Cube.GREEN) {
			return "Failed test: testGetColor.\n\tExpected green for (2, 2) but got " + getColor(straight, 2, 2);
		}*/
		
		return "Passed test: testGetColor.";
	}
}
