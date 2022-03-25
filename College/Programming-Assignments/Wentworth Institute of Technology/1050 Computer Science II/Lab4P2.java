/*********************************************************************
*	LAB 4 - Problem 2
*
*	Kyle Frick
*	COMP1050-09/10  (ENTER YOUR SESSION, EITHER 09/10 or 11/12 for XXXXX)
*	01/28/2015  (UPDATE THE DATE) 
*
**********************************************************************
*	Problem Description (PLEASE UPDATE THE DESCRIPTION)
*
*	We want you to create a class RoomPeople that can be used to record the number of people in the rooms
        of a building. The class has the attributes:
            • numberInRoom - the number of people in a room
            • totalNumber - the total number of people in all rooms as a static variable
        The class has the following methods:
            • addOneToRoom - adds a person to the room and increases the value of totalNumber
            • removeOneFromRoom - removes a person from the room, ensuring that numberInRoom does not go
        below zero, and decreases the value of totalNumber as needed
            • getNumber - returns the number of people in the room
            • getTotal - a static method that returns the total number of people
        Please write a program to test the class RoomPeople.
*
***********************************************************************
*	Analysis (PLEASE UPDATE THE DESCRIPTION)
*
*	Creating two instances of a room.
*
*	Can add or subtract people from the room, but can't remove negative people
*
*       Outputs:
*           Suggested Results:
*               Add two to room a and three to room b.
                Room a holds 2
                Room b holds 3
                Total in all rooms is 5
                Remove two from both rooms.
                Room a holds 0
                Room b holds 1
                Total in all rooms is 1
                Remove two from room a (should not change the values)
                Room a holds 0
                Room b holds 1
                Total in all rooms is 1
*
**********************************************************************/

package edu.wit.cs.comp1050;

public class Lab4P2 {

    public static void main(String[] args) {
        // TODO: write your code here
        RoomPeople a = new RoomPeople(), b = new RoomPeople();
        a.addOneToRoom();
        a.addOneToRoom();
        b.addOneToRoom();
        b.addOneToRoom();
        b.addOneToRoom();
        System.out.println("After adding two to room a and three to room b:");
        System.out.println("Room a holds " + a.getNumber());
        System.out.println("Room b holds " + b.getNumber());
                System.out.println("Total in all rooms is " + RoomPeople.getTotal());
        System.out.println("Remove two from both rooms.");
        for (int i = 0; i < 2; i++) {
            a.removeOneFromRoom();
            b.removeOneFromRoom();
        }
        System.out.println("Room a holds " + a.getNumber());
        System.out.println("Room b holds " + b.getNumber());
        System.out.println("Total in all rooms is " + RoomPeople.getTotal());
        System.out.println("Remove two from room a (should not change the values)");
        System.out.println("Room a holds " + a.getNumber());
        System.out.println("Room b holds " + b.getNumber());
        System.out.println("Total in all rooms is " + RoomPeople.getTotal());
    }
}

class RoomPeople {

    // TODO: write your code here
    private int numberInRoom;
    private static int totalNumber;

    public static int getTotal() {
        // TODO: write your code here
        return totalNumber;
    }

    public RoomPeople() {
        // TODO: write your code here
    }

    public void addOneToRoom() {
        // TODO: write your code here
        numberInRoom++;
        totalNumber++;
    }

    public void removeOneFromRoom() {
        // TODO: write your code here
        if (getNumber() == 0) {
            //nothing occurs
        } else {
            numberInRoom--;
            totalNumber--;
        }
    }

    public int getNumber() {
    	// TODO: write your code here

        return numberInRoom;
    }
}
  
