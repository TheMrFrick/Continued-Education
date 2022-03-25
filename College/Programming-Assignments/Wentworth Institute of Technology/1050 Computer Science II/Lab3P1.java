package edu.wit.cs.comp1050;

import java.util.Scanner;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author TheMrFrick
 */
public class Lab3P1 {

    public static void main(String[] args) {
        // TODO: write your code here
        Scanner input = new Scanner(System.in);
        double a,b,c,d,e,f;
        System.out.print("Enter a,b,c,d,e,f: ");
        a = input.nextDouble();
        b = input.nextDouble();
        c = input.nextDouble();
        d = input.nextDouble();
        e = input.nextDouble();
        f = input.nextDouble();
        LinearEquation le = new LinearEquation(a,b,c,d,e,f);
        if(le.isSolvable()){
            System.out.println("X is " + le.getX() + " and y is " + le.getY());
        } else{
            System.out.println("The equation has no solution.");
            System.exit(0);
        }
    }
}

class LinearEquation {

    // TODO: write your code here
    private double a;
    private double b;
    private double c;
    private double d;
    private double e;
    private double f;

    LinearEquation(double a, double b, double c, double d, double e, double f) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
        this.e = e;
        this.f = f;
    }

    /**
     * @return the a
     */
    public double getA() {
        return a;
    }

    /**
     * @return the b
     */
    public double getB() {
        return b;
    }

    /**
     * @return the c
     */
    public double getC() {
        return c;
    }

    /**
     * @return the d
     */
    public double getD() {
        return d;
    }

    /**
     * @return the e
     */
    public double getE() {
        return e;
    }

    /**
     * @return the f
     */
    public double getF() {
        return f;
    }

    public boolean isSolvable() {
        if (((a * d) - (b * c)) != 0) {
            return true;
        } else {
            return false;
        }
    }
    
    public double getX(){
        double x;
        x = ((e*d - b*f) / (a*d - b*c));
        return x;
    }
    
    public double getY(){
        double y;
        y = ((a*f - e*c) / (a*d - b*c));
        return y;
    }
}
