import java.util.Properties;
import org.omg.CORBA.ORB;
import org.omg.CORBA.OBJ_ADAPTER;
import org.omg.CosNaming.NamingContext;
import org.omg.CosNaming.NamingContextHelper;
import org.omg.CosNaming.NameComponent;
import org.omg.PortableServer.POA;

import Persistent.HelloHelper;
import Persistent.Hello;

public class PersistentClient {

    public static void main(String args[]) {

        try {
            // Step 1: Instantiate the ORB
            ORB orb = ORB.init(args, null);

            // Step 2: Resolve the PersistentHelloServant by using INS's
            // corbaname url. The URL locates the NameService running on
            // localhost and listening on 1050 and resolve 
            // 'PersistentServerTutorial' from that NameService
            org.omg.CORBA.Object obj = orb.string_to_object( 
                "corbaname::localhost:1050#PersistentServerTutorial");
 
            Hello hello = HelloHelper.narrow( obj );

            // Step 3: Call the sayHello() method every 60 seconds and shutdown
            // the server. Next call from the client will restart the server,
            // because it is persistent in nature.
            while( true ) {
                System.out.println( "Calling Persistent Server.." );
                String helloFromServer = hello.sayHello();
                System.out.println("Message From Persistent Server: " +
                    helloFromServer );
                System.out.println( "Shutting down Persistent Server.." );
                hello.shutdown( );
                Thread.sleep( 60000 );
            }
        } catch ( Exception e ) {
            System.err.println( "Exception in PersistentClient.java..." + e );
            e.printStackTrace( );
        }
    }
}