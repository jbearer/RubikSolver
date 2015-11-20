package shared;

public class Shared {
	public static final boolean DEBUG = true;
	
	public static void log(Object data) {
		if (DEBUG) System.out.println(data);
	}
}
