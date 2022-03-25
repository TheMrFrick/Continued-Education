package problem_Set_2;

public class Problem_1_2 {
	
	public static IntNode addFront(IntNode front, int data) {
		return new IntNode(data, front);
	}
	
	/**
	 * @param front
	 * @param target
	 * @param newItem
	 * @return
	 */
	public static IntNode addBefore(IntNode front, int target, int newItem) {
		if (front == null) {
			return front;
		}
		IntNode ptr = front;
		IntNode prev = null;
		if (ptr.data == target) {
			return new IntNode(newItem, front);
		} else {
			prev = ptr;
			ptr = ptr.next;
		}
		for (; ptr != null; ptr = ptr.next, prev = prev.next) {
			if (ptr.data == target) {
				IntNode temp = new IntNode(newItem, ptr);
				prev.next = temp;
				return front;
			}
		}
		return front;
	}

	public static IntNode addBeforeLast(IntNode front, int item) {
		if (front == null) {
			return front;
		}
		IntNode ptr = front;
		IntNode prev = null;
		if (ptr.next == null) {
			return new IntNode(item, front);
		} else {
			prev = ptr;
			ptr = ptr.next;
		}
		for (; ptr != null; ptr = ptr.next, prev = prev.next) {
			if (ptr.next == null) {
				IntNode temp = new IntNode(item, ptr);
				prev.next = temp;
				return front;
			}
		}
		return front;
	}

	public static void deleteEveryOther(IntNode front) {
		/* COMPLETE THIS METHOD */
		if(front == null) {
			return;
		}
		IntNode prev = front;
		int num = 0;
		for(IntNode temp = front; temp != null; temp = temp.next) {
			if(num % 2 == 1) {
				prev.next = temp.next;
				prev = prev.next;
			}
			num++;
		}
	}
	
	public static void traverse(IntNode front) {
		if (front == null) {
			System.out.println("Empty list");
			return;
		}
		System.out.print(front.data);  // first item
		IntNode ptr=front.next;    // prepare to loop starting with second item
		while (ptr != null) {
			System.out.print("->" + ptr.data);
			ptr = ptr.next;
		}
		System.out.println();
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		IntNode aim = null;
		aim = addFront(aim, 2);
		System.out.println("Add 2 to Front");
		traverse(aim);
		for(int i = 1; i < 19; i+=2) {
			aim = addFront(aim, i);
		}
		traverse(aim);
		aim = addBefore(aim, 2, 3);
		System.out.println("Add 3 before 2");
		traverse(aim);
		aim = addBeforeLast(aim, 4);
		System.out.println("Add 4 before last");
		traverse(aim);
		deleteEveryOther(aim);
		System.out.println("Delete every other");
		traverse(aim);
		
	}
}
