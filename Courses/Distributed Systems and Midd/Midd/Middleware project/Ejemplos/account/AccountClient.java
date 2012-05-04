import Finance.*;
import org.omg.CORBA.*;
import org.omg.CosNaming.*;

import java.io.*;

public class AccountClient {
  public static void main(String args[]) {
    try{
      // create and initialize the ORB
      ORB orb = ORB.init(args, null);
      
      // get the root naming context
      org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
      
      // resolve the object Reference in naming
      String name = "Finance/RossiAccount";
      Account account = AccountHelper.narrow(ncRef.resolve_str(name));
      
      // System.out.println("Obtained a handle on server object: " + account);
      
      // invoke operations
      account.deposit(100);
      System.out.println("Current balance: "+account.details().balance);
      account.withdraw(20);
      System.out.println("Current balance: "+account.details().balance);
      
      // resolve the object reference through corbaloc url
      objRef = orb.string_to_object("corbaname::localhost:1050#Finance/RossiAccount");
      Account account1 = AccountHelper.narrow(objRef);
      account1.withdraw(50);
      System.out.println("Current balance: "+account1.details().balance);

      // resolve the object reference by reading it from file in "stringified" form
      BufferedReader in = new BufferedReader(new FileReader("RossiAccount.ref"));
      String objRefAsString = in.readLine();
      in.close();
      objRef = orb.string_to_object(objRefAsString);
      Account account2 = AccountHelper.narrow(objRef);
      account2.withdraw(5);
      System.out.println("Current balance: "+account2.details().balance);
    } catch (Exception e) { e.printStackTrace(); }
  }
}
