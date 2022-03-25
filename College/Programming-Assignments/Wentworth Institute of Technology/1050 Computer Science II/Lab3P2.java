package edu.wit.cs.comp1050;


import java.util.Scanner;

public class Lab3P2 {

    public static void main(String[] args) {
        // TODO: write your code here
        MyPoint p1 = new MyPoint();
        MyPoint p2 = new MyPoint(10, 30.5);
        System.out.println("The distance is (using MyPoint class's distance method): " + p1.distance(p2));
        System.out.println("The distance is (using one point's distance method): " + p1.distance(p1, p2));
    }
}

class MyPoint {

    // TODO: write your code here

    private double x, y;

    MyPoint() {
        x = 0;
        y = 0;
    }

    MyPoint(double x, double y) {
        this.x = x;
        this.y = y;
    }

    /**
     * @return the x
     */
    public double getX() {
        return x;
    }

    /**
     * @return the y
     */
    public double getY() {
        return y;
    }

    public double distance(MyPoint point) {
        double distance;
        distance = Math.sqrt(Math.pow((point.getX() - this.x), 2) + Math.pow(point.getY() - this.y, 2));
        return distance;
    }

    public double distance(MyPoint p1, MyPoint p2) {
        double distance;
        distance = Math.sqrt(Math.pow((p1.getX() - p2.getX()), 2) + Math.pow((p1.getY() - p2.getY()), 2));
        return distance;
    }
}

