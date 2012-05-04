import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;

public class DoodleServer {
  
	public static void main(String args[]) {
    
		try{		
			// create and initialize the ORB
			//String[] info = {"-ORBInitialPort","1050","-ORBInitialHost","localhost"};
			ORB orb = ORB.init(args, null);//info, null);

			// get reference to rootpoa & activate the POAManager
			POA rootpoa = 
				POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			rootpoa.the_POAManager().activate();
			
			//doodleManager.setORB( orb ); // ojo and register it with the ORB
			// create the servant
			DoodleInformerImpl doodleInformer = new DoodleInformerImpl();
			
			//------------------------------------------------------
		    // get object reference from the servant (related with Addressing)
			// activate the servant associating it to the rootpoa and getting the object reference
			org.omg.CORBA.Object ref2 = rootpoa.servant_to_reference( doodleInformer );
			//------------------------------------------------------			          
			// get the root naming context
			org.omg.CORBA.Object objRef =
				orb.resolve_initial_references("NameService");
			// Use NamingContextExt which is part of the Interoperable
		    // Naming Service (INS) specification.
			NamingContextExt ncRef = 
				NamingContextExtHelper.narrow( objRef );
			//------------------------------------------------------      
			// bind the object reference in naming
			NameComponent path2[] = ncRef.to_name( "DoodleInformer" );
			ncRef.rebind(path2, ref2); 
			//------------------------------------------------------
			
			ManageDoodleImpl doodleManager = new ManageDoodleImpl( doodleInformer._this() );
			org.omg.CORBA.Object ref1 = rootpoa.servant_to_reference( doodleManager );			
			NameComponent path1[] = ncRef.to_name( "DoodleManager" );
			ncRef.rebind(path1, ref1); 
			
			// wait for invocations from clients
			System.out.println("Doodle System server ready and waiting ...");
			orb.run();
			
		} catch(Exception e) { e.printStackTrace(); }
	}
}
