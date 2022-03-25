package apps;

import java.io.*;
import java.util.*;
import java.util.regex.*;

import structures.Stack;

public class Expression {

	/**
	 * Expression to be evaluated
	 */
	String expr;

	/**
	 * Scalar symbols in the expression
	 */
	ArrayList<ScalarSymbol> scalars;

	/**
	 * Array symbols in the expression
	 */
	ArrayList<ArraySymbol> arrays;

	/**
	 * String containing all delimiters (characters other than variables and
	 * constants), to be used with StringTokenizer
	 */
	public static final String delims = " \t|\\*|\\+|\\-|\\/|\\(|\\)|\\[|\\]";

	/**
	 * Initializes this Expression object with an input expression. Sets all other
	 * fields to null.
	 * 
	 * @param expr
	 *            Expression
	 */
	public Expression(String expr) {
		this.expr = expr;
	}

	/**
	 * Populates the scalars and arrays lists with symbols for scalar and array
	 * variables in the expression. For every variable, a SINGLE symbol is created
	 * and stored, even if it appears more than once in the expression. At this
	 * time, values for all variables are set to zero - they will be loaded from a
	 * file in the loadSymbolValues method.
	 */
	public void buildSymbols() {
		/** COMPLETE THIS METHOD **/
		scalars = new ArrayList<>();
		arrays = new ArrayList<>();
		String symDelim = "\t|\\*|\\+|\\-|\\/|\\d|\\s|\\]|\\(|\\)";
		String[] tokens = expr.replace("[", "[ ").split(symDelim);
		// System.out.println(expr);
		System.out.println(tokens.length);
		for (int i = 0; i < tokens.length; i++) {
			boolean dup = false;
			System.out.println(tokens[i]);
			if (tokens[i].contains("[")) {

				// find duplicates
				for (ArraySymbol arr : arrays) {
					if (arr.name.equals(tokens[i].substring(0, tokens[i].length() - 1))) {
						dup = true;
					}
				}
				System.out.println("Token: " + tokens[i] + "\tlength: " + tokens[i].length());
				System.out.println("Name: " + tokens[i].substring(0, tokens[i].length() - 1));
				if (!dup) {
					arrays.add(new ArraySymbol(tokens[i].substring(0, (tokens[i].length() - 1))));
				}
			} else if (tokens[i].length() > 0) {
				for (ScalarSymbol s : scalars) {
					if (s.name.equals((tokens[i]))) {
						dup = true;
					}
				}
				System.out.println("Scalar: " + tokens[i]);
				if (!dup) {
					scalars.add(new ScalarSymbol(tokens[i]));
				}
			}
		}
	}

	/**
	 * Loads values for symbols in the expression
	 * 
	 * @param sc
	 *            Scanner for values input
	 * @throws IOException
	 *             If there is a problem with the input
	 */
	public void loadSymbolValues(Scanner sc) throws IOException {
		while (sc.hasNextLine()) {
			StringTokenizer st = new StringTokenizer(sc.nextLine().trim());
			int numTokens = st.countTokens();
			String sym = st.nextToken();
			ScalarSymbol ssymbol = new ScalarSymbol(sym);
			ArraySymbol asymbol = new ArraySymbol(sym);
			int ssi = scalars.indexOf(ssymbol);
			int asi = arrays.indexOf(asymbol);
			if (ssi == -1 && asi == -1) {
				continue;
			}
			int num = Integer.parseInt(st.nextToken());
			if (numTokens == 2) { // scalar symbol
				scalars.get(ssi).value = num;
			} else { // array symbol
				asymbol = arrays.get(asi);
				asymbol.values = new int[num];
				// following are (index,val) pairs
				while (st.hasMoreTokens()) {
					String tok = st.nextToken();
					StringTokenizer stt = new StringTokenizer(tok, " (,)");
					int index = Integer.parseInt(stt.nextToken());
					int val = Integer.parseInt(stt.nextToken());
					asymbol.values[index] = val;
				}
			}
		}
	}

	/**
	 * Evaluates the expression, using RECURSION to evaluate subexpressions and to
	 * evaluate array subscript expressions.
	 * 
	 * @return Result of evaluation
	 */
	public float evaluate() {
		return evaluate(expr.trim().replace(" ", ""));
	}

