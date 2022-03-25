/*********************************************************************
*	LAB 7 - Problem 1
*
*	Kyle Frick
*	COMP1050-09/10  (ENTER YOUR SESSION, EITHER 09/10 or 11/12 for XXXXX)
*	02/18/2015  (UPDATE THE DATE) 
*
**********************************************************************
*	Problem Description (PLEASE UPDATE THE DESCRIPTION)
*
*	Write a program that randomly fills in 0-9 into an n * m matrix, prints the matrix, and finds the rows and columns with the largest value.
*       You can use different approaches to solve this problem.
*       For example, you can use two-dimensional arrays to store randomly generated data and ArrayList for saving the indices.
*
***********************************************************************
*	Analysis (PLEASE UPDATE THE DESCRIPTION)
*
*	Inputs:  Two integers, read from the user.
*
*	Outputs: The matrix along with the indices of the largest row and column.
*
*	Details: For this problem we had to figure out our own way to implement making a random matrix
*                and then finding the largest.
*   
*		Step 1: Ask for dimension of the Array
*               Step 2: Create the Array with said sizes
*               Step 3: Randomly put 0-9 in the two-dimension matrix
*               Step 4: Create a storage for keeping track of the vertices.
*               Step 5: Sum up each individual row.
*               Step 6: Add each row value to the storage
*               Step 7: Repeat 5 and 6 for columns.
*               Step 8: Figure out the largest row and column and get the index.
*               Step 9: Print everything out. 
* 
*       Learning: Given 3 methods that consists of the main method, the sumRow and sumColumn adding some constraint.
*                 This constraint didn't allow me to create another way of summing the row and columns. Therefore with the 
*                 parameters of one method including a two-dimensional array, I was forced to use two-dimensional arrays. 
*                 Using Math.Random() * HighestValue + lowestValue instead of Random r = new Random() proved to be more efficient.
*
**********************************************************************/

package edu.wit.cs.comp1050;

import java.util.ArrayList;
import java.util.Scanner;

public class Lab7P1 {
	public static void main(String[] args) {
		// TODO: write your code here
            Scanner input = new Scanner(System.in);
            System.out.print("Enter the number of rows: ");
            int rowSize = input.nextInt();
            System.out.print("Enter the number of columns: ");
            int columnSize = input.nextInt();
            int[][] arr = new int[rowSize][columnSize];           
            ArrayList<Integer> largeRow = new ArrayList<>();
            ArrayList<Integer> largeColumn = new ArrayList<>();
            fillArray(arr, largeRow, largeColumn);
            //System.out.println("Row Array: " + largeRow.toString());
            //System.out.println("Column Array: " + largeColumn.toString());
            int largestRowIndex = findLargestIndex(largeRow);
            int largestColumnIndex = findLargestIndex(largeColumn);
            //System.out.println("");
            System.out.println("");
            printArray(arr);
            System.out.println("");
            System.out.println("The index of the largest row: " + largestRowIndex);
            System.out.println("The index of the largest column: "+ largestColumnIndex);
	}
        
        public static void fillArray(int grid[][] , ArrayList row, ArrayList column){
            for (int i = 0; i < grid.length; i++) {
                for (int j = 0; j < grid[0].length; j++) {
                    grid[i][j] = (int) (Math.random() * 9);
                }
                row.add(sumRow(grid[i]));
            }
            for (int i = 0; i < grid[0].length; i++) {
                column.add(sumColumn(grid, i));
            }
           
        }
        
        public static void printArray(int grid[][]){
            for (int i = 0; i < grid.length; i++) {
                for (int j = 0; j < grid[0].length; j++) {
                    System.out.print(grid[i][j] + " ");
                }
                System.out.println("");
            }
        }

	public static int sumRow(int row[]) {
            int sum = 0;
            for (int i = 0; i < row.length; i++) {
                sum+= row[i];
            }
            return sum;
		// TODO: write your code here
	}

	public static int sumColumn(int matrix[][], int column) {
            int sum = 0;
            //System.out.println("Length: " + matrix[0].length);
            for (int i = 0; i < matrix.length; i++) {
                sum+= matrix[i][column];
            }
            return sum;
		// TODO: write your code here
	}
        
        public static int findLargestIndex(ArrayList<Integer> al){
            int max = 0;
            for (int i = 0; i < al.size(); i++) {
                if (al.get(max) < al.get(i)) {
                    max = i;
                }
            }
            return max;
        }
}
