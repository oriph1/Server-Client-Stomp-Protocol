package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImp;
import java.lang.String;
import java.util.HashMap;

public class StompMessagingProtocolimp implements StompMessagingProtocol<String> {
    int connectionID;
    ConnectionsImp connections;

    @Override
    public void start(int connectionId, ConnectionsImp<String> connections) {
        this.connectionID = connectionId;
        this.connections = connections;
    }

    @Override
    public String process(String message) {
        System.out.println(message);
        String[] lines = message.split("\n");
        String command = lines[0];
        if (command.length() > 0) {
            // Get headers
            HashMap headers = new HashMap();
            String header;
            int i = 1;
            while (i < lines.length && (header = lines[i]).length() > 0) {
                int ind = header.indexOf(':');
                String k = header.substring(0, ind);
                String v = header.substring(ind + 1, header.length());
                headers.put(k.trim(), v.trim());
                i++;
            }
            // Read body
            String body = new String();
            String b;
            i++;
            while (i < lines.length && !(b = lines[i]).equals("\u0000")) {
                body += b + "\n";

                i++;
            }
            String ans = new String();
            switch (command) {
                case "CONNECT":
                    ans = connect(headers);
                    break;
                case "SEND":
                    ans = send(headers, body);
                    break;
                case "SUBSCRIBE":
                    ans = subScribe(headers);
                    break;
                case "UNSUBSCRIBE":
                    ans = unSubscribe(headers);
                    break;
                case "DISCONNECT":
                    ans = disconnect(headers);
                    break;
            }
            System.out.println("sending to client");
            System.out.println(ans);
            connections.send(connectionID, ans);
        }
        return "";
    }

    @Override
    public boolean shouldTerminate() {
        return false;
    }

    public String connect(HashMap<String, String> headers) {
        String userName = headers.get("login");
        String password = headers.get("passcode");
        if (!connections.userExist(userName)) {
            connections.addNewUser(userName, password, connectionID);
            HashMap<String, String> map = new HashMap<String, String>();
            map.put("version", "1.2");
            return Transmitter.transmit("CONNECTED", map, "");
        }
        if (connections.wrongPassord(userName, password))
            return Transmitter.transmit("ERROR", new HashMap<>(), "Wrong password");
        if (connections.isUserLogedIn(userName))
            return Transmitter.transmit("ERROR", new HashMap<>(), "User already logged in ");
        connections.connect(userName, connectionID);
        HashMap<String, String> map = new HashMap<String, String>();
        map.put("version", "1.2");
        return Transmitter.transmit("CONNECTED", map, "");
    }

    public String send(HashMap<String, String> headers, String body) {
        String topic = headers.get("destination");
        if (!connections.isUserSubscribedToChannel(topic, connectionID)) {
            connections.disconnect(connectionID);
            return Transmitter.transmit("ERROR", new HashMap<>(), "not subscribed");
        }
        String msg = body; // from input
        String subId = (String) connections.getSubId(topic, connectionID);
        String messageID = ("" + connections.getMessageCounter());
        String reciept = headers.get("reciept");

        HashMap<String, String> map = new HashMap<>();
        map.put("subscription", subId);
        map.put("message-id", messageID);
        map.put("destination", topic);
        String msgFrame = Transmitter.transmit("MESSAGE", map, msg);

        connections.send(topic, connectionID, msgFrame);
        HashMap<String, String> m = new HashMap<String, String>();
        m.put("reciept-id", reciept);
        return Transmitter.transmit("RECIEPT", m, "");
    }

    public String subScribe(HashMap<String, String> headers) {
        String topic = headers.get("destination");
        String subId = headers.get("id");
        String reciept = headers.get("reciept");
        connections.subScribe(topic, subId, connectionID);
        HashMap<String, String> m = new HashMap<String, String>();
        m.put("reciept-id", reciept);
        return Transmitter.transmit("RECIEPT", m, "");
    }

    public String unSubscribe(HashMap<String, String> headers) {
        String id = headers.get("id");
        String reciept = headers.get("reciept");
        if (id.equals("-1")) {
            connections.disconnect(connectionID);
            return Transmitter.transmit("ERROR", new HashMap<>(), "not subscribed");
        }
        // if(!connections.isUserSubscribedToChannel(id,connectionID))
        //
        connections.unSubscribe(id, connectionID);
        HashMap<String, String> m = new HashMap<String, String>();
        m.put("reciept-id", reciept);
        return Transmitter.transmit("RECIEPT", m, "");
    }

    public String disconnect(HashMap<String, String> headers) {
        connections.disconnect(connectionID);
        String reciept = headers.get("reciept");
        HashMap<String, String> m = new HashMap<String, String>();
        m.put("reciept-id", reciept);
        return Transmitter.transmit("RECIEPT", m, "");
    }

}
