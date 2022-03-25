
public class AgePicker {
	public static void main(String[] args) {
		int sum = 0;
		for(int i = 0; i < 19890; i++) {
			int roll = (int) ((Math.random() * 20) + 1);
			if(roll % 2 == 0) {
				sum -= roll;
				if(sum < 0) {
					sum = 0;
				}
			} else {
				sum += roll;
			}
		}
		System.out.println(sum);
	}
}
