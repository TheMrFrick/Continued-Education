package edu.wit.cs.comp1050;

import java.util.Scanner;

public class LAB1_P1 {
	public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        System.out.println("Enter the amount of water in kilograms:");
        double water = input.nextDouble();
        System.out.println("Enter the initial Temperature (Celsius):");
        double startTemp = input.nextDouble();
        System.out.println("Enter the final temperature (Celsius): ");
        double finalTemp = input.nextDouble();
        double energy = water * (finalTemp - startTemp) * 4184;
        System.out.println("The energy needed is:" + energy);
    }
}
