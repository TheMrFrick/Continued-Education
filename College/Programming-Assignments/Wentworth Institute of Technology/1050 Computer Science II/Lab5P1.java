package edu.wit.cs.comp1050;

import java.util.Scanner;

public class Lab5P1 {

    public static void main(String[] args) {
        // TODO: write your code here
        Scanner input = new Scanner(System.in);
        System.out.println("Enter three sides:");
        double side1 = input.nextDouble(), side2 = input.nextDouble(), side3 = input.nextDouble();
        System.out.println("Enter the color:");
        String color = input.next();
        System.out.println("Enter a boolean value for filled:");
        boolean bool = input.nextBoolean();
        Triangle t1 = new Triangle(side1,side2, side3);
        t1.setColor(color);
        t1.setFilled(bool);
        System.out.printf("The area is %.2f%n",  + t1.getArea());
        System.out.printf("The perimeter is %.2f%n",  t1.getPerimeter());
        System.out.println(t1.toString());
    }
}

class Triangle extends GeometricObject {

    // TODO: write your code here
    // You still need to implement a few methods
    private double side1 = 1, side2 = 1, side3 = 1;
    public Triangle() {
        
    }
    
    public Triangle(double side1, double side2, double side3){
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
    }
    @Override

    /**
     * Override method findArea in GeometricObject
     */
    public double getArea() {
        // TODO: write your code here
        double s = (side1 + side2 + side3)/2;
        double area = Math.sqrt(s*(s - side1)*(s - side2)*(s-side3));
        return area;
    }

    @Override
    /**
     * Override method findPerimeter in GeometricObject
     */
    public double getPerimeter() {
        // TODO: write your code here
        return side1 + side2 + side3;
    }

    @Override
    public String toString() {
        // TODO: write your code here
        return "Triangle: side1 = " + side1 + " side2 = " + side2 + " side3 = " + side3;
    }

    /**
     * @return the side1
     */
    public double getSide1() {
        return side1;
    }

    /**
     * @param side1 the side1 to set
     */
    public void setSide1(double side1) {
        this.side1 = side1;
    }

    /**
     * @return the side2
     */
    public double getSide2() {
        return side2;
    }

    /**
     * @param side2 the side2 to set
     */
    public void setSide2(double side2) {
        this.side2 = side2;
    }

    /**
     * @return the side3
     */
    public double getSide3() {
        return side3;
    }

    /**
     * @param side3 the side3 to set
     */
    public void setSide3(double side3) {
        this.side3 = side3;
    }
}
