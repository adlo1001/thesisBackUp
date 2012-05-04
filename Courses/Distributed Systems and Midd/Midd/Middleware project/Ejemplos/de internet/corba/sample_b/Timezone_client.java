package corba.sample_b;
//
import java.util.*;
import java.io.*;
import TimeZone.*; // The package containing stubs
import org.omg.CosNaming.*; // Client_hello will use the naming service
import org.omg.CORBA.*; // All CORBA applications need these classes
//
//
// Usage: java -cp [path] corba.sample_b.Timezone_client -ORBInitialPort [Port]
// [path] is the path that contains the "TimeZone" directory and "corba.sample_b"
//        Ex:   [path]\TimeZone
//              [path]\corba\sample_b  
// [Port] is the same port number of "orbd -ORBInitialPort [Port]"
//
public class Timezone_client {
	//
	public Timezone_client() {
		super();
	}
	//
	public static void main(String[] args) {
		BufferedReader input_kbd =
			new BufferedReader(new InputStreamReader(System.in));
		String msg_keybd = " ";
		String name_service = "Timezone_server";

		System.out.println("Client>starting client...");
		try {
			System.out.println("Client>Creating and initializing the ORB");
			ORB orb = ORB.init(args, null);
			//      
			System.out.println("Client>getting the root naming context");
			org.omg.CORBA.Object objRef =
				orb.resolve_initial_references("NameService");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
			//      
			System.out.println(
				"Client>Resolving the object reference in naming ["
					+ name_service
					+ "]");
			NameComponent nc = new NameComponent(name_service, "");
			NameComponent path[] = { nc };
			Interface_TimeZone service =
				Interface_TimeZoneHelper.narrow(ncRef.resolve(path));
			//      
			String[] messages_know =
				{ "help", "get_ids", "get_time", "get_date", "set_timezone" };
			//
			System.out.println(
				"Client>Enter with the command:\n help, get_ids, get_time, get_date, set_timezone and quit");

			while (!msg_keybd.equals("quit") && !msg_keybd.equals("QUIT")) {
				//

				System.out.print("Client>");

				msg_keybd = input_kbd.readLine();
				//
				StringTokenizer tk = new StringTokenizer(msg_keybd, " ");
				//
				String first_token = tk.nextToken();
				// 
				if (first_token.equals("quit") || first_token.equals("QUIT"))
					continue;
				//
				int index_message = -1;
				for (int i = 0; i < messages_know.length; i++) {
					if (first_token.equals(messages_know[i]))
						index_message = i;
				}
				//

				switch (index_message) {
					case 0 : // help
						{
							String msg_received = service.get_help();
							System.out.println(
								"Client>server returned the following message:"
									+ msg_received);

						}
						break;
					case 1 : // get_ids
						{
							String msg_received = service.get_ids();
							System.out.println(
								"Client>server returned the following message:"
									+ msg_received);
						}
						break;
					case 2 : // get_time
						{
							String msg_received = service.get_time();
							System.out.println(
								"Client>server returned the following message:"
									+ msg_received);

						}
						break;
					case 3 : //get_date
						{
							String msg_received = service.get_date();
							System.out.println(
								"Client>server returned the following message:"
									+ msg_received);
						}
						break;
					case 4 : //  set_timezone
						{
							if (tk.countTokens() >= 1) {
								String second_token = tk.nextToken();
								String msg_received =
									service.set_timezone(second_token);
								System.out.println(
									"Client>server returned the following message:"
										+ msg_received);
							} else {
								System.out.println(
									"Client>sintax error:\n sintax: set_timezone <TimeZone>");
							}

						}
						break;
					default : // default
						{
							System.out.println("Client>unknow command");
						}
						break;

				}
			}
			//
		} catch (Exception e) {
			System.out.println("Exception in client...\nException:\n" + e);
		}
	}
}
