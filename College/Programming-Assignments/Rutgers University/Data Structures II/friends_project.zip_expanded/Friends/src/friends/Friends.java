package friends;

import structures.Queue;
import structures.Stack;

import java.util.*;

public class Friends {

	/**
	 * Finds the shortest chain of people from p1 to p2. Chain is returned as a
	 * sequence of names starting with p1, and ending with p2. Each pair (n1,n2) of
	 * consecutive names in the returned chain is an edge in the graph.
	 * 
	 * @param g
	 *            Graph for which shortest chain is to be found.
	 * @param p1
	 *            Person with whom the chain originates
	 * @param p2
	 *            Person at whom the chain terminates
	 * @return The shortest chain from p1 to p2. Null if there is no path from p1 to
	 *         p2
	 */
	public static ArrayList<String> shortestChain(Graph g, String p1, String p2) {

		/** COMPLETE THIS METHOD **/
		// error cases
		if (!g.map.containsKey(p1)) {
			return null;
		}
		if (!g.map.containsKey(p2)) {
			return null;
		}
		if (g.members[g.map.get(p1).intValue()].first == null) {
			return null;
		}
		if (g.members[g.map.get(p2).intValue()].first == null) {
			return null;
		}
		// end of error cases

		// prep for BFS
		Queue<Person> next = new Queue<>();
		int[] dist = new int[g.members.length];
		int[] prevVec = new int[g.members.length];
		boolean[] visited = new boolean[g.members.length];
		// initializing all things
		for (int i = 0; i < g.members.length; i++) {
			dist[i] = Integer.MAX_VALUE;
			if (g.members[i].name.equals(p1)) {
				dist[i] = 0;
			}
			visited[i] = false;
		}
		// End of Prep
		
		// Start of BFS
		visited[g.map.get(p1).intValue()] = true;
		for (int i = 0; i < visited.length; i++) {
			System.out.println(i + " - " + visited[i]);
		}
		next.enqueue(g.members[g.map.get(p1).intValue()]);
		while (!next.isEmpty()) {
			Person v = next.dequeue();
			System.out.println(v.name);
			for (Friend ptr = v.first; ptr != null; ptr = ptr.next) {
				int distance = dist[g.map.get(v.name).intValue()];
				System.out.println("Distance: " + distance);
				distance += 1;
				if (dist[ptr.fnum] == Integer.MAX_VALUE) {
					dist[ptr.fnum] = distance;
					prevVec[ptr.fnum] = g.map.get(v.name).intValue();
				}
				if (visited[ptr.fnum]) {
					if (distance < dist[ptr.fnum]) {
						dist[ptr.fnum] = distance;
						prevVec[ptr.fnum] = g.map.get(v.name).intValue();
					}
					continue;
				}
				System.out.println(ptr.fnum + " - " + dist[ptr.fnum] + " - " + prevVec[ptr.fnum]);
				visited[(ptr.fnum)] = true;
				next.enqueue(g.members[ptr.fnum]);
			}
		}
		// End of BFS
		//printing out for self viewing
		System.out.println("------------Vist-----------------");
		for (int i = 0; i < visited.length; i++) {
			System.out.println(g.members[i].name + "\t- " + visited[i]);
		}
		System.out.println("-------------Dist----------------");
		for (int i = 0; i < dist.length; i++) {
			System.out.println(g.members[i].name + "\t- " + dist[i]);
		}
		System.out.println("---------------------------------");

		// checking to see if p2 was reached
		if (!visited[g.map.get(p2).intValue()]) {
			return null;
		}

		// getting the shortest length
		ArrayList<String> path = new ArrayList<>();

		if (p1.equals(p2)) {
			path.add(p1);
			return path;
		}

		//retracing the path
		Stack<String> reversePath = new Stack<>();
		reversePath.push(g.members[g.map.get(p2).intValue()].name);
		int index = prevVec[g.map.get(p2).intValue()];
		while (index != g.map.get(p1).intValue()) {
			reversePath.push(g.members[index].name);
			index = prevVec[index];
		}
		reversePath.push(g.members[g.map.get(p1).intValue()].name);
		while (!reversePath.isEmpty()) {
			path.add(reversePath.pop());
		}
		// FOLLOWING LINE IS A PLACEHOLDER TO MAKE COMPILER HAPPY
		// CHANGE AS REQUIRED FOR YOUR IMPLEMENTATION
		return path;
	}

	/**
	 * Finds all cliques of students in a given school.
	 * 
	 * Returns an array list of array lists - each constituent array list contains
	 * the names of all students in a clique.
	 * 
	 * @param g
	 *            Graph for which cliques are to be found.
	 * @param school
	 *            Name of school
	 * @return Array list of clique array lists. Null if there is no student in the
	 *         given school
	 */
	public static ArrayList<ArrayList<String>> cliques(Graph g, String school) {
		ArrayList<ArrayList<String>> cliq = new ArrayList<>();
		ArrayList<String> thisCliq = new ArrayList<>();

		// prep for BFS
		boolean[] visited = new boolean[g.members.length];
		// initializing all things
		for (int i = 0; i < g.members.length; i++) {
			visited[i] = false;
		}
		// End of Prep

		for (int i = 0; i < visited.length; i++) {
			if (!visited[i]) {
				thisCliq = cliqueSearch(g, i, visited, school);
				if (!thisCliq.isEmpty()) {
					cliq.add(thisCliq);
				}
			}
		}

		// FOLLOWING LINE IS A PLACEHOLDER TO MAKE COMPILER HAPPY
		// CHANGE AS REQUIRED FOR YOUR IMPLEMENTATION
		if (cliq.isEmpty()) {
			return null;
		}
		return cliq;

	}

