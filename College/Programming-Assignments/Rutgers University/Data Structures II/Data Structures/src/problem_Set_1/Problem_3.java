package problem_Set_1;

import java.util.ArrayList;
import java.util.Collections;

/*
* Exercise 3.14 of the textbook
A card game program keeps a deck of cards in an array. Give an algorithm to "unshuffle" the deck so that all the cards of a suit are grouped together, and within each suit, the cards are in ascending order or rank--consider the ace as the lowest ranked card. Since there are only 52 cards, space is not an issue so you can use extra space if needed. The only constraint is that your algorithm be as fast as possible.

What is the worst case big O running time of your algorithm? What are the basic operations you used in your analysis? Is the average big O running time different from the worst case?

 */

public class Problem_3 {
	public static void main(String[] args) {
		String[] deck = createDeck();
		ArrayList<String> deckAL = arrayToAL(deck);
		ArrayList<String> club = sort(sepClubs(deckAL));
		ArrayList<String> heart = sort(sepHearts(deckAL));
		ArrayList<String> spade = sort(sepSpades(deckAL));
		ArrayList<String> diamond = sort(sepDiamonds(deckAL));
		if(!deckAL.isEmpty()) {
			deckAL.clear();
		}
		deckAL.addAll(club);
		deckAL.addAll(heart);
		deckAL.addAll(spade);
		deckAL.addAll(diamond);
		deck = (String[]) deckAL.toArray(deck);
		printArray(deck);
	}

	private static void printArray(String[] deck) {
		// TODO Auto-generated method stub
		for(String x: deck) {
			System.out.print(x + " ");
		}
	}

	public static String[] createDeck() {
		String[] temp = new String[52];
		temp[0] = "A-H";
		temp[1] = "2-H";
		temp[2] = "3-H";
		temp[3] = "4-H";
		temp[4] = "5-H";
		temp[5] = "6-H";
		temp[6] = "7-H";
		temp[7] = "8-H";
		temp[8] = "9-H";
		temp[9] = "10-H";
		temp[10] = "J-H";
		temp[11] = "Q-H";
		temp[12] = "K-H";
		temp[13] = "A-C";
		temp[14] = "2-C";
		temp[15] = "3-C";
		temp[16] = "4-C";
		temp[17] = "5-C";
		temp[18] = "6-C";
		temp[19] = "7-C";
		temp[20] = "8-C";
		temp[21] = "9-C";
		temp[22] = "10-C";
		temp[23] = "J-C";
		temp[24] = "Q-C";
		temp[25] = "K-C";
		temp[26] = "A-S";
		temp[27] = "2-S";
		temp[28] = "3-S";
		temp[29] = "4-S";
		temp[30] = "5-S";
		temp[31] = "6-S";
		temp[32] = "7-S";
		temp[33] = "8-S";
		temp[34] = "9-S";
		temp[35] = "10-S";
		temp[36] = "J-S";
		temp[37] = "Q-S";
		temp[38] = "K-S";
		temp[39] = "A-D";
		temp[40] = "2-D";
		temp[41] = "3-D";
		temp[42] = "4-D";
		temp[43] = "5-D";
		temp[44] = "6-D";
		temp[45] = "7-D";
		temp[46] = "8-D";
		temp[47] = "9-D";
		temp[48] = "10-D";
		temp[49] = "J-D";
		temp[50] = "Q-D";
		temp[51] = "K-D";
		return temp;
	}

	public static ArrayList<String> arrayToAL(String[] arr) {
		ArrayList<String> temp = new ArrayList<>();
		for (String x : arr) {
			temp.add(x);
		}
		Collections.shuffle(temp);
		return temp;
	}

	public static ArrayList<String> sepHearts(ArrayList<String> deck) {
		ArrayList<String> temp = new ArrayList<>();
		for (String x : deck) {
			if (x.contains("H")) {
				temp.add(x);
			}
		}
		return temp;
	}

	public static ArrayList<String> sepDiamonds(ArrayList<String> deck) {
		ArrayList<String> temp = new ArrayList<>();
		for (String x : deck) {
			if (x.contains("D")) {
				temp.add(x);
			}
		}
		return temp;
	}

	public static ArrayList<String> sepSpades(ArrayList<String> deck) {
		ArrayList<String> temp = new ArrayList<>();
		for (String x : deck) {
			if (x.contains("S")) {
				temp.add(x);
			}
		}
		return temp;
	}

	public static ArrayList<String> sepClubs(ArrayList<String> deck) {
		ArrayList<String> temp = new ArrayList<>();
		for (String x : deck) {
			if (x.contains("C")) {
				temp.add(x);
			}
		}
		return temp;
	}

	public static ArrayList<String> sort(ArrayList<String> deck) {
		Collections.sort(deck);
		return deck;
	}
}
