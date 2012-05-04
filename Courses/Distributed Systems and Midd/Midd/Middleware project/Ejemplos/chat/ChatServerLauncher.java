import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;

import chat.*;

public class ChatServerLauncher {
    public static void main(String args[]) throws Exception {
	// create the servant
	ChatServerImpl cs = new ChatServerImpl();
	
	// create and initialize the ORB
	String[] info = {"-ORBInitialPort","1050",
			 "-ORBInitialHost","localhost"};
	ORB orb = ORB.init(info, null);
	
	// get reference to rootpoa & activate the POAManager
	POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
	rootpoa.the_POAManager().activate();
	
	// activate the servant associating it to the rootpoa and getting the object reference
	org.omg.CORBA.Object ref = rootpoa.servant_to_reference(cs);
	
	// get the root naming context
	org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
	NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
	
	// bind the object reference in naming
	NameComponent path1[] = ncRef.to_name("Chat");
	ncRef.bind_new_context(path1);
	NameComponent path2[] = ncRef.to_name("Chat/MainServer");
	ncRef.rebind(path2, ref);
	
	// wait for invocations from clients
	System.out.println("ChatServer ready and waiting ...");
	orb.run();
    }
}


