package friends;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

/*
 * Large_list.txt connectors:
 * expected: [joanna, Neil, Lauren, Lee, Dana, Charlie, Timothy, earl, Jose, Sharon, Julie, Meredith, Jeff, Amanda, Theodore, John, Matt]
 * my: [matt, john, lauren, lee, dana, joanna, julie, sharon, theodore, jeff] missing [niel,jose,amanda]
 * Tom's: [john, matt, joanna, neil, lauren, debra, theresa, lee, dana, benny, charlie, timothy, meredith, julie, earl, jose, sharon, jeff, amanda, theodore] extra: [debra, benny, theresa]
 * 
 */

public class driver {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		File f = new File("large_list.txt");
		Scanner file;
		try {
			file = new Scanner(f);
			Graph g = new Graph(file);
			Friends friend = new Friends();
			System.out.println(g.map.toString());
			System.out.println(friend.shortestChain(g, "michele", "michele"));
			System.out.println(friend.cliques(g,"rutgers"));
			System.out.println(friend.connectors(g));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
