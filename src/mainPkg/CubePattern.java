package mainPkg;

import java.util.HashMap;

public class CubePattern {

	// ORDER:
	// 			FRONT, RIGHT, BACK, LEFT, UP, DOWN
	
	public static final String ANY = "_";
	public static final String[] ANY_ROW = {ANY, ANY, ANY};
	public static final String[][] ANY_FACE = {ANY_ROW, ANY_ROW, ANY_ROW};
	public static final String[][][] ANY_CUBE = {ANY_FACE, ANY_FACE, ANY_FACE, ANY_FACE, ANY_FACE, ANY_FACE};
	
	String[][][] pattern;
	
	// Constructors
	
	public CubePattern(String[][][] myPattern){
		pattern = myPattern;
	}
	
	// specify the pattern of a face, the rest of the pattern can be anything
	public CubePattern(String[][] facePattern, int face){
		pattern = ANY_CUBE;
		pattern[face] = facePattern;
		}
	
	
	public boolean matches(Cube c){
		
		HashMap<String, Integer> pairs = new HashMap<String, Integer>();
		
		for(int face : Cube.FACES){
			for(int row = 0; row < 3; row++){
				for(int col = 0; col < 3; col++){
					
					int tileColor = c.data[face][row][col];
					String tilePattern = pattern[face][row][col];
					
					if(!matchesTile(tileColor, tilePattern, pairs)){
						return false;
					}
				}
			}
		}
		
		return true;
	}
	
	private boolean matchesTile(int color, String tilePattern, HashMap<String, Integer> pairs){
		if(tilePattern.equals("_")){
			return true;
		}
		
		
		return true;
	}
	
	
}
