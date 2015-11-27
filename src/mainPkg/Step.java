package mainPkg;

import java.util.ArrayList;


public class Step {

	public int face;
	public int times;
	
	public Step(int myFace, int myTimes){
		face = myFace;
		times = myTimes;
	}
	
	public String toString(){
		
		String first = new String();
		
//		System.out.println("face is " + this.face);
//		System.out.println("times is " + this.times);
		
		switch (this.face){
		
		case Cube.FRONT : first = "F";
			break;
		case Cube.BACK : first = "B";
			break;
		case Cube.LEFT : first = "L";
			break;
		case Cube.RIGHT : first = "R";
			break;
		case Cube.UP : first = "U";
			break;
		case Cube.DOWN : first = "D";
			break;
		
		}
		
		String second = new String();
		
		switch (this.times){
		
		case 1 : second = "";
			break;
		case 2 : second = "2";
			break;
		case 3 : second = "p";
			break;
		}
		
		return first + second;
	}
	
	public static void condense(ArrayList<Step> steps){
		
		// get info for the first step
		Step curr = steps.get(0);
		int currTimes = curr.times;
		int currFace = curr.face;		
		int i = 0;
		
		// loop through the rest of steps.  If the next step has the same
		// face as the current one then combine the steps
		// if you end up doing a step a multiple of 4 times, remove it
		while (i < steps.size()-1){
			
			Step next = steps.get(i+1);
			
			if (next.face == currFace) {
				currTimes = (currTimes + next.times) % 4;
				steps.remove(next);
			}
			
			else {
				
				if (currTimes % 4 == 0){
					steps.remove(curr);
				} else {
					curr.times = currTimes;
					i++;
				}
				
				currFace = next.face;
				currTimes = next.times;
				curr = next;
				
			}
		}
	}
	
	public static void main(String args[]){
		Step a = new Step(Cube.FRONT, 3);
		Step b = new Step(Cube.FRONT, 3);
		Step c = new Step(Cube.BACK, 2);
		Step d = new Step(Cube.FRONT, 1);
		Step e = new Step(Cube.FRONT, 3);
		Step f = new Step(Cube.RIGHT, 1);
		Step g = new Step(Cube.FRONT, 1);
		Step h = new Step(Cube.FRONT, 2);
		Step i = new Step(Cube.FRONT, 2);
		
		Step[] stepArray = {a, b, c, d, e, f, g, h, i};
		
		ArrayList<Step> stepList = new ArrayList<Step>();
		for (Step step : stepArray) {
			stepList.add(step);
		}
		
		condense(stepList);
		System.out.println(stepList);
	}
	
}