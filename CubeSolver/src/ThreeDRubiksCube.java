//import javafx.animation.Animation;
//import javafx.animation.KeyFrame;
//import javafx.animation.KeyValue;
//import javafx.animation.Timeline;
//import javafx.application.Application;
//import javafx.scene.Camera;
//import javafx.scene.Group;
//import javafx.scene.Parent;
//import javafx.scene.PerspectiveCamera;
//import javafx.scene.Scene;
//import javafx.scene.paint.Color;
//import javafx.scene.paint.Material;
//import javafx.scene.paint.PhongMaterial;
//import javafx.scene.shape.Box;
//import javafx.scene.shape.Rectangle;
//import javafx.scene.transform.Rotate;
//import javafx.stage.Stage;
//import javafx.util.Duration;
// 
//public class ThreeDRubiksCube extends Application {
//
//	/**
//	 * main()
//	 * run the program
//	 * @param args
//	 */
//	public static void main(String[] args){
//		launch(args);
//	}
//	
//	/**
//	 * start()
//	 * initializes the scene
//	 */
//	@Override
//	public void start(Stage stage){
//		Parent root = createRoot(new HelloCube());
//		
//		/** 
//		root.setRotationAxis(Rotate.X_AXIS);
//		root.setRotate(270);
//		root.rotateProperty();
//		**/
//		
//		stage.setTitle("3D Rubik's Cube");
//		stage.setScene(makeScene(root));
//		stage.show();
//		
//		
//		/**
//		Timeline animation = new Timeline();
//		animation.getKeyFrames().addAll(
//				new KeyFrame(Duration.ZERO,
//						new KeyValue(root.)
//		
//		Timeline animation = new Timeline();
//		animation.getKeyFrames().addAll(
//				 new KeyFrame(Duration.ZERO,
//	                        new KeyValue(cube.ry.angleProperty(), 0d)
//	                ),
//	                new KeyFrame(Duration.valueOf("1000ms"),
//	                        new KeyValue(c.ry.angleProperty(), 360d)
//	                ));
//	               **/
//	}
//	
//	public Scene makeScene(Parent root){
//		// Sets root
//		//Parent root = createCube();
//		
//		Scene s = new Scene(root, 1000, 1000, true); 
//		s.setCamera(new PerspectiveCamera());
//		s.setFill(Color.GRAY);
//		
//		return s;
//
//	}
//	/**
//	 * colorizeCube
//	 * @return a 3D Color array representing the cube
//	 */
//	public Color[][][] colorizeCube(HelloCube cube){
//		
//		// Create colorized cube
//		Color[][][] ColorizedCube = new Color[6][3][3];
//		
//		// Loop through every piece, to fill in colorized cube
//		for (int face = 0; face < 6; face++){
//			for (int row = 0; row < 3; row ++){
//				for (int col = 0; col < 3; col++){
//					if (cube.data[face][row][col]==0){
//						ColorizedCube[face][row][col] = Color.GREEN;
//					}
//					else if (cube.data[face][row][col]==1){
//						ColorizedCube[face][row][col] = Color.BLUE;
//					}
//					else if (cube.data[face][row][col]==2){
//						ColorizedCube[face][row][col] = Color.RED;
//					}
//					else if (cube.data[face][row][col]==3){
//						ColorizedCube[face][row][col] = Color.ORANGE;
//					}
//					else if (cube.data[face][row][col]==4){
//						ColorizedCube[face][row][col] = Color.YELLOW;
//					}
//					else{
//						ColorizedCube[face][row][col] = Color.WHITE;
//					}
//				}
//			}
//		}
//		return ColorizedCube;
//	}
//	
//	/**
//	 * createCube
//	 * @return
//	 */
//	// CREATE THE CUBE
//	public Parent createRoot(HelloCube cube){ // // TODO: take in cube as arguement
//		
//		Color[][][] CC = colorizeCube(cube); 
//		// Front back left right up down
//		
//		
//		// Attempt to set inside black
//		/**
//		// Black inside
//		Box b1 = new Box(298,298,298);
//		b1.setTranslateX(500);
//		b1.setTranslateY(500);
//		b1.setTranslateZ(0);
//		Material phong = new PhongMaterial();
//		b1.setMaterial(phong);
//		**/
//		
//		// Front Face: Row 1
//		Rectangle F00 = new Rectangle(98,98); F00.setTranslateX(500-150); F00.setTranslateY(500-150); F00.setTranslateZ(-150); F00.setFill(CC[0][0][0]);
//		Rectangle F01 = new Rectangle(98,98); F01.setTranslateX(500-50); F01.setTranslateY(500-150); F01.setTranslateZ(-150); F01.setFill(CC[0][0][1]);
//		Rectangle F02 = new Rectangle(98,98); F02.setTranslateX(500+50); F02.setTranslateY(500-150); F02.setTranslateZ(-150); F02.setFill(CC[0][0][2]);
//		// Front Face: Row 2
//		Rectangle F10 = new Rectangle(98,98); F10.setTranslateX(500-150); F10.setTranslateY(500-50); F10.setTranslateZ(-150); F10.setFill(CC[0][1][0]);
//		Rectangle F11 = new Rectangle(98,98); F11.setTranslateX(500-50); F11.setTranslateY(500-50); F11.setTranslateZ(-150); F11.setFill(CC[0][1][1]);
//		Rectangle F12 = new Rectangle(98,98); F12.setTranslateX(500+50); F12.setTranslateY(500-50); F12.setTranslateZ(-150); F12.setFill(CC[0][1][2]);
//		// Front Face: Row 3
//		Rectangle F20 = new Rectangle(98,98); F20.setTranslateX(500-150); F20.setTranslateY(500+50); F20.setTranslateZ(-150); F20.setFill(CC[0][2][0]);
//		Rectangle F21 = new Rectangle(98,98); F21.setTranslateX(500-50); F21.setTranslateY(500+50); F21.setTranslateZ(-150); F21.setFill(CC[0][2][1]);
//		Rectangle F22 = new Rectangle(98,98); F22.setTranslateX(500+50); F22.setTranslateY(500+50); F22.setTranslateZ(-150); F22.setFill(CC[0][2][2]);
//		
//		// Back Face: Row 1
//		Rectangle B00 = new Rectangle(98,98); B00.setTranslateX(500-150); B00.setTranslateY(500-150); B00.setTranslateZ(150); B00.setFill(CC[1][0][0]);
//		Rectangle B01 = new Rectangle(98,98); B01.setTranslateX(500-50); B01.setTranslateY(500-150); B01.setTranslateZ(150); B01.setFill(CC[1][0][1]);
//		Rectangle B02 = new Rectangle(98,98); B02.setTranslateX(500+50); B02.setTranslateY(500-150); B02.setTranslateZ(150); B02.setFill(CC[1][0][2]);
//		// Back Face: Row 2
//		Rectangle B10 = new Rectangle(98,98); B10.setTranslateX(500-150); B10.setTranslateY(500-50); B10.setTranslateZ(150); B10.setFill(CC[1][1][0]);
//		Rectangle B11 = new Rectangle(98,98); B11.setTranslateX(500-50); B11.setTranslateY(500-50); B11.setTranslateZ(150); B11.setFill(CC[1][1][1]);
//		Rectangle B12 = new Rectangle(98,98); B12.setTranslateX(500+50); B12.setTranslateY(500-50); B12.setTranslateZ(150); B12.setFill(CC[1][1][2]);
//		// Back Face: Row 3
//		Rectangle B20 = new Rectangle(98,98); B20.setTranslateX(500-150); B20.setTranslateY(500+50); B20.setTranslateZ(150); B20.setFill(CC[1][2][0]);
//		Rectangle B21 = new Rectangle(98,98); B21.setTranslateX(500-50); B21.setTranslateY(500+50); B21.setTranslateZ(150); B21.setFill(CC[1][2][1]);
//		Rectangle B22 = new Rectangle(98,98); B22.setTranslateX(500+50); B22.setTranslateY(500+50); B22.setTranslateZ(150); B22.setFill(CC[1][2][2]);
//		
//		
//		// Left Face: Row 1
//		Rectangle L00 = new Rectangle(98,98); L00.setTranslateX(500-200); L00.setTranslateY(500-150); L00.setTranslateZ(100); 
//			L00.setRotationAxis(Rotate.Y_AXIS); L00.setRotate(90); L00.rotateProperty(); L00.setFill(CC[2][0][0]);
//		Rectangle L01 = new Rectangle(98,98); L01.setTranslateX(500-200); L01.setTranslateY(500-150); L01.setTranslateZ(0); 
//			L01.setRotationAxis(Rotate.Y_AXIS); L01.setRotate(90); L01.rotateProperty(); L01.setFill(CC[2][0][1]);
//		Rectangle L02 = new Rectangle(98,98); L02.setTranslateX(500-200); L02.setTranslateY(500-150); L02.setTranslateZ(-100); 
//			L02.setRotationAxis(Rotate.Y_AXIS); L02.setRotate(90); L02.rotateProperty(); L02.setFill(CC[2][0][2]);
//		// Left Face: Row 2
//		Rectangle L10 = new Rectangle(98,98); L10.setTranslateX(500-200); L10.setTranslateY(500-50); L10.setTranslateZ(100); 
//			L10.setRotationAxis(Rotate.Y_AXIS); L10.setRotate(90); L10.rotateProperty(); L10.setFill(CC[2][1][0]);
//		Rectangle L11 = new Rectangle(98,98); L11.setTranslateX(500-200); L11.setTranslateY(500-50); L11.setTranslateZ(0); 
//			L11.setRotationAxis(Rotate.Y_AXIS); L11.setRotate(90); L11.rotateProperty(); L11.setFill(CC[2][1][1]);
//		Rectangle L12 = new Rectangle(98,98); L12.setTranslateX(500-200); L12.setTranslateY(500-50); L12.setTranslateZ(-100); 
//			L12.setRotationAxis(Rotate.Y_AXIS); L12.setRotate(90); L12.rotateProperty(); L12.setFill(CC[2][1][2]);
//		// Left Face: Row 3
//		Rectangle L20 = new Rectangle(98,98); L20.setTranslateX(500-200); L20.setTranslateY(500+50); L20.setTranslateZ(100); 
//			L20.setRotationAxis(Rotate.Y_AXIS); L20.setRotate(90); L20.rotateProperty(); L20.setFill(CC[2][2][0]);
//		Rectangle L21 = new Rectangle(98,98); L21.setTranslateX(500-200); L21.setTranslateY(500+50); L21.setTranslateZ(0); 
//			L21.setRotationAxis(Rotate.Y_AXIS); L21.setRotate(90); L21.rotateProperty(); L21.setFill(CC[2][2][1]);
//		Rectangle L22 = new Rectangle(98,98); L22.setTranslateX(500-200); L22.setTranslateY(500+50); L22.setTranslateZ(-100); 
//			L22.setRotationAxis(Rotate.Y_AXIS); L22.setRotate(90); L22.rotateProperty(); L22.setFill(CC[2][2][2]);
//		
//
//		// Right Face: Row 1
//		Rectangle R00 = new Rectangle(98,98); R00.setTranslateX(500+100); R00.setTranslateY(500-150); R00.setTranslateZ(-100); 
//			R00.setRotationAxis(Rotate.Y_AXIS); R00.setRotate(90); R00.rotateProperty(); R00.setFill(CC[3][0][0]);
//		Rectangle R01 = new Rectangle(98,98); R01.setTranslateX(500+100); R01.setTranslateY(500-150); R01.setTranslateZ(0);
//			R01.setRotationAxis(Rotate.Y_AXIS); R01.setRotate(90); R01.rotateProperty(); R01.setFill(CC[3][0][1]);
//		Rectangle R02 = new Rectangle(98,98); R02.setTranslateX(500+100); R02.setTranslateY(500-150); R02.setTranslateZ(100);
//			R02.setRotationAxis(Rotate.Y_AXIS); R02.setRotate(90); R02.rotateProperty(); R02.setFill(CC[3][0][2]);
//		// Right Face: Row 2
//		Rectangle R10 = new Rectangle(98,98); R10.setTranslateX(500+100); R10.setTranslateY(500-50); R10.setTranslateZ(-100);
//			R10.setRotationAxis(Rotate.Y_AXIS); R10.setRotate(90); R10.rotateProperty(); R10.setFill(CC[3][1][0]);
//		Rectangle R11 = new Rectangle(98,98); R11.setTranslateX(500+100); R11.setTranslateY(500-50); R11.setTranslateZ(0);
//			R11.setRotationAxis(Rotate.Y_AXIS); R11.setRotate(90); R11.rotateProperty(); R11.setFill(CC[3][1][1]);
//		Rectangle R12 = new Rectangle(98,98); R12.setTranslateX(500+100); R12.setTranslateY(500-50); R12.setTranslateZ(100);
//			R12.setRotationAxis(Rotate.Y_AXIS); R12.setRotate(90); R12.rotateProperty(); R12.setFill(CC[3][1][2]);
//		// Right Face: Row 3
//		Rectangle R20 = new Rectangle(98,98); R20.setTranslateX(500+100); R20.setTranslateY(500+50); R20.setTranslateZ(-100);
//			R20.setRotationAxis(Rotate.Y_AXIS); R20.setRotate(90); R20.rotateProperty(); R20.setFill(CC[3][2][0]);
//		Rectangle R21 = new Rectangle(98,98); R21.setTranslateX(500+100); R21.setTranslateY(500+50); R21.setTranslateZ(0); 
//			R21.setRotationAxis(Rotate.Y_AXIS); R21.setRotate(90); R21.rotateProperty(); R21.setFill(CC[3][2][1]);
//		Rectangle R22 = new Rectangle(98,98); R22.setTranslateX(500+100); R22.setTranslateY(500+50); R22.setTranslateZ(100); 
//			R22.setRotationAxis(Rotate.Y_AXIS); R22.setRotate(90); R22.rotateProperty(); R22.setFill(CC[3][2][2]);
//			
//		// Up Face: Row 1 (closest to back face)
//		Rectangle U00 = new Rectangle(98,98); U00.setTranslateX(500-150); U00.setTranslateY(500-200); U00.setTranslateZ(100); 
//			U00.setRotationAxis(Rotate.X_AXIS); U00.setRotate(90); U00.rotateProperty(); U00.setFill(CC[4][0][0]);
//		Rectangle U01 = new Rectangle(98,98); U01.setTranslateX(500-50); U01.setTranslateY(500-200); U01.setTranslateZ(100); 
//			U01.setRotationAxis(Rotate.X_AXIS); U01.setRotate(90); U01.rotateProperty(); U01.setFill(CC[4][0][1]);
//		Rectangle U02 = new Rectangle(98,98); U02.setTranslateX(500+50); U02.setTranslateY(500-200); U02.setTranslateZ(100); 
//			U02.setRotationAxis(Rotate.X_AXIS); U02.setRotate(90); U02.rotateProperty(); U02.setFill(CC[4][0][2]);	
//		// Up Face: Row 
//		Rectangle U10 = new Rectangle(98,98); U10.setTranslateX(500-150); U10.setTranslateY(500-200); U10.setTranslateZ(0); 
//			U10.setRotationAxis(Rotate.X_AXIS); U10.setRotate(90); U10.rotateProperty(); U10.setFill(CC[4][1][0]);
//		Rectangle U11 = new Rectangle(98,98); U11.setTranslateX(500-50); U11.setTranslateY(500-200); U11.setTranslateZ(0); 
//			U11.setRotationAxis(Rotate.X_AXIS); U11.setRotate(90); U11.rotateProperty(); U11.setFill(CC[4][1][1]);
//		Rectangle U12 = new Rectangle(98,98); U12.setTranslateX(500+50); U12.setTranslateY(500-200); U12.setTranslateZ(0); 
//			U12.setRotationAxis(Rotate.X_AXIS); U12.setRotate(90); U12.rotateProperty(); U12.setFill(CC[4][1][2]);	
//		// Up Face: Row 3 (closest to Front Face)
//		Rectangle U20 = new Rectangle(98,98); U20.setTranslateX(500-150); U20.setTranslateY(500-200); U20.setTranslateZ(-100); 
//			U20.setRotationAxis(Rotate.X_AXIS); U20.setRotate(90); U20.rotateProperty(); U20.setFill(CC[4][2][0]);
//		Rectangle U21 = new Rectangle(98,98); U21.setTranslateX(500-50); U21.setTranslateY(500-200); U21.setTranslateZ(-100); 
//			U21.setRotationAxis(Rotate.X_AXIS); U21.setRotate(90); U21.rotateProperty(); U21.setFill(CC[4][2][1]);
//		Rectangle U22 = new Rectangle(98,98); U22.setTranslateX(500+50); U22.setTranslateY(500-200); U22.setTranslateZ(-100); 
//			U22.setRotationAxis(Rotate.X_AXIS); U22.setRotate(90); U22.rotateProperty(); U22.setFill(CC[4][2][2]);	
//		
//		// Down Face: Row 1 (closest to front face)
//		Rectangle D00 = new Rectangle(98,98); D00.setTranslateX(500-150); D00.setTranslateY(500+100); D00.setTranslateZ(-100); 
//			D00.setRotationAxis(Rotate.X_AXIS); D00.setRotate(90); D00.rotateProperty(); D00.setFill(CC[5][0][0]);
//		Rectangle D01 = new Rectangle(98,98); D01.setTranslateX(500-50); D01.setTranslateY(500+100); D01.setTranslateZ(-100); 
//			D01.setRotationAxis(Rotate.X_AXIS); D01.setRotate(90); D01.rotateProperty(); D01.setFill(CC[5][0][1]);
//		Rectangle D02 = new Rectangle(98,98); D02.setTranslateX(500+50); D02.setTranslateY(500+100); D02.setTranslateZ(-100); 
//			D02.setRotationAxis(Rotate.X_AXIS); D02.setRotate(90); D02.rotateProperty(); D02.setFill(CC[5][0][2]);	
//		// Down Face: Row 2
//		Rectangle D10 = new Rectangle(98,98); D10.setTranslateX(500-150); D10.setTranslateY(500+100); D10.setTranslateZ(0); 
//			D10.setRotationAxis(Rotate.X_AXIS); D10.setRotate(90); D10.rotateProperty(); D10.setFill(CC[5][1][0]);
//		Rectangle D11 = new Rectangle(98,98); D11.setTranslateX(500-50); D11.setTranslateY(500+100); D11.setTranslateZ(0); 
//			D11.setRotationAxis(Rotate.X_AXIS); D11.setRotate(90); D11.rotateProperty(); D11.setFill(CC[5][1][1]);
//		Rectangle D12 = new Rectangle(98,98); D12.setTranslateX(500+50); D12.setTranslateY(500+100); D12.setTranslateZ(0); 
//			D12.setRotationAxis(Rotate.X_AXIS); D12.setRotate(90); D12.rotateProperty(); D12.setFill(CC[5][1][2]);
//		// Down Face: Row 3 (closest to back face)
//		Rectangle D20 = new Rectangle(98,98); D20.setTranslateX(500-150); D20.setTranslateY(500+100); D20.setTranslateZ(100); 
//			D20.setRotationAxis(Rotate.X_AXIS); D20.setRotate(90); D20.rotateProperty(); D20.setFill(CC[5][2][0]);
//		Rectangle D21 = new Rectangle(98,98); D21.setTranslateX(500-50); D21.setTranslateY(500+100); D21.setTranslateZ(100); 
//			D21.setRotationAxis(Rotate.X_AXIS); D21.setRotate(90); D21.rotateProperty(); D21.setFill(CC[5][2][1]);
//		Rectangle D22 = new Rectangle(98,98); D22.setTranslateX(500+50); D22.setTranslateY(500+100); D22.setTranslateZ(100); 
//			D22.setRotationAxis(Rotate.X_AXIS); D22.setRotate(90); D22.rotateProperty(); D22.setFill(CC[5][2][2]);
//		
//		// Construct the Cube
//		Group root = new Group(  
//				F00,F01,F02,F10,F11,F12,F20,F21,F22,
//				B00,B01,B02,B10,B11,B12,B20,B21,B22,
//				R00,R01,R02,R10,R11,R12,R20,R21,R22,
//				L00,L01,L02,L10,L11,L12,L20,L21,L22,
//				U00,U01,U02,U10,U11,U12,U20,U21,U22,
//				D00,D01,D02,D10,D11,D12,D20,D21,D22);
//		return root;
//		
//	}
//}
