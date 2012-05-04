import _DoodlePackage.*;

import java.util.*;

public class DoodleInformerImpl extends DoodleInformerPOA {
    
	private List<DoodleParticipant> clients;

	// ******************************************
    public DoodleInformerImpl() {
    	clients = new ArrayList<DoodleParticipant>();
    }    
    // ******************************************
    
    // ---------------------------------------------
    public synchronized void informParticipants(Doodle doodle, String finalDayHour) {

    	DoodleParticipant[] part = doodle.getParticipants();     	
    	List<DoodleParticipant> participants = Arrays.asList( part );    	        	    	

    	for( int i=0; i<participants.size(); i++ ) {    		
    		participants.get(i).printMsg( "Doodle "+ doodle.details().nameDoodle+" is closed." );
    		participants.get(i).printMsg( "Final date: "+finalDayHour );
        }        	
    }
    // --------------------------------------------

    // ---------------------------------------------
    public synchronized void addClient( DoodleParticipant client ) {
    	clients.add( client );
    }
    // ---------------------------------------------

    // ---------------------------------------------
    public void fwdMsg(String msg) {
    	
    	List<DoodleParticipant> clientsCopy; 
    	
    	synchronized(this) {
    		clientsCopy = new ArrayList<DoodleParticipant>( clients );
    	}
    	for( DoodleParticipant c:clientsCopy) {
    		c.printMsg(msg);
    	}
    }
    // ---------------------------------------------
     
}