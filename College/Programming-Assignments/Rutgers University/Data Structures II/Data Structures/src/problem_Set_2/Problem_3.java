package problem_Set_2;

public class Problem_3 {
	 public static int numberOfOccurrences(StringNode front, String target) {
         /* COMPLETE THIS METHOD */
		 int count = 0;
		 for(StringNode temp = front; temp != null; temp = temp.next) {
			 if(temp.data.equals(target)) {
				 count++;
			 }
		 }
		 return count;
	 } 
}
