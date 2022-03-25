package edu.wit.cs.comp1050;

import java.util.Scanner;

public class LAB1_P3 {

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        System.out.println("Enter a year:");
        int year = input.nextInt();
        while (true) {
            if (year < 0) {
                System.out.println("Year must be positive");
                year = input.nextInt();
            } else {
                break;
            }
        }
        System.out.println("Enter a month:");
        String month = input.next();
        while (true) {
            if (month.length() != 3) {
                System.out.println("The Month can only be 3 letters");
                month = input.next();
            } else {
                break;
            }
        }
        boolean leap = checkLeap(year);
        if (leap == true && month.equals("Feb")) {
            System.out.println(month + " " + year + " has 29 days");
        } else if (month.equals("Jan") || month.equals("Mar") || month.equals("May") || month.equals("Jul")
                || month.equals("Aug") || month.equals("Oct") || month.equals("Dec")) {
            System.out.println(month + year + "has 31 days");
        } else if (month.equals("Apr") || month.equals("Jun") || month.equals("Sep") || month.equals("Nov")) {
            System.out.println(month + year + "has 30 days");
        }
        if (leap == false && month.equals("Feb")) {
            System.out.println(month + year + "has 28 days");
        }
    }

    public static boolean checkLeap(int year) {
        if (year % 4 != 0) {
            return false;
        } else if (year % 100 == 0) {
            return false;
        } else if (year % 400 == 0) {
            return true;
        }
        return true;
    }
}
