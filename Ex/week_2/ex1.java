import java.util.Scanner;

class ex1 {
	public static void main (String[] args) {
		int i;
		int [] count = { 0, 0, 0 };
		Scanner input = new Scanner(System.in);

		System.out.println("Enter the string: ");

		String in = input.nextLine();

		for (i = 0; i < in.length(); i++) {
			if (Character.isLetter(in.charAt(i))) {
				count[0] += 1;
			} else if (Character.isDigit(in.charAt(i))) {
				count[1] += 1;
			} else {
				count[2] += 1;
			}
		}

		System.out.println("The letter: " + count[0]);
		System.out.println("The number: " + count[1]);
		System.out.println("The symbol: " + count[2]);
	}
}
