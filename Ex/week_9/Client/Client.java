import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;

public class Client {
    public static void main (String[] args) throws IOException {
        if (args.length > 0 ) {
            System.err.println("Usage: Client needs no arguments!");
            System.exit(1);
        }    
    
        String hostName = "127.0.0.1";
        int portNumber = 4444;
        int buffSize = 10240;

        // bien de doc du lieu nhap tu ban phim 
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

        // doc vao host name, port number
        System.out.println("Enter host IP (127.0.0.1): ");
        hostName = stdIn.readLine();

        System.out.println("Enter port number (4444): ");
        portNumber = Integer.parseInt(stdIn.readLine());

        try (
            Socket cliSocket = new Socket(hostName, portNumber);
            DataInputStream in = new DataInputStream(cliSocket.getInputStream());
            DataOutputStream out = new DataOutputStream(cliSocket.getOutputStream());
        ) {
            // doc va gui byte toi server
            String fromUser;
            String fromServer = null;
            byte[] byteHeaderFromServer = new byte[4];
            byte[] byteFromServer;
            byte[] byteHeaderToSend = new byte[4];
            int dataSize, fromUserInputSize;

            while (true) {
                System.out.print("Client: ");
                // read user input
                if ((fromUser = stdIn.readLine()) != null) {

                    // convert header length to byte[]  
                    fromUserInputSize = fromUser.length();
                    byteHeaderToSend = convertIntToByteArray(fromUserInputSize);
                    // write header to server
                    out.write(byteHeaderToSend, 0, 4);

                    // write data to server, server already know the data size 
                    out.write(fromUser.getBytes());
                    //check if user type QUIT, then close socket
                    if (fromUser.equals("QUIT")) {
                        System.out.println("Closing connection...");
                        break;
                    }

                    // check if server sent msg
                    // read data received from server
                    // read first 4 bytes, it's header length
                    if ((in.read(byteHeaderFromServer, 0, 4)) == 4) {
                        dataSize = convertByteArrayToInt(byteHeaderFromServer);

                        // create file if data > buffer size
                        // we assume this situation, that is, will be a file 
                        if (dataSize > buffSize) {
                            FileOutputStream fos = new FileOutputStream("cpy");
                            int remainBytes = dataSize;
                            while (remainBytes > 0) {
                                if (remainBytes >= buffSize) {
                                    // if data is too big, compare to buffer size, set the byte will 
                                    // read to  the buffer size
                                    byteFromServer = new byte[buffSize];
                                    in.read(byteFromServer, 0, buffSize);
                                    fos.write(byteFromServer);
                                    remainBytes -= buffSize;
                                } else {
                                    // buffer size > remain bytes
                                    byteFromServer = new byte[remainBytes];
                                    in.read(byteFromServer, 0, remainBytes);
                                    fos.write(byteFromServer);
                                    remainBytes = 0;
                                }
                            }
                            // read download status from server
                            in.read(byteHeaderFromServer, 0, 4);
                            dataSize = convertByteArrayToInt(byteHeaderFromServer);
                            byteFromServer = new byte[dataSize];
                            in.read(byteFromServer, 0, dataSize);
                            fromServer = new String(byteFromServer);
                            System.out.println("Server: " + fromServer);

                            fos.close();
                        } else {
                            // normal msg
                            byteFromServer = new byte[dataSize];
                            in.read(byteFromServer, 0, dataSize);
                            fromServer = new String(byteFromServer);
                            System.out.println("Server: " + fromServer);
                        }
                        // the code above do: 
                        // read msg from server
                        // convert byte to String 
                        // we need to allocate byteFromServer here, to let it allocate with 
                        // exact byte it need to read, if not, if the last read byte larger
                        // what's going to read, we'll have the unneccessary byte 
                    }
                    
                }
            }

        } catch (Exception e) {
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
