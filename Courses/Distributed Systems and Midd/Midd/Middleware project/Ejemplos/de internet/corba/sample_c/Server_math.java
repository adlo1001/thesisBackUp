package corba.sample_c;
//
import My_Math.*; // The package containing stubs
import org.omg.CosNaming.*; // Server_math will use the naming service
import org.omg.CosNaming.NamingContextPackage.*;
// The package containing special exceptions thrown by the name service
import org.omg.CORBA.*; // All CORBA applications need these classes
import java.io.*; // The package containing the class BufferedReader
import org.omg.PortableServer.*; // Package POA
import org.omg.PortableServer.POA; // Package POA

//
//
// Usage: java -cp [path] corba.sample_c.Server_math -ORBInitialPort [Port]
// [path] is the path that contains the "My_Math" directory and "corba.sample_c"
//      Ex:  [path]\My_Math
//           [path]\corba\sample_c  
// [Port] is the same port number of "orbd -ORBInitialPort [Port]"
// Remarks: before start the Server_math is need start the orbd application in [java directory]\bin
//          orbd -ORBInitialPort [port] 
//
public class Server_math {
	//
	public Server_math() {
		super();
	}
	//
	public static void main(String[] args) {
		//
		String name_service = "Math_server";
		//

		//
		try {
			System.out.println("Server>starting server...");
			System.out.println("Server>creating and initializing the ORB");
			ORB orb = ORB.init(args, null);
			//      
			System.out.println("Server>getting reference to rootpoa");
			POA rootpoa =
				POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			System.out.println("Server>activating the POA Manager");
			rootpoa.the_POAManager().activate();
			//
			System.out.println("Server>Creating the servant");
			Math_service math_service = new Math_service();
			//
			System.out.println("Server>obtain the reference from the servant");
			org.omg.CORBA.Object ref =
				rootpoa.servant_to_reference(math_service);
			Interface_Math service_href = Interface_MathHelper.narrow(ref);

			//
			System.out.println("Server>getting the root naming context");
			org.omg.CORBA.Object objRef =
				orb.resolve_initial_references("NameService");

			//  
			System.out.println(
				"Server>using NamingContextExt to provides interoperability");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
			//      
			System.out.println(
				"Server>binding the object reference in naming with name "
					+ name_service);
			NameComponent nc = new NameComponent(name_service, "");
			NameComponent path[] = { nc };
			ncRef.rebind(path, service_href);
			//
			System.out.println("Server>Server was started...");
			//
			BufferedReader data_reader =
				new BufferedReader(new InputStreamReader(System.in));
			String msg_kbd = " ";
			while (!msg_kbd.equals("quit") && !msg_kbd.equals("QUIT")) {
				System.out.println(
					"Server>enter with quit command to shutdown the server...");
				msg_kbd = data_reader.readLine();
				if (msg_kbd.equals("quit") || msg_kbd.equals("QUIT")) {
					System.out.println(
						"Server>shutdown of server is in progress...");
					ncRef.unbind(path);
					orb.shutdown(false);
					System.out.println("Server>shutdown was successful...");
				}
			}
			//
		} catch (Exception e) {
			System.out.println("Exception in server...\nException:\n" + e);

		}
		System.exit(0);

	}
}
