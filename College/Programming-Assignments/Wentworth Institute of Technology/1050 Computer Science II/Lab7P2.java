/*********************************************************************
*	LAB 7 - Problem 2
*
*	Kyle Frick
*	COMP1050-09/10  (ENTER YOUR SESSION, EITHER 09/10 or 11/12 for XXXXX)
*	02/18/2015  (UPDATE THE DATE) 
*
**********************************************************************
*	Problem Description (PLEASE UPDATE THE DESCRIPTION)
*
*	Write a program that uses a bar chart to display the percentages of the overall grade represented
    by project, exams, assignments, and the attendance, as shown in Figure 1. Suppose that project
    takes 35 percent and are displayed in blue, exams take 30 percent and are displayed in green,
    assignments take 30 percent and are displayed in red, and the attendance takes 5 percent and is
    displayed in orange. Please use the JavaFX Rectangle class to display the bars.
*
***********************************************************************
*	Analysis (PLEASE UPDATE THE DESCRIPTION)
*
*	Inputs: No input
*
*	Outputs: A picture that shows bar graphs of our grades worked out in 35%, 30%, 30% and then 5%
*
*	Details: For this problem we are using JavaFX which is new to me.
*   
*		Step 1: Figure out the code for JavaFX
*               Step 2: Add 4 rectangles with 4 labels explaining what they are
*               Step 3: Fill each rectangle with different colors.
*               Step 4: Make a scene and add to Scene
*               Step 5: Make sure the padding for each rectangle and label levels out into the depicted results.
* 
*       Learning:
*           For this project, most of the students did not know the programming for JavaFX. Swing maybe, but FX no.
*           I myself have never used JavaFX before, so in order to complete this assignment, we had to look up further
*           tutorials. I learned about the CSS styling in the programming itself using various Object methods
*           such as .setBorder and .setPadding. Also I learned about the VBox and HBox which are the Vertical and 
*           Horizontal boxes respectively. 
*           With this assignment, it was challenging, yet rewarding.
*
**********************************************************************/

package edu.wit.cs.comp1050;

import javafx.application.Application;
import javafx.stage.Stage;

import javafx.application.Application;
import static javafx.application.Application.launch;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;

/**
 *
 * @author TheMrFrick
 */
public class Lab7P2 extends Application {
    
    @Override
    public void start(Stage primaryStage) {
        Rectangle project = new Rectangle(10,10,100, 120);
        Rectangle exams = new Rectangle(40,110,100, 90);
        Rectangle assign = new Rectangle(100,90);
        Rectangle attendance = new Rectangle(100, 10);
        
        project.setFill(Color.BLUE);
        exams.setFill(Color.GREEN);
        assign.setFill(Color.RED);
        attendance.setFill(Color.ORANGE);
        
        Label projectLabel = new Label();
        projectLabel.setText("Projects -- 35%");
        Label examsLabel = new Label();
        examsLabel.setText("Exams -- 30%");
        Label assignLabel = new Label();
        assignLabel.setText("Assignments -- 30%");
        Label attendanceLabel = new Label();
        attendanceLabel.setText("Attendance -- 5%");
        
        VBox projectBox = new VBox();
        projectBox.getChildren().addAll(projectLabel, project);
        projectBox.setPadding(new Insets(5,5,5,5));
        VBox examsBox = new VBox();
        examsBox.getChildren().addAll(examsLabel,exams);
        examsBox.setPadding(new Insets(35,5,5,5));
        VBox assignBox = new VBox();
        assignBox.getChildren().addAll(assignLabel,assign);
        assignBox.setPadding(new Insets(35,5,5,5));
        VBox homeworkBox = new VBox();
        homeworkBox.getChildren().addAll(attendanceLabel,attendance);
        homeworkBox.setPadding(new Insets(115,5,5,5));
        
        
        HBox root = new HBox();
        root.getChildren().addAll(projectBox, examsBox,assignBox,homeworkBox);
       
       
        
        Scene scene = new Scene(root, 500, 150);
        
        primaryStage.setTitle("Lab7P2");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
}