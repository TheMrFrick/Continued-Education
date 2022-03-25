package problem_Set_1;

import java.util.Scanner;

/*
 * 
Exercise 3.7 of the textbook.
An algorithm prints the following pattern:

*
* *
* * *
* * * *
* * * * *

(a) What are the basic operations performed by the algorithm that you would count towards its running time?
(b) Count the number of these basic operations for the specific output shown above.
(c) The number of lines printed in the preceding pattern is 5. Assume that the algorithm can extend this pattern for any number of lines (line number i has i stars). If the number of lines, n is the input to the algorithm, how many basic operations are performed as a function of n?
(d) Write your answer to the above question as a big O order.
 * 
 * 
 */

public class Problem_1 {
	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		String res = "";
		System.out.println("Enter an int");
		int val = input.nextInt();
		for(int i = 0; i < val; i++) {
			res += "*";
			System.out.println(res);
		}
	}
}
