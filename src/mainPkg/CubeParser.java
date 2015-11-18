package mainPkg;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.ListIterator;

public class CubeParser {
	private ImageParser[] faceParsers;
	
	public CubeParser(String frontFile, String backFile, String leftFile, String rightFile, String upFile, String downFile) {
		faceParsers[0] = new ImageParser(frontFile);
		faceParsers[1] = new ImageParser(backFile);
		faceParsers[2] = new ImageParser(leftFile);
		faceParsers[3] = new ImageParser(rightFile);
		faceParsers[4] = new ImageParser(upFile);
		faceParsers[5] = new ImageParser(downFile);
	}
	
	public Cube getCube() {
		
		FaceColor[][][] faces = new FaceColor[6][3][3];
		for (int i = 0; i < faces.length; i++) {
			faces[i] = faceParsers[i].getFace();
		}
		
		int[][][] groups = assignGroups(faces);
		int[] groupColors = assignColors(groups, faces);
		
		// An array of six faces. Each face is a 3x3 array of ints representing Cube colors
		int[][][] cubeFaces = new int[6][3][3];
		
		for (int group : groupColors) {
			for (int[] tiles : groups[group]) {
				cubeFaces[tiles[0]][tiles[1]][tiles[2]] = group;
			}
		}
		
		return new Cube(cubeFaces[0], cubeFaces[1], cubeFaces[2],
						cubeFaces[3], cubeFaces[4], cubeFaces[5]);
	}
	
	/**
	 * Assign groups of similar tiles to distinct Rubik's cube colors
	 * @param groups An array of six groups of similar tiles. A group is an array of
	 * 	9 ordered triples of the form (face, row, col) indicating the coordinates of
	 * 	a tile belonging to that group
	 * @return A mapping from group number to color, using the color constants defined
	 * 	in Cube. The ith element of the result is the color of the ith group.
	 */
	private int[] assignColors(int[][][] groups, FaceColor[][][] faces) {
		// Matrix representing the cost of assigning a group to a color
		// Rows are indexed by groups, columns are indexed by colors
		double[][] costMatrix = new double[groups.length][faces.length];
		
		for (int group = 0; group < groups.length; group++) {
			for (int color = 0; color < faces.length; color++) {
				// Compute the total cost of assigning all tiles in this group to this color
				double cost = 0;
				for (int[] coords : groups[group]) {
					FaceColor tileColor = faces[coords[0]][coords[1]][coords[2]];
					cost += tileColor.difference(color);
				}
				costMatrix[group][color] = cost;
			}
		}
		
		HungarianAlgorithm assigner = new HungarianAlgorithm(costMatrix);
		
		return assigner.execute();
	}
	
	/**
	 * Assign the computed faces to 6 groups, one for each color
	 * A group should consist of one square and the 8 other squares that are
	 * most similar to that square
	 * 
	 * @param An array of 6 faces, each represented as a 3x3 array of FaceColors
	 * @return An array mapping groups to coordinates (face, row, col)
	 */
	private int[][][] assignGroups(FaceColor[][][] faces) {
		// keeps track of the tiles already assigned to a group
		HashSet<ArrayList<Integer>> usedCoords = new HashSet<ArrayList<Integer>>(); 
		
		int[][][] groups = new int[6][9][3];
		for (int group = 0; group < 6; group++) {
			// Store the coordinates (face, row, col) of the elements currently in this group
			LinkedList<ArrayList<Integer>> curMembers = new LinkedList<ArrayList<Integer>>(); 
			
			// Element to which to compare other potential group members
			ArrayList<Integer> prototype = new ArrayList<Integer>();
			FaceColor protocolor = null;
			
			for (int face = 0; face < faces.length; face++) {
				for (int row = 0; row < faces[face].length; row++) {
					for (int col = 0; col < faces[face][row].length; col++) {
						ArrayList<Integer> coords = new ArrayList<Integer>();
						coords.add(face); coords.add(row); coords.add(col);
						if (usedCoords.contains(coords)) {
							// This tile is already assigned
							continue;
						}
						
						if (curMembers.isEmpty()) {
							// Start a new group with elements similar to this one
							prototype = coords;
							protocolor = faces[coords.get(0)][coords.get(1)][coords.get(2)];
							usedCoords.add(coords);
						}
						else {
							// We need 8 members in addition to the prototype
							
							// Add elements in order of increasing difference from the prototype
							ListIterator<ArrayList<Integer>> i = curMembers.listIterator();
							while(i.hasNext()) {
								ArrayList<Integer> nextCoords = i.next();
								FaceColor nextColor = faces[nextCoords.get(0)][nextCoords.get(1)][nextCoords.get(2)];
								FaceColor curColor = faces[coords.get(0)][coords.get(1)][coords.get(2)];
								if (curColor.difference(protocolor) > nextColor.difference(protocolor)) break;
							}
							i.add(coords);
							if (curMembers.size() > 8) {
								curMembers.removeLast();
							}
							usedCoords.add(coords);
						}
					}
				}
			}
			int curMember = 0;
			ListIterator<ArrayList<Integer>> i = curMembers.listIterator();
			while(i.hasNext()) {
				ArrayList<Integer> coords = i.next();
				for (int coord = 0; coord < coords.size(); coord++) {
					groups[group][curMember][coord] = coords.get(coord);
				}
			}
			for (int j = 0; j < prototype.size(); j++) {
				groups[group][groups[group].length - 1][j] = prototype.get(j);
			}
		}
		
		return groups;
	}
	
}
