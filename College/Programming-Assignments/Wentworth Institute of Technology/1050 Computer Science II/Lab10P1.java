/*********************************************************************
*	LAB 7 - Problem 1
*
*	Kyle Frick
*	COMP1050-09/10  (ENTER YOUR SESSION, EITHER 09/10 or 11/12 for XXXXX)
*	03/17/2015  (UPDATE THE DATE) 
*
**********************************************************************
*	Problem Description (PLEASE UPDATE THE DESCRIPTION)
*
*	Take already made images, and make an interactive display with two buttons
*	that allow the user to refresh the images and sort the images.
***********************************************************************
*	Analysis (PLEASE UPDATE THE DESCRIPTION)
*
*	Inputs:  Event driven Events
*
*	Outputs: 5 cards, either sorted or unsorted
*
*	Details: JavaFX hardcoded objects that shows images and then can sort the images
*	
*	Steps:
*		1. Load the images into a list and display 5 random ones to the output
*		2. have event-driven buttons that when pressed either refreshed the images or sort the images
* 
*   Learning: 
*   	-Event driven Handlers using button pressed or .setOnAction methods for each object.
*   	-Able to understand when events should be used.
*
**********************************************************************/


package edu.wit.cs.comp1050;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.Arrays;

public class Lab10P1 extends Application {

	@Override
	public void start(Stage primaryStage) {
		// TODO: write your code here

		// HBOX adding
		HBox image = new HBox();
		HBox buttons = new HBox();

		// Vbox
		VBox all = new VBox();

		// Buttons
		Button refresh = new Button("Refresh");
		Button sort = new Button("Sort");
		// refresh.setMinHeight(80);
		refresh.setMinSize(100, 40);
		sort.setMinSize(100, 40);
		refresh.setText("Refresh");
		sort.setText("Sort");

		// refresh.setPadding(new Insets(50, 50, 50, 50));
		// sort.setPadding(new Insets(50, 50, 50, 50));

		// Images
		Image img1 = new Image("image/card/1.png");
		Image img10 = new Image("image/card/10.png");
		Image img11 = new Image("image/card/11.png");
		Image img12 = new Image("image/card/12.png");
		Image img13 = new Image("image/card/13.png");
		Image img2 = new Image("image/card/2.png");
		Image img3 = new Image("image/card/3.png");
		Image img4 = new Image("image/card/4.png");
		Image img5 = new Image("image/card/5.png");
		Image img6 = new Image("image/card/6.png");
		Image img7 = new Image("image/card/7.png");
		Image img8 = new Image("image/card/8.png");
		Image img9 = new Image("image/card/8.png");

		// ImageViews
		ImageView imv1 = new ImageView();
		ImageView imv2 = new ImageView();
		ImageView imv3 = new ImageView();
		ImageView imv4 = new ImageView();
		ImageView imv5 = new ImageView();

		// ArrayList<>
		ArrayList<Image> images = new ArrayList<>();
		images.add(img1);
		images.add(img2);
		images.add(img3);
		images.add(img4);
		images.add(img5);
		images.add(img6);
		images.add(img7);
		images.add(img8);
		images.add(img9);
		images.add(img10);
		images.add(img11);
		images.add(img12);
		images.add(img13);

		int[] random = new int[5];
		// Button Pressing
		refresh.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent e) {
				for (int i = 0; i < random.length; i++) {
					int r = (int) (Math.random() * 12) + 1;
					random[i] = r;
				}
				imv1.setImage(images.get(random[0]));
				imv2.setImage(images.get(random[1]));
				imv3.setImage(images.get(random[2]));
				imv4.setImage(images.get(random[3]));
				imv5.setImage(images.get(random[4]));
			}
		});

		sort.setOnAction(new EventHandler<ActionEvent>() {

			@Override
			public void handle(ActionEvent event) {
				// TODO Auto-generated method stub
				// ArrayList<Image> img = new ArrayList<>();
				Arrays.sort(random);
				imv1.setImage(images.get(random[0]));
				imv2.setImage(images.get(random[1]));
				imv3.setImage(images.get(random[2]));
				imv4.setImage(images.get(random[3]));
				imv5.setImage(images.get(random[4]));
			}

		});

		// Adding to boxes

		buttons.getChildren().addAll(refresh, sort);
		image.getChildren().addAll(imv1, imv2, imv3, imv4, imv5);
		all.getChildren().addAll(image, buttons);

		// imv.setImage(img1);
		// h1.getChildren().add(imv);

		for (int i = 0; i < random.length; i++) {
			int r = (int) (Math.random() * 12) + 1;
			random[i] = r;
		}
		imv1.setImage(images.get(random[0]));
		imv2.setImage(images.get(random[1]));
		imv3.setImage(images.get(random[2]));
		imv4.setImage(images.get(random[3]));
		imv5.setImage(images.get(random[4]));

		BorderPane root = new BorderPane();
		root.getChildren().add(all);

		Scene scene = new Scene(root, 400, 150);
		//
		// scene.getStylesheets().add(getClass().getResource("application.css").toExternalForm());
		primaryStage.setScene(scene);
		primaryStage.show();

	}

	/**
	 * The main method is only needed for the IDE with limited JavaFX support.
	 * Not needed for running from the command line.
	 */
	public static void main(String[] args) {
		Application.launch(args);
	}

}
