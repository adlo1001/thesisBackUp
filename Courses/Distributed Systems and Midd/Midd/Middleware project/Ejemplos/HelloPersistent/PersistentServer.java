// PersistentServer.java
// Copyright and License 
import java.util.Properties;
import org.omg.CORBA.Object;
import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.CORBA.Policy;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.*;
import org.omg.PortableServer.Servant;

public class PersistentServer {

    public static void main( String args[] ) {
        Properties properties = System.getProperties();
        properties.put( "org.omg.CORBA.ORBInitialHost",
            "localhost" );
        properties.put( "org.omg.CORBA.ORBInitialPort",
            "1050" );
    
        try {
            // Step 1: Instantiate the ORB
            ORB orb = ORB.init(args, properties);

            // Step 2: Instantiate the servant
            PersistentHelloServant servant = new PersistentHelloServant(orb); 

            // Step 3 : Create a POA with Persistent Policy
            // *******************  
            // Step 3-1: Get the rootPOA 
            POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
            // Step 3-2: Create the Persistent Policy
            Policy[] persistentPolicy = new Policy[1];
            persistentPolicy[0] = rootPOA.create_lifespan_policy(
                LifespanPolicyValue.PERSISTENT);
            // Step 3-3: Create a POA by passing the Persistent Policy
            POA persistentPOA = rootPOA.create_POA("childPOA", null, 
                persistentPolicy ); 
            // Step 3-4: Activate PersistentPOA's POAManager, Without this
            // All calls to Persistent Server will hang because POAManager
            // will be in the 'HOLD' state.
            persistentPOA.the_POAManager().activate( );
            // ***********************
 
            // Step 4: Associate the servant with PersistentPOA
            persistentPOA.activate_object( servant );

            // Step 5: Resolve RootNaming context and bind a name for the
            // servant.
            // NOTE: If the Server is persistent in nature then using Persistent
            // Name Service is a good choice. Even if ORBD is restarted the Name
            // Bindings will be intact. To use Persistent Name Service use
            // 'NameService' as the key for resolve_initial_references() when
            // ORBD is running.
            org.omg.CORBA.Object obj = orb.resolve_initial_references(
                "NameService" );
            NamingContextExt rootContext = NamingContextExtHelper.narrow( obj );

            NameComponent[] nc = rootContext.to_name( 
                "PersistentServerTutorial" );
            rootContext.rebind( nc, persistentPOA.servant_to_reference( 
                servant ) );

            // Step 6: We are ready to receive client requests
            orb.run();
        } catch ( Exception e ) {
            System.err.println( "Exception in Persistent Server Startup " + e );
        }
    }
}