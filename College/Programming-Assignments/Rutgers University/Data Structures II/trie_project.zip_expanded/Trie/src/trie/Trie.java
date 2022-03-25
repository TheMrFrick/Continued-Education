package trie;

import java.util.ArrayList;

/**
 * This class implements a Trie.
 * 
 * @author Sesh Venugopal
 *
 */
public class Trie {

	// prevent instantiation
	private Trie() {
	}

	/**
	 * Builds a trie by inserting all words in the input array, one at a time, in
	 * sequence FROM FIRST TO LAST. (The sequence is IMPORTANT!) The words in the
	 * input array are all lower case.
	 * 
	 * @param allWords
	 *            Input array of words (lowercase) to be inserted.
	 * @return Root of trie with all words inserted from the input array
	 */
	public static TrieNode buildTrie(String[] allWords) {
		/** COMPLETE THIS METHOD **/
		TrieNode root = null;
		if (allWords.length == 0) {
			return new TrieNode(null, null, null);
		}
		// step 1: iterate next word
		for (int i = 0; i < allWords.length; i++) {
			// step 2: pas word, word list, index of word and tree to method
			root = addWord(root, allWords[i], allWords, i);
		}
		// FOLLOWING LINE IS A PLACEHOLDER TO ENSURE COMPILATION
		// MODIFY IT AS NEEDED FOR YOUR IMPLEMENTATION
		return root;
	}

