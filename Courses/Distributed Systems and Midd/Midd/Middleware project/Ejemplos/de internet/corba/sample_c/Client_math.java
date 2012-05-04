package corba.sample_c;
import My_Math.*;
// The package containing stubs and other files generated from Math.idl
import org.omg.CosNaming.*; // Client_math will use the naming service
import org.omg.CORBA.*; // All CORBA applications need these classes
import java.io.*;
//
// Usage: java -cp [path] corba.sample_c.Client_math -ORBInitialPort [Port]
// [path] is the path that contains the "My_Math" directory and "corba.sample_c"
//      Ex:  [path]\My_Math
//           [path]\corba\sample_c  
// [Port] is the same port number of "tnameserv -ORBInitialPort [Port]"
//
public class Client_math {
	//
	public Client_math() {
		super();
	}
	//
	public static void main(String[] args) {
		//
		BufferedReader input_kbd =
			new BufferedReader(new InputStreamReader(System.in));
		String msg_keybd = " ";
		String name_service = "Math_server";
		System.out.println("Client>starting client...");
		//
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
			Interface_Math math_service =
				Interface_MathHelper.narrow(ncRef.resolve(path));
			//      
			String[] messages_know =
				{
					"help",
					"addition",
					"subtraction",
					"multiplication",
					"transpose" };
			// help =0   addition=1  subtraction=2 multiplication=3  transpost = 4       
			System.out.println(
				"Client>Enter with the command:\n help, addition, subtraction, multiplication, transpose and quit");

			while (!msg_keybd.equals("quit") && !msg_keybd.equals("QUIT")) {
				//

				System.out.print("Client>");

				msg_keybd = input_kbd.readLine();
				// 
				if (msg_keybd.equals("quit") || msg_keybd.equals("QUIT"))
					continue;
				//
				int index_message = -1;
				for (int i = 0; i < messages_know.length; i++) {
					if (msg_keybd.equals(messages_know[i]))
						index_message = i;
				}
				//

				switch (index_message) {
					case 0 : // help
						{
							String msg_received = math_service.get_help();
							System.out.println(
								"Client>server returned the following message:"
									+ msg_received);

						}
						break;
					case 1 : // addition
						{
							Result a = read_array("A");
							Result b = read_array("B");
							Result result =
								math_service.addition(
									new My_Math.My_Array(a.result),
									a.line_result,
									a.row_result,
									new My_Math.My_Array(b.result),
									b.line_result,
									b.row_result);
							System.out.println(
								"Client>server returned the following message:");
							print_array(result, "C");
						}
						break;
					case 2 : // subtraction
						{
							Result a = read_array("A");
							Result b = read_array("B");
							Result result =
								math_service.subtraction(
									new My_Math.My_Array(a.result),
									a.line_result,
									a.row_result,
									new My_Math.My_Array(b.result),
									b.line_result,
									b.row_result);
							System.out.println(
								"Client>server returned the following message:");
							print_array(result, "C");

						}
						break;
					case 3 : // multiplication
						{
							Result a = read_array("A");
							Result b = read_array("B");
							Result result =
								math_service.multiplication(
									new My_Math.My_Array(a.result),
									a.line_result,
									a.row_result,
									new My_Math.My_Array(b.result),
									b.line_result,
									b.row_result);
							System.out.println(
								"Client>server returned the following message:");
							print_array(result, "C");

						}
						break;
					case 4 : // transpose
						{
							Result a = read_array("A");
							Result result =
								math_service.transpose(
									new My_Math.My_Array(a.result),
									a.line_result,
									a.row_result);
							System.out.println(
								"Client>server returned the following message:");
							print_array(result, "C");
						}
						break;
					default : // default
						{
							System.out.println(
								"Client>unknow command:" + msg_keybd);
						}
						break;

				}
			}
			//
		} catch (Exception e) {
			System.out.println("Exception in client...\nException:\n" + e);
		}
	}
	//
	public static void print_array(Result result, String name)
		throws Exception {
		if (!result.msg.equals("null"))
			System.out.println("message returned:" + result.msg);
		if (result.e.equals("null")) {
			try {
				int line = result.line_result;
				int row = result.row_result;
				float[][] a = result.result;
				for (int i = 0; i < line; i++)
					for (int j = 0; j < row; j++) {
						System.out.println(
							name + "[" + i + "," + j + "]=" + a[i][j]);
					}

			} catch (Exception e) {
				throw new Exception("Exception in print_array:\n" + e);
			}
		} else {
			System.out.println("Error message:" + result.e);

		}

		return;
	}
	//
	public static Result read_array(String name) throws Exception {
		BufferedReader input_kbd =
			new BufferedReader(new InputStreamReader(System.in));
		Result array = null;
		int line = 0, row = 0;
		try {

			System.out.println("Enter the number line of array " + name);
			line = Integer.parseInt(input_kbd.readLine());
			System.out.println("Enter the number row of array " + name);
			row = Integer.parseInt(input_kbd.readLine());
			float[][] a =
				new float[My_Math.SIZE_MAX.value][My_Math.SIZE_MAX.value];
			// all array in CORBA have a fixed dimension
			for (int i = 0; i < line; i++)
				for (int j = 0; j < row; j++) {
					System.out.print(name + "[" + i + "," + j + "]=");
					a[i][j] = Float.parseFloat(input_kbd.readLine());
				}
			array = new Result(a, line, row, "null", "null");
			// "null" is passed as string because null not exist in CORBA

		} catch (Exception e) {

			throw new Exception("Exception in read_array:\n" + e);
		}

		return array;
	}
}
