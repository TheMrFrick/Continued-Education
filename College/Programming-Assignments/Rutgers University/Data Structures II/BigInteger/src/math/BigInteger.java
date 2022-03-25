package math;

/**
 * This class encapsulates a BigInteger, i.e. a positive or negative integer
 * with any number of digits, which overcomes the computer storage length
 * limitation of an integer.
 * 
 */
public class BigInteger {

	/**
	 * True if this is a negative integer
	 */
	boolean negative;

	/**
	 * Number of digits in this integer
	 */
	int numDigits;

	/**
	 * Reference to the first node of this integer's linked list representation
	 * NOTE: The linked list stores the Least Significant Digit in the FIRST node.
	 * For instance, the integer 235 would be stored as: 5 --> 3 --> 2
	 */
	DigitNode front;

	/**
	 * Initializes this integer to a positive number with zero digits, in other
	 * words this is the 0 (zero) valued integer.
	 */
	public BigInteger() {
		negative = false;
		numDigits = 0;
		front = null;
	}

	private DigitNode addFront(DigitNode front, char digit) throws IllegalArgumentException {
		int dig = 0;
		switch (digit) {
		case '0':
			dig = 0;
			break;
		case '1':
			dig = 1;
			break;
		case '2':
			dig = 2;
			break;
		case '3':
			dig = 3;
			break;
		case '4':
			dig = 4;
			break;
		case '5':
			dig = 5;
			break;
		case '6':
			dig = 6;
			break;
		case '7':
			dig = 7;
			break;
		case '8':
			dig = 8;
			break;
		case '9':
			dig = 9;
			break;
		default:
			// System.out.println("Digit " + digit);
			throw new IllegalArgumentException();
		}
		return new DigitNode(dig, front);
	}

	/**
	 * Parses an input integer string into a corresponding BigInteger instance. A
	 * correctly formatted integer would have an optional sign as the first
	 * character (no sign means positive), and at least one digit character
	 * (including zero). Examples of correct format, with corresponding values
	 * Format Value +0 0 -0 0 +123 123 1023 1023 0012 12 0 0 -123 -123 -001 -1 +000
	 * 0
	 * 
	 * 
	 * @param integer
	 *            Integer string that is to be parsed
	 * @return BigInteger instance that stores the input integer
	 * @throws IllegalArgumentException
	 *             If input is incorrectly formatted
	 */
	public static BigInteger parse(String integer) throws IllegalArgumentException {
		BigInteger temp = new BigInteger();
		// checks to see if String is empty.
		if (integer.isEmpty()) {
			throw new IllegalArgumentException();
		}
		integer = integer.trim();
		// checks to see if String has anything but digits in it after point 0
		for (int i = 1; i < integer.length(); i++) {
			if (!Character.isDigit(integer.charAt(i))) {
				throw new IllegalArgumentException();
			}
		}
		// checks to see if there is a plus or minus sign
		if (integer.charAt(0) == ('+')) {
			integer = integer.substring(1);
		}
		if (integer.charAt(0) == ('-')) {
			integer = integer.substring(1);
			integer = reduceString(integer);
			if (integer.charAt(0) == ('0')) {
				temp.negative = false;
			} else {
				temp.negative = true;
			}
		}
		integer = reduceString(integer);
		// adds int digits to digitNode
		for (int i = 0; i < integer.length(); i++) {
			temp.front = temp.addFront(temp.front, integer.charAt(i));
			// System.out.println("Front: " + temp.front);
			temp.numDigits++;
		}
		// THE FOLLOWING LINE IS A PLACEHOLDER SO THE PROGRAM COMPILES
		// YOU WILL NEED TO CHANGE IT TO RETURN THE APPROPRIATE BigInteger
		// System.out.println("NumDigits: " + temp.numDigits);
		return temp;
	}

	private static DigitNode addEnd(DigitNode front, int digit) {
		//Adds to the end of the node
		if (front == null) {
			return new DigitNode(digit, null);
		}
		for (DigitNode curr = front; curr != null; curr = curr.next) {
			if (curr.next == null) {
				curr.next = new DigitNode(digit, null);
				return front;
			}
		}
		return front;
	}

	private static String reduceString(String integer) {
		//reducing string if there are preceding 0s
		while (integer.length() > 1) {
			// System.out.println("String length: " + integer.length() + "\tString: " +
			// integer);
			if (integer.startsWith("0")) {
				integer = integer.substring(1);
				// System.out.println("new String: " + integer);
			} else {
				break;
			}
		}
		// TODO Auto-generated method stub
		return integer;
	}