	private static TrieNode addWord(TrieNode root, String x, String[] words, int index) {
		// TODO Auto-generated method stub
		// Step 3: check if root is null
		print(root, words);
		if (root == null) {
			TrieNode child = new TrieNode(new Indexes(0, (short) 0, (short) (x.length() - 1)), null, null);
			return new TrieNode(null, child, null);
		}
		StringBuilder prefix = new StringBuilder();
		TrieNode ptr = root.firstChild;
		TrieNode prev = root;
		TrieNode parent = root;
		System.out.println(ptr);
		boolean goToSib = false;
		boolean wentToSib = false;
		// step 4: check if word and child are similar.
		int iteration = 0;
		for (; ptr != null; ptr = ptr.firstChild) {
			// get word
			System.out.println("Previous = Root: " + (prev == root));
			System.out.println("Goto Sibling: " + goToSib);
			if (goToSib) {
				System.out.println("PTR Before: " + ptr);
				System.out.println("Iterations: " + iteration);
				for (int i = 0; i < iteration; i++) {
					if (parent == root) {
						System.out.println("Parent is root");
					} else {
						System.out.println(parent);
					}
					prev = ptr;
					ptr = ptr.sibling;
					System.out.println(ptr);
				}
				System.out.println("PTR After: " + ptr);
				goToSib = false;
				wentToSib = true;
			}
			String word = words[ptr.substr.wordIndex].substring(0, ptr.substr.endIndex + 1);
			System.out.println("Word: " + word);
			System.out.println("X: " + x);
			// check if sibling exists,
			// if so checks if sibling is more similar to the word
			if (ptr.sibling != null) {
				if (prefix.length() != 0) {
					prefix.delete(0, prefix.length());
				}
				for (int i = 0; i < word.length(); i++) {
					if (word.charAt(i) == x.charAt(i)) {
						prefix.append(x.charAt(i));
					} else {
						break;
					}
				}
				StringBuilder prefixSib = new StringBuilder();
				String sibWord = words[ptr.sibling.substr.wordIndex].substring(0, ptr.sibling.substr.endIndex + 1);
				System.out.println("SibWord: " + sibWord);
				for (int i = 0; i < sibWord.length(); i++) {
					if (sibWord.charAt(i) == x.charAt(i)) {
						prefixSib.append(x.charAt(i));
					} else {
						break;
					}
				}
				if (prefixSib.toString().length() > prefix.toString().length()) {
					System.out.println("PrefixSib: " + prefixSib.toString() + " prefix: " + prefix.toString());
					prefix = prefixSib;
					System.out.println("Prefix: " + prefix.toString());
					prev = ptr;
					ptr = parent;
					iteration++;
					goToSib = true;
					continue;
				}
			} else {
				if (prefix.length() != 0) {
					prefix.delete(0, prefix.length());
				}
				for (int i = 0; i < word.length(); i++) {
					if (word.charAt(i) == x.charAt(i)) {
						prefix.append(x.charAt(i));
					} else {
						break;
					}
				}
			}
			boolean wordParentEqual = false;
			if (parent != root) {
				String parPre = words[parent.substr.wordIndex].substring(0, parent.substr.endIndex + 1);
				System.out.println(parPre);
				if (prefix.toString().equals(parPre)) {
					wordParentEqual = true;
				}
			}
			System.out.println("Prefix: " + prefix.toString() + " Length: " + prefix.length());
			if (prefix.toString().equals(word)) {
				System.out.println("Prefix equals word: " + word);
				prefix.delete(0, prefix.length());
				parent = ptr;
				prev = ptr;
				iteration = 0;
				continue;
			}
			if (prefix.length() == 0 || wordParentEqual) {
				if (ptr.sibling != null) {
					System.out.println("Going to sib");
					prev = ptr;
					ptr = parent;
					System.out.println();
					goToSib = true;
					iteration++;
					continue;
				} else {
					TrieNode sib = new TrieNode(new Indexes(index, (short) 0, (short) (x.length() - 1)), null, null);
					if(parent != root) {
						sib.substr.startIndex = prev.substr.startIndex;
					}
					ptr.sibling = sib;
					break;
				}
			} else {
				// new node
				if (prev != root) {
					System.out.println("Prev Before adding to tree: " + prev);
				} else {
					System.out.println("Prev is root");
				}
				System.out.println("Ptr Before Adding to Tree: " + ptr);
				if (parent != root) {
					System.out.println("Parent Before adding to tree: " + parent);
				} else {
					System.out.println("Parent is root");
				}
				TrieNode temp = new TrieNode(new Indexes(index, (short) prefix.length(), (short) (x.length() - 1)),
						null, null);
				// prefix Node
				TrieNode prefixNode = new TrieNode(
						new Indexes(ptr.substr.wordIndex, (short) 0, (short) (prefix.length() - 1)), ptr, null);
				if (parent != root) {
					prefixNode.substr.startIndex = (short) (parent.substr.endIndex + 1);
				}
				ptr.substr.startIndex = (short) (words[prefixNode.substr.wordIndex]
						.substring(0, prefixNode.substr.endIndex).length() + 1);
				// prev Node
				if (wentToSib && (prev != parent)) {
					prev.sibling = prefixNode;
					wentToSib = false;
				} else {
					prev.firstChild = prefixNode;
				}
				if (ptr.sibling != null) {
					prefixNode.sibling = ptr.sibling;
				}
				ptr.sibling = temp;
				break;
			}
			// if (ptr.firstChild == null && !(prefix.length() == 0)) {
			// TrieNode temp = new TrieNode(new Indexes(index, (short) prefix.length(),
			// (short) (x.length() - 1)), null, null);
			// ptr.sibling = temp;
			// TrieNode preNode = new TrieNode(new Indexes(ptr.substr.wordIndex,
			// (short)prefix.substring(ptr.substr.startIndex).length(),(short)
			// prefix.length()),ptr,null);
			// prev.firstChild = preNode;
			// } else if(ptr.firstChild == null) {
			// TrieNode temp = new TrieNode(new Indexes(index, (short) prefix.length(),
			// (short) (x.length() - 1)), null, null);
			// root = temp;
			// }
			// prev = ptr;
		}
		return root;
	}

