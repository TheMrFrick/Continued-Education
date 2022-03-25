package problem_Set_2;

public class Recitation_Answers {
	//Question 1
	public static IntNode addBefore(IntNode front, int target, int newItem) {
		IntNode prev = null, ptr = front;
		while (ptr != null && ptr.data != target) {
			prev = ptr;
			ptr = ptr.next;
		}

		if (ptr == null) {
			System.out.println("Can't Find target");
			return front;
		}

		IntNode temp = new IntNode(newItem, ptr);
		if (prev == null) {
			return temp;
		}
		prev.next = temp;
		return front;
	}

	//Question 2
	public static IntNode addBeforeLast(IntNode front, int item) {
		IntNode prev = null, ptr = front;
		while (ptr.next != null) {
			prev = ptr;
			ptr = ptr.next;
		}
		IntNode temp = new IntNode (item, ptr);
		prev.next = temp;
		return front;
	}
	
	//Question 4
	public static void deleteEveryOther(IntNode front) {
		if(front == null) {
			return;
		}
		
		IntNode prev = front, ptr = front.next;
		
		boolean tbd = true;
		while(ptr != null) {
			if(tbd) {
				ptr = ptr.next;
				prev.next = ptr;
				tbd = false;
			} else {
				prev = ptr;
				ptr = ptr.next;
				tbd = true;
			}
		}
	}
}
