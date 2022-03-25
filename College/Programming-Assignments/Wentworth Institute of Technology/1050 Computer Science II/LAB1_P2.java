package edu.wit.cs.comp1050;

import java.util.Scanner;

public class LAB1_P2 {

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        System.out.println("Enter package weight:");
        double weight = input.nextDouble();
        double price = checkPrice(weight);
        if (price == 0) {
            System.out.println("The package cannot be shipped.");
        } else {
            System.out.println("The shipping cost is $" + price);
        }
    }

    public static double checkPrice(double weight) {
        if (weight > 0 && weight <= 1) {
            return 3.5;
        } else if (weight > 1 && weight <= 3) {
            return 5.5;
        } else if (weight > 3 && weight <= 10) {
            return 8.5;
        } else if (weight > 10 && weight <= 20) {
            return 10.5;
        } else if (weight > 20) {
            return 0;
        } else {
            return 0;
        }
    }
}
