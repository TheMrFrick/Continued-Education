package edu.wit.cs.comp1050;

import java.util.Scanner;

public class LAB1_P4 {

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        System.out.println("Enter a string:");
        String in = input.nextLine();
        char[] inChar = in.toCharArray();
        int count = 0;
        for (int i = 0; i < in.length(); i++) {
            if (Character.isUpperCase(inChar[i])) {
                count++;
            }
        }
        System.out.println("The number of uppercase letters is " + count);
    }
}
