import chat.*;
import java.util.*;

public class ChatServerImpl extends ChatServerPOA {
    private List<ChatClient> clients;

    public ChatServerImpl() {
	clients = new ArrayList<ChatClient>();
    }
    public synchronized void addClient (chat.ChatClient client) {
	clients.add(client);
    }

    public void fwdMsg (String msg) {
	List<ChatClient> clientsCopy; 
	synchronized(this) {
	   clientsCopy = new ArrayList<ChatClient>(clients);
	}
	for(ChatClient c:clientsCopy) {
	    c.printMsg(msg);
	}
    }
}