	private float evaluate(String expr) {
		float sum = 0;
		int thisPref = 0;
		Stack<Float> value = new Stack<>();
		Stack<Character> operator = new Stack<>();
		for (int i = 0; i < expr.length(); i++) {
			if (Character.isAlphabetic(expr.charAt(i))) {
				// System.out.println("Is Alphabetic");
				// printScalars();
				// determine if array or not
				boolean isArr = false;
				for (int d = i; d < expr.length(); d++) {
					if (expr.charAt(d) == '[') {
						isArr = true;
						break;
					} else if (expr.charAt(d) == '+' || expr.charAt(d) == '-' || expr.charAt(d) == '*'
							|| expr.charAt(d) == '/') {
						break;
					}
				}
				if ((i + 1) < expr.length() && isArr
						&& (Character.isAlphabetic(expr.charAt(i + 1)) || expr.charAt(i + 1) == '[')) {
					StringBuilder arrayName = new StringBuilder();
					arrayName.append(expr.charAt(i));
					for (int j = i + 1; j < expr.length(); j++) {
						if (Character.isAlphabetic(expr.charAt(j))) {
							arrayName.append(expr.charAt(j));
						} else if (expr.charAt(j) == '[') {
							i = j; // setting i = '['

							break;
						}
					}
					int endSubExpression = expr.indexOf(']');
					String subExpression = expr.substring(i + 1, endSubExpression);
					String restExpression = expr.substring(i+1, expr.length());
					// determine how many ']' there are in subexpression
					if (restExpression.contains("[")) {
							System.out.println("Contains [");
							int countBrackets = 0;
							int counter = 0;
							
							for(int r = 0; r < restExpression.length(); r++) {
								if(restExpression.charAt(r) == '[') {
									countBrackets++;
									counter++;
								} 
								if(restExpression.charAt(r) == ']' && countBrackets == 0) {
									break;
								} else if(restExpression.charAt(r) == ']') {
									countBrackets-= 1;
								}
							}
							System.out.println("Count Brackets: " + countBrackets);
							System.out.println("Expr: " + expr);
							for(int r = 0; r < counter; r++) {
								endSubExpression = expr.indexOf(']', endSubExpression+1);
							}
//							System.out.println("EndSubExp: " + endSubExpression);
//							subExpression += "" + expr.substring(endSubExpression, expr.indexOf(']'));
//							System.out.println("Addition:" + expr.substring(endSubExpression, expr.lastIndexOf(']')));
//							endSubExpression = subExpression.lastIndexOf("]");
//							endSubExpression = expr.indexOf(']', expr.length() - i);
//							endSubExpression -= 1;
							subExpression = expr.substring(i+1, endSubExpression);
					}
					System.out.println("SubExpression: " + subExpression);
					System.out.println("EndSubExpression: " + endSubExpression);
					int subExpressionValue = (int) evaluate(subExpression);
					System.out.println(arrayName);
					printArrays();
					for (ArraySymbol arr : arrays) {
						if (arrayName.toString().trim().equals(arr.name)) {
							System.out.println("Before change: " + expr);
							expr = expr.substring(0, i - (arrayName.length()))
									+ Integer.toString(arr.values[subExpressionValue])
									+ expr.substring(endSubExpression +1, expr.length());
							System.out.println("After Change: " + expr);
							i -= arrayName.length();
						}
					}
				} else {
					StringBuilder scalarName = new StringBuilder();
					scalarName.append(expr.charAt(i));
					if ((i + 1) < expr.length() && Character.isAlphabetic(expr.charAt(i + 1))) {
						for (int d = i + 1; d < expr.length(); d++) {
							if (Character.isAlphabetic(expr.charAt(d))) {
								scalarName.append(expr.charAt(d));
							} else {
								break;
							}
						}
					}
					for (ScalarSymbol x : scalars) {
						if (x.name.equals(scalarName.toString().trim())) {
							if (scalarName.length() > 1) {
								System.out.println("Expression: " + expr);
								expr = expr.substring(0, i) + Integer.toString(x.value)
										+ expr.substring(
												i + scalarName.length() + Integer.toString(x.value).length() - 1,
												expr.length());
								System.out.println("Expression After resizing 1: " + expr);
							} else {
								System.out.println("Expression Before: " + expr);
								expr = expr.replace(Character.toString(expr.charAt(i)), Integer.toString(x.value));
								System.out.println("Expression After: " + expr);
							}
						}
					}
				}
			}
			if (Character.isDigit(expr.charAt(i))) {
				System.out.println(expr.charAt(i));
				int digits = 1;
				for (int j = i + 1; j < expr.length(); j++) {
					if (Character.isDigit(expr.charAt(j))) {
						digits++;
					} else {
						break;
					}
				}
				if (digits > 1) {
					value.push(Float.parseFloat(expr.substring(i, i + (digits))));
					i += digits - 1;
					// System.out.println(value.peek());
				} else {
					// System.out.println(expr.substring(i, i+1));
					value.push(Float.parseFloat(expr.substring(i, i + 1)));
				}
			} else if (!Character.isDigit(expr.charAt(i))) {
				thisPref = determinePref(expr.charAt(i));
				System.out.println(expr.charAt(i));
				if (expr.charAt(i) == '(') {
					int endSubExpression = expr.indexOf(')');
					String subExpression = expr.substring(i + 1, endSubExpression);
					String restExpression = expr.substring(i+1, expr.length());
					// determine how many ']' there are in subexpression
					if (restExpression.contains("(")) {
						System.out.println("Contains (");
						int countBrackets = 0;
						int counter = 0;
						
						for(int r = 0; r < restExpression.length(); r++) {
							if(restExpression.charAt(r) == '(') {
								countBrackets++;
								counter++;
							} 
							if(restExpression.charAt(r) == ')' && countBrackets == 0) {
								break;
							} else if(restExpression.charAt(r) == ')') {
								countBrackets-= 1;
							}
						}
						System.out.println("Count Brackets: " + countBrackets);
						System.out.println("Expr: " + expr);
						for(int r = 0; r < counter; r++) {
							endSubExpression = expr.indexOf(')', endSubExpression+1);
						}
						subExpression = expr.substring(i+1, endSubExpression);
					}
					System.out.println("SubExpression: " + subExpression);
					System.out.println("EndSubExpression: " + endSubExpression);
					float subExpressionValue = evaluate(subExpression);
					value.push(subExpressionValue);
					System.out.println("Before change: " + expr);
					if (i == 0) {
						expr = Integer.toString((int) subExpressionValue)
								+ expr.substring(endSubExpression + 1, expr.length());
					} else {
						expr = expr.substring(0, i) + Integer.toString((int) subExpressionValue)
								+ expr.substring(endSubExpression + 1, expr.length());
					}
					System.out.println(Integer.toString((int) subExpressionValue).length() + "\t" + i);
					i += Integer.toString((int) subExpressionValue).length() - 1;
					System.out.println("After i: " + i);
					System.out.println("After Change: " + expr);
					continue;
					// i -= 1;
					// while (operator.peek() != '(') {
					// sum = comOper(operator.pop(), value.pop(), value.pop());
					// value.push(sum);
					// // System.out.println(sum);
					// }
					// operator.pop();
				}
				if (expr.charAt(i) == ']') {
					while (operator.peek() != '[') {
						sum = comOper(operator.pop(), value.pop(), value.pop());
						value.push(sum);
						// System.out.println(sum);
					}
					operator.pop();
				}
				if (expr.charAt(i) == '+' || expr.charAt(i) == '/' || expr.charAt(i) == '*' || expr.charAt(i) == '-') {
					int peekPref = -1;
					if (!operator.isEmpty()) {
						peekPref = determinePref(operator.peek());
					}
					while (!operator.isEmpty() && (peekPref >= thisPref)) {
						System.out.println("Operator Size: " + operator.size());
						System.out.println("Value Size: " + value.size());
						sum = comOper(operator.pop(), value.pop(), value.pop());
						value.push(sum);
						if (!operator.isEmpty()) {
							peekPref = determinePref(operator.peek());
						}
					}
				}
				operator.push(expr.charAt(i));
				if (operator.peek() == ')' || operator.peek() == ']') {
					operator.pop();
				}
			}
		}
		while (!operator.isEmpty()) {
			System.out.println("Operator Size: " + operator.size());
			System.out.println("Value Size: " + value.size());
			sum = comOper(operator.pop(), value.pop(), value.pop());
			value.push(sum);
		}
		return value.pop();
	}

	private int determinePref(char op) {
		if (op == '+' || op == '-') {
			return 1;
		}
		if (op == '*' || op == '/') {
			return 2;
		}
		return -1;
	}

	private float comOper(Character operator, Float right, Float left) {
		// TODO Auto-generated method stub
		if (operator == '+') {
			return left + right;
		}
		if (operator == '-') {
			return left - right;
		}
		if (operator == '*') {
			return left * right;
		}
		if (operator == '/') {
			return left / right;
		}
		return 0;
	}

	/**
	 * Utility method, prints the symbols in the scalars list
	 */
	public void printScalars() {
		for (ScalarSymbol ss : scalars) {
			System.out.println(ss);
		}
	}

	/**
	 * Utility method, prints the symbols in the arrays list
	 */
	public void printArrays() {
		for (ArraySymbol as : arrays) {
			System.out.println(as);
		}
	}

}
