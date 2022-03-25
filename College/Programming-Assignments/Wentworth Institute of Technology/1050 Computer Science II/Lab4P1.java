/*********************************************************************
*	LAB 4 - Problem 1
*
*	Kyle Frick
*	COMP1050-09/10  (ENTER YOUR SESSION, EITHER 09/10 or 11/12 for XXXXX)
*	01/28/2015  (UPDATE THE DATE) 
*
**********************************************************************
*	Problem Description (PLEASE UPDATE THE DESCRIPTION)
*
*	To help you understand wrapper classes, we want you to design your Integer-like wrapper class. Please
        design a class named MyInteger. The class contains:
            • An int data field named value that stores the int value represented by this object.
            • A constructor that creates a MyInteger object for the specified int value.
            • A getter method that returns the int value.
            • The methods isEven(), isOdd(), and isPrime() that return true if the value in this object is
        even, odd, or prime, respectively.
            • The static methods isEven(int), isOdd(int), and isPrime(int) that return true if the
        specified value is even, odd, or prime, respectively.
            • The static methods isEven(MyInteger), isOdd(MyInteger), and isPrime(MyInteger) that
        return true if the specified value is even, odd, or prime, respectively.
            • The methods equals(int) and equals(MyInteger) that return true if the value in this object is
        equal to the specified value.
            • A static method parseInt(char[]) that converts an array of numeric characters to an int value.
            • A static method parseInt(String) that converts a string into an int value.
        Please write a program to test all methods in the class.
*
***********************************************************************
*	Analysis (PLEASE UPDATE THE DESCRIPTION)
*
*       Things that need to be done in the project:
        *	Creating instances of MyInteger
        *
        *	Things to be done: Filling out the methods of the MyInteger class that was already created.
        *
        *	Suggested Results:
        *           n1 is even? False
                    n1 is prime? true
                    15 is prime? False
                    parseInt(char[]) for { '4', '4', '7', '8' } = 4478
                    parseInt(String) for "4378" = 4378
                    n2 is odd? False
                    45 is odd? True
                    n1 is equal to n2? false
                    n1 is equal to 7? true
       
       * Ideas learned from the project:
       *    When creating an instance variable, it is creating an object of the "class".
       *    The classes could contain instance or static sub-objects.
       *        Instance:
       *            Apart of a singular object or call of the class.
       *                E.g.
       *                    private int value;
       *        Static:
       *            The object is contained and accessed through the class itself, but every instance of the
       *            class gets the same value. "Shared" among instances.
*
**********************************************************************/

package edu.wit.cs.comp1050;

public class Lab4P1 {

    public static void main(String[] args) {
        // TODO: write your code here
        
        //Testing for Expected Results
        MyInteger n1 = new MyInteger(7);
        MyInteger n2 = new MyInteger(24);
        System.out.println("n1 = " + n1.getValue());
        System.out.println("n2 = " + n2.getValue());
        System.out.println("n1 is even? " + n1.isEven());
        System.out.println("n1 is prime? " + n1.isPrime());
        System.out.println("15 is prime? "+ MyInteger.isPrime(15));
        char[] num = {'4','4','7','8'};
        System.out.println("parseInt(char[]) for { '4', '4', '7', '8' } = " + MyInteger.parseInt(num));
        System.out.println("parseInt(String) or \"4378\" = " + MyInteger.parseInt("4378"));
        System.out.println("n2 is odd? " +n2.isOdd());
        System.out.println("45 is odd? " + MyInteger.isOdd(45));
        System.out.println("n1 is equal to n2? " + n1.equals(n2));
        System.out.println("n1 is equal to 7? " + n1.equals(7));
    }
}

class MyInteger {

    // TODO: write your code here
    private int value;

    //returns the value
    public int getValue() {
        // TODO: write your code here
        return value;
    }

    //Constructor with parameter of the value
    public MyInteger(int value) {
        // TODO: write your code here
        this.value = value;
    }

    //checking to see if something is prime.
    public boolean isPrime() {
        // TODO: write your code here
        //prime is when a number is only divisible by itself and 1.
        int check = value / 2;
        if (check <= 1) {
            return true;
        }
        for (int i = 2; i < check; i++) {
            if (check % i == 0) {
                return false;
            }
        }
        return true;
    }

    //class method that checks for any number being prime
    public static boolean isPrime(int num) {
        // TODO: write your code here
        int check = num / 2;
        if (check <= 1) {
            return true;
        }
        for (int i = 2; i < check; i++) {
            if (check % i == 0) {
                return false;
            }
        }
        return false;
    }

    //class method to check for an instance of the class is prime
    public static boolean isPrime(MyInteger o) {
        // TODO: write your code here
        int check = o.getValue() / 2;
        if (check <= 1) {
            return true;
        }
        for (int i = 2; i < check; i++) {
            if (check % i == 0) {
                return false;
            }
        }
        return false;
    }

    //checks to see if the value is even
    public boolean isEven() {
        // TODO: write your code here
        if (value % 2 == 0) {
            return true;
        }
        return false;
    }

    //checks to see if the value is odd
    public boolean isOdd() {
        // TODO: write your code here
        if (isEven()) {
            return false;
        } else {
            return true;
        }
    }

    //class method to check to see if a number is even.
    public static boolean isEven(int n) {
        // TODO: write your code here
        if (n % 2 == 0) {
            return true;
        } else {
            return false;
        }
    }

    //class method to check to see if a number is odd.
    public static boolean isOdd(int n) {
        // TODO: write your code here
        if (n % 2 == 0) {
            return false;
        } else {
            return true;
        }
    }

    //class method to compare class values
    public static boolean isEven(MyInteger n) {
        // TODO: write your code here
        if (n.getValue() % 2 == 0) {
            return true;
        } else {
            return false;
        }
    }

    //class method to compare this.value against another number.
    public boolean equals(int anotherNum) {
        // TODO: write your code here
        if (value == anotherNum) {
            return true;
        } else {
            return false;
        }
    }

    //instance method to compare this.value against another instance.
    public boolean equals(MyInteger o) {
        // TODO: write your code here
        if (value == o.getValue()) {
            return true;
        } else {
            return false;
        }
    }

    //class method to return a number from a char[].
    public static int parseInt(char[] numbers) {
	// numbers consists of digit characters.
        // For example, if numbers is {'1', '2', '5'}, the return value
        // should be 125. Please note that
        // numbers[0] is '1'
        // numbers[1] is '2'
        // numbers[2] is '5'

        // TODO: write your code here
        int[] num = new int[numbers.length];
        for (int i = 0; i < numbers.length; i++) {
            num[i] = ((int) numbers[i]) - 48;
        }
        int number = 0;
        for (int i = 0; i < num.length; i++) {
            number += num[i];
            if (i + 1 != num.length) {
                number *= 10;
            }
        }
        return number;
    }

    //class method to return an int from a string.
    public static int parseInt(String s) {
		// s consists of digit characters.
        // For example, if s is "125", the return value
        // should be 125.
        
        // TODO: write your code here
        int[] num = new int[s.length()];
        for (int i = 0; i < s.length(); i++) {
            num[i] = ((int) s.charAt(i)) - 48;
        }
        int number = 0;
        for (int i = 0; i < num.length; i++) {
            number += num[i];
            if (i + 1 != num.length) {
                number *= 10;
            }
        }

        return number;
    }
}