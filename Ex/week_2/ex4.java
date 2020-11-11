import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;

import java.io.InputStream;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.security.DigestInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

class ex4 {

    	private static final Charset UTF_8 = StandardCharsets.UTF_8;
    	private static final String OUTPUT_FORMAT = "%-20s:%s";

	private static byte[] checksum(String filePath) {

		MessageDigest md;
		try {
		    md = MessageDigest.getInstance("MD5");
		} catch (NoSuchAlgorithmException e) {
		    throw new IllegalArgumentException(e);
		}

		try (InputStream is = new FileInputStream(filePath);
		     DigestInputStream dis = new DigestInputStream(is, md)) {
		    while (dis.read() != -1) ; //empty loop to clear the data
		    md = dis.getMessageDigest();
		} catch (IOException e) {
		    throw new IllegalArgumentException(e);
		}
		return md.digest();

    	}

    	private static String bytesToHex(byte[] bytes) {
		StringBuilder sb = new StringBuilder();
		for (byte b : bytes) {
		    sb.append(String.format("%02x", b));
		}
		return sb.toString();
	}
	public static void main(String[] args) throws IOException {
		FileInputStream input = null;
		FileOutputStream output = null;
		Scanner in = new Scanner(System.in);

		try {
			System.out.println("Image name: ");
			String or_img = in.nextLine();

			System.out.println("New image name: ");
			String nw_img = in.nextLine();

			input = new FileInputStream(or_img);
			output = new FileOutputStream(nw_img);
			int c;

			while((c = input.read()) != -1) {
				output.write(c);
			}
			
			// checksum

			String checksum_ori = bytesToHex(checksum(or_img));
			String checksum_new = bytesToHex(checksum(nw_img)); 

			if (checksum_ori.equals(checksum_new)) {
				System.out.println("Copied successfully");
			} else {
				System.out.println("Failed");
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