	/**
	 * Adds an integer to this integer, and returns the result in a NEW BigInteger
	 * object. DOES NOT MODIFY this integer. NOTE that either or both of the
	 * integers involved could be negative. (Which means this method can effectively
	 * subtract as well.)
	 * 
	 * @param other
	 *            Other integer to be added to this integer
	 * @return Result integer
	 */
	public BigInteger add(BigInteger other) {
		//creating nodes
		BigInteger temp = new BigInteger();
		DigitNode node_this = this.front;
		DigitNode node_other = other.front;
		int largerDigit;
		int numDigit = 1;
		if (this.numDigits >= other.numDigits) { //determining which has more digits
			largerDigit = this.numDigits;
		} else {
			largerDigit = other.numDigits;
		}
		int addon = 0;
		int sumDigs;
		if ((this.negative && other.negative) || (!this.negative && !other.negative)) { //adds based on the positive values
			while (largerDigit >= numDigit) {
				if (numDigit > this.numDigits) {
					sumDigs = node_other.digit + addon;
					node_other = node_other.next;
				} else if (numDigit > other.numDigits) {
					sumDigs = node_this.digit + addon;
					node_this = node_this.next;
				} else {
					sumDigs = node_this.digit + node_other.digit + addon;
					node_this = node_this.next;
					node_other = node_other.next;
				}
				if (addon == 1) {
					addon = 0;
				}
				if (sumDigs >= 10) {
					addon++;
					sumDigs -= 10;
				}
				temp.front = addEnd(temp.front, sumDigs);
				temp.numDigits++;
				if (addon == 1 && (numDigit + 1) > largerDigit) {
					temp.front = addEnd(temp.front, addon);
					temp.numDigits++;
				}
				numDigit++;
			}
			if (this.negative && other.negative) {
				temp.negative = true;
			}
		} else { // subtracts the numbers
			while (largerDigit >= numDigit) {
				if (numDigit > this.numDigits) {
					if(other.negative) {
						temp.negative = true;
					}
					sumDigs = node_other.digit - addon;
					node_other = node_other.next;
				} else if (numDigit > other.numDigits) {
					if(this.negative) {
						temp.negative = true;
					}
					sumDigs = node_this.digit - addon;
					node_this = node_this.next;
				} else {
					if(this.negative) {
						if(node_this.digit > node_other.digit) {
							temp.negative = true;
						} else  if (node_this.digit < node_other.digit){
							temp.negative = false;
						}
					}
					if(other.negative) {
						if(node_other.digit > node_this.digit) {
							temp.negative = true;
						} else if (node_other.digit < node_this.digit){
							temp.negative = false;
						}
					}
					sumDigs = node_this.digit - node_other.digit - addon;
					node_this = node_this.next;
					node_other = node_other.next;
				}
				if (addon == 1) {
					addon = 0;
				}
				if (sumDigs <= 0 && (numDigit + 1) > largerDigit) {
					sumDigs *= -1;
				} else if (sumDigs < 0) {
					addon = 1;
					sumDigs += 10;
				}
				
				temp.front = addEnd(temp.front, sumDigs);
				temp.numDigits++;
				numDigit++;
			}
		}
		//Re-fixing leading zeroes
		String tempDig= "";
		for(DigitNode curr = temp.front; curr != null; curr = curr.next) {
			tempDig = curr.digit + tempDig +"";
		}
		if(temp.negative) {
			tempDig = "-" + tempDig;
		}
		temp = parse(tempDig);
		return temp;
	}

	/**
	 * Returns the BigInteger obtained by multiplying the given BigInteger with this
	 * BigInteger - DOES NOT MODIFY this BigInteger
	 * 
	 * @param other
	 *            BigInteger to be multiplied
	 * @return A new BigInteger which is the product of this BigInteger and other.
	 */
	public BigInteger multiply(BigInteger other) {
		BigInteger temp = new BigInteger(); //creating a temp to return
		temp.front = temp.addFront(temp.front, '0');
		boolean bothNeg = false; 
		//determining negativity of multiplication
		if((this.negative && other.negative) || (!this.negative && !other.negative)) {
			this.negative = false;
			other.negative = false;
			bothNeg = true;
		}
		//determining which one is larger
		boolean otherlarge = false;
		if(this.numDigits < other.numDigits) {
			otherlarge = true;
		}
		//multiplies based on smaller value
		if(!otherlarge) {
			int numdig = 0;
			long num = 0;
			for(DigitNode curr = other.front; curr != null; curr = curr.next, numdig++) {
				num = curr.digit * (int) Math.pow(10, numdig);
				for(int i = 0; i < num; i++) {
					temp = temp.add(this);
				}
			}
			if(!bothNeg) {
				temp.negative =true;
			}
		} else {
			int numdig = 0;
			long num = 0;
			for(DigitNode curr = this.front; curr != null; curr = curr.next, numdig++) {
				num = curr.digit * (int) Math.pow(10, numdig);
				for(int i = 0; i < num; i++) {
					temp = temp.add(other);
				}
			}
			if(!bothNeg) {
				temp.negative =true;
			}
		}
		return temp;
		// THE FOLLOWING LINE IS A PLACEHOLDER SO THE PROGRAM COMPILES
		// YOU WILL NEED TO CHANGE IT TO RETURN THE APPROPRIATE BigInteger
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		if (front == null) {
			return "0";
		}

		String retval = front.digit + "";
		for (DigitNode curr = front.next; curr != null; curr = curr.next) {
			retval = curr.digit + retval;
		}

		if (negative) {
			retval = '-' + retval;
		}

		return retval;
	}

}
