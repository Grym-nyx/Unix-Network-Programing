/* Nguyen Thanh Nam - 18020925 */
/* Y tuong: Su dung trang thai
            Su dung JSON-Java de xu ly du lieu dang JSON 
            Bo qua error tren text editor, chuong trinh se van chay nhu binh thuong */
import org.json.*;
public class Protocol {
    private static final int LISTENING = 0;
    private static final int USER_INFO = 1;
        
    private int state = LISTENING;
    public String processInput (String theInput) {
        String theOuput = null;

        if (state == LISTENING) {
            if (theInput.equals("QUIT")) {
                theOuput = "500 bye";
            }
            else if (theInput.equals("HELO Server")) {
                theOuput = "200 Hello Client";
                state = USER_INFO;
            } 
            else {
                theOuput = "Invalid command, try HELO Server";
            }
        }

        else if (state == USER_INFO) {
            if (theInput.equals("QUIT")) {
                theOuput = "500 bye";
                state = LISTENING;
            }
            else {
                String jsonStr = theInput;
                String userName = "TOM";
                String userAge = "20";
                int valid = 1;
                JSONObject obj = new JSONObject(jsonStr);
                userName = obj.getString("Username");
                userAge = obj.getString("Userage");
    
                for (int i=0; i<userName.length(); i++) {
                    if(!Character.isLetter(userName.charAt(i))) {
                        valid = 0;
                    } 
                }
    
                for (int i=0; i<userAge.length(); i++) {
                    if(!Character.isDigit(userAge.charAt(i))) {
                        valid = 0;
                    }
                }
    
                if (valid == 1) {
                    theOuput = "211 User Info OK";
                } else {
                    theOuput = "400 User Info Error";
                }
            }
        }
        return theOuput;
    }
}
