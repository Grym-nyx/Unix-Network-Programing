/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: doc du lieu nhap tu nguoi dung vao buffer, gui no toi cho Server */
import java.io.*;
import java.net.*;

public class Client {
    public static void main (String[] args) throws IOException {
        if (args.length != 2) {
            System.err.println("Usage: java Client <host name> <port number>");
            System.exit(1);
        }

        String hostName = args[0];
        int portNumber = Integer.parseInt(args[1]);

        try (
            Socket cliSocket = new Socket(hostName, portNumber);
            PrintWriter out = new PrintWriter(cliSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(cliSocket.getInputStream()));
        ) {
            BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
            String fromServer;
            String fromUser;

            while (true) {
                System.out.print("Client: ");
                fromUser = stdIn.readLine();
                if (fromUser != null) {
                    out.println(fromUser);
                    if (fromUser.equals("QUIT")) {
                        System.out.println("Server: 500 bye");
                        break;
                    }
                    if ((fromServer = in.readLine()) != null) {
                        System.out.println("Server: " + fromServer);
                    }
                }
            }

        } catch (UnknownHostException e) {
            System.err.println("Don't know about host " + hostName);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to " + hostName);
            System.exit(1);
        }
    }
}
