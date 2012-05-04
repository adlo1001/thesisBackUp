import MyBank.*;
import org.omg.CORBA.*;
import org.omg.CosNaming.*;

public class BankClient {
  public static void main(String args[]) {
    try{
      // create and initialize the ORB
      ORB orb = ORB.init(args, null);
      
      // get the root naming context
      org.omg.CORBA.Object objRef = 
        orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = 
        NamingContextExtHelper.narrow(objRef);
      
      // resolve the object Reference in naming
      String name = "MyBank";
      Bank bank = BankHelper.narrow(ncRef.resolve_str(name));
      
      System.out.println("Obtained a handle on server object: "
                         + bank);
      
      // invoke operations
      Account acc1 = bank.createNewAccount("Rossi", "Milano");
      System.out.println("Obtained a handle on server object: "
                         + acc1);
      acc1.deposit(20);
      Account acc2 = bank.createNewAccount("Verdi", "Roma");
      acc2.deposit(100);
      System.out.println("acc1: "+acc1.details().name+" "+
                         acc1.details().number+" "+
                         acc1.details().balance);
      System.out.println("acc1: "+acc2.details().name+" "+
                         acc2.details().number+" "+
                         acc2.details().balance);
      
      // resolve the object reference through corbaloc url
      // objRef = orb.string_to_object("corbaname::localhost:5000#MyBank");
      // Bank bank1 = BankHelper.narrow(objRef);
    } catch (Exception e) { e.printStackTrace(); }
  }
}
