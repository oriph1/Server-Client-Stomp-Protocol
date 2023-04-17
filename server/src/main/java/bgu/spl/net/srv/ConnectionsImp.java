package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.Pair;
import bgu.spl.net.impl.stomp.Triple;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImp<T> implements Connections<T> {

    private ConcurrentHashMap<Integer, Triple<ConnectionHandler<T>, Boolean, String>> clients;
    private ConcurrentHashMap<String, List<Integer>> channelSubs;//channel : clientsID
    private ConcurrentHashMap<Integer, List<String>> clientSubs;//clientID: subs
    private ConcurrentHashMap<Pair<Integer, String>, String> subscriptions;//<ClientID,subscriptionID> : channel

    private ConcurrentHashMap<String, Pair<String, Boolean>> users;// userName: <password,connected>
    int clientCounter;
    int messageCounter;

    public ConnectionsImp() {
        clients = new ConcurrentHashMap<>();
        channelSubs = new ConcurrentHashMap<>();
        clientSubs = new ConcurrentHashMap<>();
        subscriptions = new ConcurrentHashMap<>();
        users = new ConcurrentHashMap<>();
        clientCounter = 0;
        messageCounter=0;
    }

    @Override
    public boolean send(int connectionId, T msg) {
        ConnectionHandler ch = clients.get(connectionId).getLeft();
        ch.send(msg);
        return true;
    }

    @Override
    public void send(java.lang.String channel,int connectionId, T msg) {
        List<Integer> subs = channelSubs.get(channel);
        for (Integer i : subs) {
            if (i!=connectionId)
                this.send(i, msg);
        }
    }

    @Override
    public void disconnect(int connectionId) {
        String userName=clients.get(connectionId).getRight();
        users.get(userName).right=false;
        clients.get(connectionId).right=null;
        clients.get(connectionId).middle =false;
        List<String> subs=clientSubs.get(connectionId);
        for(String subId: subs){
            String channel=subscriptions.get(new Pair<>(connectionId,subId));
            subscriptions.remove(new Pair<>(connectionId,subId));
            channelSubs.get(channel).remove(subId);
        }
        subs.clear();
        clientSubs.remove(connectionId);
    }

    public void subScribe(String topic, String subId, int idClient) {
        if (channelSubs.containsKey(topic)) {
            channelSubs.get(topic).add(idClient);
        } else {
            ArrayList<Integer> newChanel = new ArrayList();
            newChanel.add(idClient);
            channelSubs.put(topic, newChanel);
        }
        clientSubs.get(idClient).add(subId);
        subscriptions.put(new Pair(idClient, subId), topic);
    }

    public void unSubscribe(String subID, int clientID) {
        clientSubs.get(clientID).remove(subID);
        String channel = subscriptions.get(new Pair(clientID, subID));
        channelSubs.get(channel).remove(clientID);
        subscriptions.remove(new Pair(clientID, subID));
    }

    public boolean userExist(String userName) {
        return users.containsKey(userName);
    }

    public void addNewUser(String userName, String password, int ClieintID) {
        users.put(userName, new Pair(password, true));
        clients.get(ClieintID).middle = true;
        clients.get(ClieintID).right = userName;
    }
    public void addClient(ConnectionHandler<T> ch){
        clients.put(clientCounter,new Triple<>(ch,false,null));
        clientSubs.put(clientCounter,new ArrayList<String>());
        clientCounter++;
    }
    public boolean wrongPassord(String userName,String password){
        return !password.equals(users.get(userName).getLeft());
    }
    public boolean isUserLogedIn(String userName){
        return users.get(userName).getRight();
    }
    public void connect(String userName,int clientId){
        users.get(userName).right=true;
        clients.get(clientId).right=userName;
        clients.get(clientId).middle=true;
    }
    public int getCounter(){
        return clientCounter;
    }

    public String getSubId(String channel, int clientID) {
        List<String> subsClient=clientSubs.get(clientID);
        for(String s: subsClient){
            if(subscriptions.get(new Pair<>(clientID,s)).equals(channel))
                return s;
        }
        return "";
    }
    public int getMessageCounter(){
        messageCounter++;
        return messageCounter;
    }

    public boolean isUserSubscribedToChannel(String topic, int connectionID){
        if(channelSubs.size()==0)
            return false;
        if(!channelSubs.containsKey(topic))
            return false;
        return channelSubs.get(topic).contains(connectionID);   
    }
}
