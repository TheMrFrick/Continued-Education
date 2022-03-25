package edu.wit.cs.comp1050;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

public class Lab5P2 {

    public static void main(String[] args) {
        // TODO: write your code here
        ArrayList<Integer> intList = new ArrayList<>();
        Scanner input = new Scanner(System.in);
        System.out.println("Enter Integers (end with character 'q')");
        while (input.hasNextInt()) {
            int in = input.nextInt();
            intList.add(in);
        }
        removeIdenticalNumbers(intList);
        System.out.println("The distinct integers: " + intList.toString());
    }
    
    public static void removeIdenticalNumbers(ArrayList<Integer> list) {
        // TODO: write your code here
        Object[] st = list.toArray();
        for (Object s : st) {
            if (list.indexOf(s) != list.lastIndexOf(s)) {
                list.remove(list.lastIndexOf(s));
            }
        }
 //       System.out.println(list.toString());
    }
}

