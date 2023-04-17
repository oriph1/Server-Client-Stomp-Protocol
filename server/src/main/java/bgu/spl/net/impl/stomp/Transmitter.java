package bgu.spl.net.impl.stomp;
import java.util.Map;
import java.util.Iterator;
public class Transmitter {
    public static String transmit(String c, Map h, String b) {
        String message = new String(c);
        message += ("\n");

        if (h != null) {
            for (Iterator keys = h.keySet().iterator(); keys.hasNext(); ) {
                String key = (String) keys.next();
                String value = (String) h.get(key);
                message += (key);
                message += (":");
                message += (value);
                message += ("\n");
            }
        }
        // message += ("\n");

        if (b != null) message += (b);
         message += ("\n");
       // message += ("\000");
        return message;
    }
}
