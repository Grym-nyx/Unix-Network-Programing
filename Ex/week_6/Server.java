/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: Tao mot doi tuong protocol, co nhiem vu xu ly output gui cho Client */
import java.net.*;
import java.io.*;

public class Server {
    public static void main (String[] args) throws IOException {

        if (args.length != 1) {
            System.err.println("Usage: java Server need only <port number>");
            System.exit(1);
        }

        int portNumber = Integer.parseInt(args[0]);

        try (
            ServerSocket serverSocket = new ServerSocket(portNumber);
            Socket clientSocket = serverSocket.accept();
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        ) {

            String inputLine, outputLine;

            Protocol pr = new Protocol();

            while ((inputLine = in.readLine()) != null) {
                System.out.println("Message received from client: " + inputLine);
                outputLine = pr.processInput(inputLine);
                out.println(outputLine);
                System.out.println("Message sent to client: " + outputLine);
                if (outputLine.equals("500 bye"))
                    break;
            }
        } catch (IOException e) {
            System.out.println("Exception caught when trying to listen on port " + portNumber + " or listening for a connection");
            System.out.println(e.getMessage());
        }

    }
}