import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

class MyRunnable implements Runnable {
    private Socket clientSocket;

    public MyRunnable(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    public void run() {
        final int buffSize = 10240;
        final int WAIT_GREET = 0;
        final int WAIT_NAME = 1;
        final int WAIT_GET = 2;
        int state = 0;

        try (
                // create server socket which will listen client socket connection
                // create client socket which will connect to server
                DataInputStream in = new DataInputStream(clientSocket.getInputStream());
                DataOutputStream out = new DataOutputStream(clientSocket.getOutputStream());) {
            String fromServer = null;
            String fromUser;
            byte[] byteHeaderFromClient = new byte[4];
            byte[] byteFromClient;
            int dataSize, fromServerResponseSize;
            FileInputStream file = null;

            byte[] byteHeaderToSend;
            while (in.read(byteHeaderFromClient, 0, 4) == 4) {
                // if server read 4 byte header length successfully
                // server continues to read data

                dataSize = convertByteArrayToInt(byteHeaderFromClient);

                // read data
                // allocate variable byteFromClient to let it receive
                // the exact byte (value of dataSize)
                // because when convert to String, we need to compare to
                // if the size is not exact, the comparison result in false anyway
                byteFromClient = new byte[dataSize];
                in.read(byteFromClient, 0, dataSize);
                fromUser = new String(byteFromClient, StandardCharsets.UTF_8);

                // state to clarify what need to do
                // make response
                if (state == WAIT_GREET) {
                    if (fromUser.equals("QUIT")) {
                        fromServer = "Bye";
                        break;
                    } else if (fromUser.equals("HELLO")) {
                        fromServer = "Connection accepted. Enter file name:";
                        state = WAIT_NAME;
                    } else {
                        fromServer = "Invalid command, try 'HELLO'";
                    }
                } else if (state == WAIT_NAME) {
                    if (fromUser.equals("QUIT")) {
                        fromServer = "Bye";
                        break;
                    } else {
                        // read file
                        try {
                            file = new FileInputStream(fromUser);
                            // get file size
                            int fileSize = file.available();
                            // print server side file info
                            System.out.println(String.format("file name: %s | file size: %fkb", fromUser,
                                    (double) fileSize / 1024));
                            // set response to client
                            fromServer = String.format("file name: %s | file size: %fkb", fromUser,
                                    (double) fileSize / 1024);
                            state = WAIT_GET;
                        } catch (FileNotFoundException e) {
                            fromServer = "File not found, try again";
                        }
                    }
                } else if (state == WAIT_GET) {
                    if (fromUser.equals("QUIT")) {
                        fromServer = "Bye";
                        break;
                    }
                    if (fromUser.equals("DOWNLOAD")) {
                        // write file size to socket
                        // note: file size need to be bigger than client buffer size (5120B)
                        // smaller will let client read as normal msg
                        int fileSize = file.available();
                        fromServerResponseSize = fileSize;
                        byteHeaderToSend = convertIntToByteArray(fromServerResponseSize);
                        out.write(byteHeaderToSend, 0, 4);

                        // send file data
                        int remainBytes = fileSize;
                        while (remainBytes > 0) {
                            if (remainBytes >= buffSize) {
                                byte[] byteFile = new byte[buffSize];
                                file.read(byteFile, 0, buffSize); // read to byteFile
                                out.write(byteFile);
                                remainBytes -= buffSize;
                            } else {
                                byte[] byteFile = new byte[remainBytes];
                                file.read(byteFile, 0, remainBytes);
                                out.write(byteFile);
                                remainBytes = 0;
                            }
                        }
                        fromServer = "file downloaded!";
                        state = WAIT_NAME;
                    } else {
                        fromServer = "Invalid command, only support 'DOWNLOAD'";
                    }
                }

                // write response to socket
                // write header first
                fromServerResponseSize = fromServer.length();
                byteHeaderToSend = convertIntToByteArray(fromServerResponseSize);
                out.write(byteHeaderToSend, 0, 4);

                // then write data
                out.write(fromServer.getBytes());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static int convertByteArrayToInt(byte[] bytes) {
        return ByteBuffer.wrap(bytes).getInt();
    }
    public static byte[] convertIntToByteArray(int value) {
        return  ByteBuffer.allocate(4).putInt(value).array();
    }
}
public class Server {
    
    public static void main (String[] args) throws IOException{
        if (args.length > 0) {
            System.err.println("Usage: Server need no arguments");
            System.exit(1);
        }
        
        int portNumber = 4444;
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));


        System.out.print("Enter port number (4444): ");
        portNumber = Integer.parseInt(stdIn.readLine());
        ServerSocket servSocket = new ServerSocket(portNumber);
        
        while (true) {
            Socket clientSocket = servSocket.accept();
            System.out.println("Received a connection from " + clientSocket.getInetAddress() + " on port " + clientSocket.getPort());
            MyRunnable mr = new MyRunnable(clientSocket);
            Thread tr = new Thread(mr);
            tr.start();
        }
    }
    
}