	private static ArrayList<String> cliqueSearch(Graph g, int i, boolean[] visited, String school) {
		// TODO Auto-generated method stub
		ArrayList<String> clique = new ArrayList<>();

		Queue<Person> next = new Queue<>();
		next.enqueue(g.members[i]);
		while (!next.isEmpty()) {
			Person v = next.dequeue();
			visited[g.map.get(v.name).intValue()] = true;
			if (v.student) {
				if (v.school.equals(school)) {
					clique.add(v.name);
					for (Friend ptr = v.first; ptr != null; ptr = ptr.next) {
						if (visited[ptr.fnum]) {
							continue;
						}
						Person friend = g.members[ptr.fnum];
						if (friend.student) {
							if (friend.school.equals(school)) {
								next.enqueue(friend);
							}
						}
					}
				} else {
					break;
				}
			} else {
				break;
			}
		}
		// rid duplicates in set
		Set<String> s = new HashSet<>(clique);

		// clear current clique
		clique.clear();
		// add back to clique
		for (String x : s) {
			clique.add(x);
		}
		return clique;
	}

	/**
	 * Finds and returns all connectors in the graph.
	 * 
	 * @param g
	 *            Graph for which connectors needs to be found.
	 * @return Names of all connectors. Null if there are no connectors.
	 */
	public static ArrayList<String> connectors(Graph g) {
		ArrayList<String> connects = dfs(g);
		// ArrayList<String> temp = new ArrayList<>();
		// // prep for BFS
		// boolean[] visited = new boolean[g.members.length];
		// // initializing all things
		// for (int i = 0; i < g.members.length; i++) {
		// visited[i] = false;
		// }
		// // End of Prep
		// for (int i = 0; i < visited.length; i++) {
		// if (!visited[i]) {
		// temp = connectSearch(g, i, visited);
		// for (String x : temp) {
		// connects.add(x);
		// }
		// }
		// }

		// FOLLOWING LINE IS A PLACEHOLDER TO MAKE COMPILER HAPPY
		// CHANGE AS REQUIRED FOR YOUR IMPLEMENTATION
		return connects;

	}

	private static ArrayList<String> connectSearch(Graph g, int i, boolean[] visited) {
		// TODO Auto-generated method stub
		//normal BFS
		ArrayList<String> con = new ArrayList<>();
		Queue<Person> next = new Queue<>();
		next.enqueue(g.members[i]);
		Person prev = g.members[i];
		while (!next.isEmpty()) {
			Person v = next.dequeue();
			visited[g.map.get(v.name).intValue()] = true;
			for (Friend ptr = v.first; ptr != null; ptr = ptr.next) {
				int count = 0;
				Person friend = g.members[ptr.fnum];
				ArrayList<String> chain = null;
				for (Friend f = friend.first; f != null; f = f.next) {
					count++;
				}
				System.out.println("Count: " + count + " " + v.name);
				if (count == 1) {
					con.add(v.name);
				}
				if (visited[ptr.fnum]) {
					continue;
				}
				next.enqueue(g.members[ptr.fnum]);
				prev = v;
			}
		}
		
		// rid duplicates in set
		Set<String> s = new HashSet<>(con);

		// clear current con
		con.clear();
		// add back to con
		for (String x : s) {
			con.add(x);
		}

		return con;
	}

	private static ArrayList<String> dfs(Graph g) {
		boolean[] visited = new boolean[g.members.length];
		ArrayList<String> connectors = new ArrayList<>();
		int[] dfsnum = new int[g.members.length];
		int[] back = new int[g.members.length];
		int currentNum = 0;
		int source = 0;
		for (int v = 0; v < visited.length; v++) {
			visited[v] = false;
		}
		for (int v = 0; v < visited.length; v++) {
			if (!visited[v]) {
				System.out.println("\nSTARTING AT " + g.members[v].name + "\n");
				dfs(v, visited, dfsnum, back, g, connectors, currentNum, v);
				if (g.members[v].first != null) {
					Friend ptr = g.members[v].first;
					while (ptr != null) {
						source++;
						ptr = ptr.next;
					}
					if (source >= 2) {
						dfs(g.members[v].first.fnum, new boolean[g.members.length], new int[g.members.length],
								new int[g.members.length], g, connectors, 0, g.members[v].first.fnum);

					}
				}
			}
		}
		for (int i = 0; i < g.members.length; i++) {
			System.out.println(g.members[i].name + ": " + dfsnum[i] + "," + dfsnum[i]);
		}

		return connectors;
	}

	// recursive dfs
	private static void dfs(int v, boolean[] visited, int[] dfsnum, int[] back, Graph g, ArrayList<String> connectors,
			int currentNum, int source) {
		visited[v] = true;
		dfsnum[v] = currentNum;
		back[v] = currentNum;
		currentNum++;
		System.out.println("\tvisiting " + g.members[v].name);
		for (Friend w = g.members[v].first; w != null; w = w.next) {
			if (!visited[w.fnum]) {
				System.out.println("\t" + g.members[v].name + "--" + g.members[w.fnum].name);
				dfs(w.fnum, visited, dfsnum, back, g, connectors, currentNum, source);
				if (!connectors.contains(g.members[v].name) && dfsnum[v] <= back[w.fnum] && v != source) {
					connectors.add(g.members[v].name);
				}
				if (dfsnum[v] > back[w.fnum]) {
					back[v] = Math.min(back[v], back[w.fnum]);
				}
			} else {
				back[v] = Math.min(back[v], dfsnum[w.fnum]);
			}
		}

	}

}
