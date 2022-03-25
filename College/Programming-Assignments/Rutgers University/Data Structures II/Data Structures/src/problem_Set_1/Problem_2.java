package problem_Set_1;

/*
Exercise E3.10 of the textbook.
A spreadsheet keeps track of student scores on all the exams in a course.  Each row of the spreadsheet corresponds to one student, and each column in a row corresponds to his/her score on one of the exams.  There are r students and c exams, so the spreadsheet has r rows and c columns.

Consider an algorithm that computes the total score on all exams for each student, and the average class score on each exam.  You need to analyze the running time of this algorithm.

(a) What are the basic operations you would count toward the running time?
(b) What is the worst-case running time as a total count (not big O) of these basic operations?
(c) What is the big O running time?
(d) Is your algorithm linear, quadratic, or some other order?
 */

public class Problem_2 {
	public static void main(String[] args) {
		int[][] examScore = new int[5][5];
		//filling the array
		examScore[0][0] = 100;
		examScore[0][1] = 90;
		examScore[0][2] = 80;
		examScore[0][3] = 70;
		examScore[0][4] = 100;
		examScore[1][0] = 90;
		examScore[1][1] = 85;
		examScore[1][2] = 76;
		examScore[1][3] = 90;
		examScore[1][4] = 100;
		examScore[2][0] = 90;
		examScore[2][1] = 0;
		examScore[2][2] = 60;
		examScore[2][3] = 30;
		examScore[2][4] = 100;
		examScore[3][0] = 54;
		examScore[3][1] = 42;
		examScore[3][2] = 27;
		examScore[3][3] = 0;
		examScore[3][4] = 100;
		examScore[4][0] = 0;
		examScore[4][1] = 0;
		examScore[4][2] = 0;
		examScore[4][3] = 0;
		examScore[4][4] = 100;
		//done filling array
		//testing each row
		System.out.println("Billy's Row: " + avgRow(examScore, 0));
		System.out.println("Jill's Row: " + avgRow(examScore, 1));
		System.out.println("Joe's Row: " + avgRow(examScore, 2));
		System.out.println("Tom's Row: " + avgRow(examScore, 3));
		System.out.println("Baker's Row: " + avgRow(examScore, 4));
		//testing each column
		System.out.println("Exam 1 Avg: " + avgCol(examScore, 0));
		System.out.println("Exam 2 Avg: " + avgCol(examScore, 1));
		System.out.println("Exam 3 Avg: " + avgCol(examScore, 2));
		System.out.println("Exam 4 Avg: " + avgCol(examScore, 3));
		System.out.println("Exam 5 Avg: " + avgCol(examScore, 4));
	}
	
	public static double avgRow(int[][] arr, int row) {
		double sum = 0; 
		for(int i = 0; i < arr.length; i++) {
			sum += arr[row][i];
		}
		return sum/500;
	}
	
	public static double avgCol(int[][] arr, int col) {
		double sum = 0;
		for (int i = 0; i < arr[0].length; i++) {
			sum+= arr[i][col];
		}
		return sum/500;
	}
}
