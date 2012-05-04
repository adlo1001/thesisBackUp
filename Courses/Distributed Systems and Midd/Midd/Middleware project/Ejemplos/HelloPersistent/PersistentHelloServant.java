// PersistentHelloServant.java
// Copyright and License 
import org.omg.CORBA.ORB;

public class PersistentHelloServant extends Persistent.HelloPOA {
    private ORB orb;
  
    public PersistentHelloServant( ORB orb ) {
        this.orb = orb;
    }

    /**
     *  sayHello() method implementation returns a simple message.
     */
    public String sayHello( ) {
        return "Hello From Persistent Server...";
    }

    /**
     *  shutdown() method shuts down the Persistent Server.
     *  See NOTE below.
     */
    public void shutdown( ) {
        orb.shutdown( false );
    }
}