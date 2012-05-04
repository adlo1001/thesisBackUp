import MyBank.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;
import java.io.*;

public class BankServer {
  public static void main(String args[]) {
    try{
      // create the servant
      BankImpl bankImpl = new BankImpl();

      // create and initialize the ORB
      ORB orb = ORB.init(args, null);

      // get reference to rootpoa & activate the POAManager
      POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();

      // activate the servant associating it to the rootpoa and getting the object reference
      org.omg.CORBA.Object ref = rootpoa.servant_to_reference(bankImpl);
         
      //--------------------------------------------------
      // OJO this was not here!!!!      
      // save the object reference to file in "stringified" form
      PrintWriter out = new PrintWriter("RossiAccount.ref");
      out.println(orb.object_to_string(ref));
      out.close();
      //--------------------------------------------------      
      
      // get the root naming context
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
      
      // bind the object reference in naming
      NameComponent path[] = ncRef.to_name("MyBank");
      ncRef.rebind(path, ref);

      // wait for invocations from clients
      System.out.println("MyBank server ready and waiting ...");
      orb.run();
    } catch(Exception e) { e.printStackTrace(); }
  }
}