	/**
	 * Given a trie, returns the "completion list" for a prefix, i.e. all the leaf
	 * nodes in the trie whose words start with this prefix. For instance, if the
	 * trie had the words "bear", "bull", "stock", and "bell", the completion list
	 * for prefix "b" would be the leaf nodes that hold "bear", "bull", and "bell";
	 * for prefix "be", the completion would be the leaf nodes that hold "bear" and
	 * "bell", and for prefix "bell", completion would be the leaf node that holds
	 * "bell". (The last example shows that an input prefix can be an entire word.)
	 * The order of returned leaf nodes DOES NOT MATTER. So, for prefix "be", the
	 * returned list of leaf nodes can be either hold [bear,bell] or [bell,bear].
	 *
	 * @param root
	 *            Root of Trie that stores all words to search on for completion
	 *            lists
	 * @param allWords
	 *            Array of words that have been inserted into the trie
	 * @param prefix
	 *            Prefix to be completed with words in trie
	 * @return List of all leaf nodes in trie that hold words that start with the
	 *         prefix, order of leaf nodes does not matter. If there is no word in
	 *         the tree that has this prefix, null is returned.
	 */
	public static ArrayList<TrieNode> completionList(TrieNode root, String[] allWords, String prefix) {
		/** COMPLETE THIS METHOD **/
		System.out.println("Prefix is: " + prefix);
		ArrayList<TrieNode> matches = new ArrayList<>();
		TrieNode newRoot;
		if (prefix.equals("") || prefix == null) {
			newRoot = root;
		} else {
			newRoot = findRoot(root.firstChild, allWords, prefix);
		}
		//System.out.println("NewRoot: " + newRoot);
		if (newRoot == null) {
			return null;
		}
		if (newRoot.firstChild == null) {
			if(newRoot == root) {
				return null;
			}
			matches.add(newRoot);
		} else {
			matches = findMatches(newRoot.firstChild, allWords, matches);
		}
		System.out.println(matches.toString());
		// FOLLOWING LINE IS A PLACEHOLDER TO ENSURE COMPILATION
		// MODIFY IT AS NEEDED FOR YOUR IMPLEMENTATION
		return matches;
	}

	private static ArrayList<TrieNode> findMatches(TrieNode firstChild, String[] allWords, ArrayList<TrieNode> list) {

		if (firstChild.firstChild != null) {
			System.out.println("Going down to child: " + firstChild.firstChild);
			findMatches(firstChild.firstChild, allWords, list);
		} else {
			System.out.println("Adding child to match: " + firstChild);
			list.add(firstChild);
		}
		if (firstChild.sibling != null) {
			System.out.println("Going to sibling: " + firstChild.sibling);
			findMatches(firstChild.sibling, allWords, list);
		}
		return list;
	}

	private static TrieNode findRoot(TrieNode root, String[] allWords, String prefix) {
		String rootWord = allWords[root.substr.wordIndex].substring(0, root.substr.endIndex + 1);
		System.out.println("Root Word: " + rootWord + " Prefix: " + prefix);
		if (rootWord.contains(prefix)) {
			System.out.println("Root Found: " + rootWord);
			return root;
		}
		if (root.firstChild != null) {// check if right child is parent of match
			TrieNode tmp = findRoot(root.firstChild, allWords, prefix);
			if (tmp != null)// match found -> complete path from this node
				return tmp;
		}
		if (root.sibling != null) {// check if left child is parent of match
			TrieNode tmp = findRoot(root.sibling, allWords, prefix);
			if (tmp != null)
				return tmp;
		}
		return null;
	}

	public static void print(TrieNode root, String[] allWords) {
		System.out.println("\nTRIE\n");
		print(root, 1, allWords);
	}

	private static void print(TrieNode root, int indent, String[] words) {
		if (root == null) {
			return;
		}
		for (int i = 0; i < indent - 1; i++) {
			System.out.print("    ");
		}

		if (root.substr != null) {
			String pre = words[root.substr.wordIndex].substring(0, root.substr.endIndex + 1);
			System.out.println("      " + pre);
		}

		for (int i = 0; i < indent - 1; i++) {
			System.out.print("    ");
		}
		System.out.print(" ---");
		if (root.substr == null) {
			System.out.println("root");
		} else {
			System.out.println(root.substr);
		}

		for (TrieNode ptr = root.firstChild; ptr != null; ptr = ptr.sibling) {
			for (int i = 0; i < indent - 1; i++) {
				System.out.print("    ");
			}
			System.out.println("     |");
			print(ptr, indent + 1, words);
		}
	}
}