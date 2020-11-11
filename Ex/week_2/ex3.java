import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class ex3 {
	public static void main(String[] args) throws IOException {
		FileReader input = null;
		FileWriter output = null;
		Scanner in = new Scanner(System.in);

		try {
			System.out.println("Enter file name to read: ");
			String fileToRead = in.nextLine();

			System.out.println("Create file to write to: ");
			String fileToWrite = in.nextLine();

			input = new FileReader(fileToRead);
			output = new FileWriter(fileToWrite);

			int c;
			while ((c = input.read()) != -1) {
				output.write(c);
			}
		} finally {
			if (input != null) {
				input.close();
			}
			if (output != null) {
				output.close();
			}
		}
	}
}
