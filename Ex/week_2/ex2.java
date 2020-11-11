import java.io.File;
import java.io.FileWriter;
import java.util.Scanner;
import java.io.IOException;

class ex2 {
	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		
		System.out.println("Enter file name: ");
		String fileName = input.nextLine(); 
		File file = new File(fileName);
		
		// if file is created successful
		try {

			if (file.createNewFile()) {
				System.out.println("Enter the content: ");
				String content = input.nextLine();

				// write to file
				FileWriter  myWriter = new FileWriter(fileName);
				myWriter.write(content);
				myWriter.close();

				// file size
				long fileSize = file.length();
			       	System.out.format("File size: %d Bytes\n",  fileSize);
			}
			else {
				System.out.println("File already exist.");
			}
		} catch (IOException e) {
			System.out.println("An error occurred");
			e.printStackTrace();
		} finally {
			input.close();
		}
		
		System.out.println("Successful");
	}
}
