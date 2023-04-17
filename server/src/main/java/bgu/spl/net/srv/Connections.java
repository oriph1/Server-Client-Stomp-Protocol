package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel,int connectionID, T msg);

    void disconnect(int connectionId);
}
