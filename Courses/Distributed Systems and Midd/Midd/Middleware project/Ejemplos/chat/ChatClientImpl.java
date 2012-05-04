import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;

import chat.*;

public class ChatClientImpl extends ChatClientPOA {
    private ChatServer server;

    public ChatClientImpl(String args[]) throws Exception {
	// attach this to the POA
	// init the ORB
	ORB orb = ORB.init(args, null);
	
	// get reference to rootpoa & activate the POAManager
	POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
	rootpoa.the_POAManager().activate();
	
	// activate the servant associating it to the rootpoa and getting the object reference
	rootpoa.servant_to_reference(this);

	// get the reference to the ChatServer
	org.omg.CORBA.Object srvRef = orb.string_to_object("corbaname::"+args[0]+":1050#Chat/MainServer");
	server = ChatServerHelper.narrow(srvRef);
    }

    public void doJob() {
	// add myself to the ChatServer
	server.addClient(_this());
	
	// main loop
	while(true) {
	    String msg = System.console().readLine();
	    server.fwdMsg(msg);
	}
    }
    
    public void printMsg(String msg) {
	System.out.println("MSG: "+msg);
    }

    public static void main(String args[]) throws Exception {
	ChatClientImpl client = new ChatClientImpl(args);
	client.doJob();
    }
}


