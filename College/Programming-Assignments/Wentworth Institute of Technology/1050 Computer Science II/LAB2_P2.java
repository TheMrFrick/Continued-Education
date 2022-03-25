
import java.util.Scanner;

/**
 * *******************************************************************
 * LAB 2 - Problem 2
 * 
 * Kyle Frick 
 * COMP1050-09/10 (ENTER YOUR SESSION, EITHER 09/10 or 11/12 for XXXXX)
 * 01/15/2016 (UPDATE THE DATE)
 * 
**********************************************************************
 * Problem Description (PLEASE UPDATE THE DESCRIPTION)
 * 
 * Let the user input a length of integers for an array index
 * then the integers input
 * then check to see if there are consecutive fours in a row
 * (four numbers of equal value in a row)
 * 
***********************************************************************
 * Analysis (PLEASE UPDATE THE DESCRIPTION)
 * 
* Inputs: Length of Array, Integers for array
 * 
* Outputs: Consecutive fours validation
 * 
* Details: For this problem, try to determine if there are four consecutive numbers of same value in a row. The
 * steps to solve the problem are:
 * 
 * Step 1: 
 *  Get input from user of the length and the integers.
 * 
 * Step 2:
 *  Send the array into another method to check for consecutive 4s
 * 
 * Step 3:
 *  return a boolean stating whether true or false
 * 
 * Step 4: 
 *  Output whether it has consecutive 4s.
 * 
*********************************************************************
 */
package edu.wit.cs.comp1050;

public class LAB2_P2 {

    public static void main(String[] args) {
        // TODO: write your code here
        Scanner input = new Scanner(System.in);
        System.out.print("Enter the number of values: ");
        int length = input.nextInt();
        int[] intarr = new int[length];
        System.out.println("Enter the number: ");
        for (int i = 0; i < intarr.length; i++) {
            intarr[i] = input.nextInt();
        }
        if (isConsecutiveFour(intarr)) {
            System.out.println("The list has consecutive fours");
        } else {
            System.out.println("The list does not have consecutive fours");
        }
    }

    public static boolean isConsecutiveFour(int[] values) {
        // TODO: write your code here
        int count = 1;
        int length = values.length;
        for (int i = 0; i < values.length; i++) {
            if (length != (i + 1)) {
                if (values[i] == values[i + 1]) {
                    count++;
                    if (count == 4) {
                        break;
                    }
                } else {
                    count = 1;
                }
            }

        }
        if (count >= 4) {
            return true;
        } else {
            return false;
        }
    }
}
