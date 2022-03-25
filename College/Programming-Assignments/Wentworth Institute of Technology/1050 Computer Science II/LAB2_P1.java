/**
 * *******************************************************************
 * LAB 2 - Problem 1
 * 
* KYLE FRICK COMP1050-09/10 (ENTER YOUR SESSION, EITHER 09/10 or 11/12 for
 * XXXXX) 01/14/2015 (UPDATE THE DATE)
 * 
**********************************************************************
 * Problem Description (PLEASE UPDATE THE DESCRIPTION)
 * 
 * Write a program in which gets an input from the user with a digit from 13-16
 * Then apply the Mod 10 check to see if the digits from 13-16 would be considered
 * a correct credit/debit card
 * If it doesn't work reply with invalid, if it does reply with valid
 * 
 * 
***********************************************************************
 * Analysis (PLEASE UPDATE THE DESCRIPTION)
 * 
* Inputs: A long number with up to 16 digits
 * 
* Outputs: Valid or invalid
 * 
* Details: For this problem, apply the mod 10 check
* The steps to solve the problem are:
* Step 1:
*   Add the double of the second to last digit increment by 2
*       If the double of the digit results in a two digit result then add the individual digits and add that to the sum
* Step 2:
*   Add the odd last digit increment by 2
* 
* Step 3:
*   Add those two sums together
* 
* Step 4:
*   If the sum of those individuals sums are divisible by 10 with no remainder, then it is valid
*   considering the individual card type prefixes.
*   
 * 
*********************************************************************
 */
 
package edu.wit.cs.comp1050;
import java.util.Scanner;

public class LAB2_P1 {

    public static void main(String[] args) {
        // TODO: write your code here
        System.out.print("Enter a credit card number as a long number: ");
        Scanner input = new Scanner(System.in);
        long cardNumber = input.nextLong();
        if (isValid(cardNumber)) {
            System.out.println(cardNumber + " is Valid");
        } else {
            System.out.println(cardNumber + " is inValid");
        }
    }

    /**
     * Return true if the card number is valid
     */
    public static boolean isValid(long number) {
        // TODO: write your code here
        int sumeven = sumOfDoubleEvenPlace(number);
        int sumodd = sumOfOddPlace(number);
        int sum = sumeven + sumodd;
        boolean isCard = false;
        if (prefixMatched(number, 4)) {
            isCard = true;
        } else if (prefixMatched(number, 5)) {
            isCard = true;
        } else if (prefixMatched(number, 6)) {
            isCard = true;
        } else if (prefixMatched(number, 37)) {
            isCard = true;
        }
        if (sum % 10 == 0 && isCard) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * Get the result from Step 2
     */
    public static int sumOfDoubleEvenPlace(long number) {
        // TODO: write your code here
        int sum = 0;
        double[] doubleTemp = new double[getSize(number)];
        int[] digits = new int[getSize(number)];
        int digitsum = 0;
        for (int i = 0; i < doubleTemp.length; i++) {
            doubleTemp[i] = number % 10;
            number /= 10;
        }
        for (int i = 0; i < digits.length; i++) {
            digits[i] = (int) doubleTemp[i];
        }
        for (int i = 1; i < digits.length; i += 2) {
            digitsum = getDigit(digits[i] * 2);
            sum += digitsum;
        }
        return sum;
    }

    /**
     * Return this number if it is a single digit, otherwise, return the sum of
     * the two digits
     */
    public static int getDigit(int number) {
        // TODO: write your code here
        if (number >= 10) {
            int num1 = number % 10;
            int num2 = (int) number / 10;
            return num1 + num2;
        } else {
            return number;
        }
    }

    /**
     * Return sum of odd place digits in number
     */
    public static int sumOfOddPlace(long number) {
        // TODO: write your code here
        double[] doubleTemp = new double[getSize(number)];
        int[] digits = new int[getSize(number)];
        int sum = 0;
        for (int i = 0; i < doubleTemp.length; i++) {
            doubleTemp[i] = number % 10;
            number /= 10;
        }
        for (int i = 0; i < digits.length; i++) {
            digits[i] = (int) doubleTemp[i];
        }
        for (int i = 0; i < digits.length; i += 2) {
            sum += digits[i];
        }
        return sum;
    }

    /**
     * Return true if the digit d is a prefix for number
     */
    public static boolean prefixMatched(long number, int d) {
        // TODO: write your code here
        if (getPrefix(number, 1) != 3) {
            long prefix = getPrefix(number, 1);
            if (prefix == d) {
                return true;
            } else {
                return false;
            }
        } else {
            long prefix = getPrefix(number, 2);
            if (prefix == d) {
                return true;
            } else {
                return false;
            }
        }
    }

    /**
     * Return the number of digits in d
     */
    public static int getSize(long d) {
        // TODO: write your code here
        int length = String.valueOf(d).length();
        return length;
    }

    /**
     * Return the first k number of digits from number. If the number of digits
     * in number is less than k, return number.
     */
    public static long getPrefix(long number, int k) {
        // TODO: write your code here
        long length = String.valueOf(number).length();
        if (k < length) {
            return (long) ((long) number / (Math.pow(10, ((length - k)))));
        } else {
            System.out.println("return: " + number);
            return number;
        }
    }
}
