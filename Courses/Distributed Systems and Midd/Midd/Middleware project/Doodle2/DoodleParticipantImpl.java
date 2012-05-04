import _DoodlePackage.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

public class DoodleParticipantImpl extends DoodleParticipantPOA {
    
	private DoodleInformer doodleInformer;
	private ManageDoodle doodleManager;

	// ************************************************
	public DoodleParticipantImpl( String args[] ) throws Exception {
 
    	// init the ORB
    	ORB orb = ORB.init(args, null);
	
    	// get reference to rootpoa & activate the POAManager
    	POA rootpoa = 
    		POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
    	rootpoa.the_POAManager().activate();
	
    	// activate the servant associating it to the 
    	// rootpoa and getting the object reference
    	rootpoa.servant_to_reference( this );

    	// get the reference to the ChatServer
    	org.omg.CORBA.Object srvRef = orb.string_to_object(
    			"corbaname::"+"localhost"+":1050#DoodleInformer");
    	doodleInformer = DoodleInformerHelper.narrow( srvRef );
    	org.omg.CORBA.Object srvRef2 = orb.string_to_object(
    			"corbaname::"+"localhost"+":1050#DoodleManager");
    	doodleManager = ManageDoodleHelper.narrow( srvRef2 );
	}    
	// ************************************************
 
		// ------------------------------------------------------   
    	public void printMsg(String msg) {
    		System.out.println("MSG: "+msg);
    	}
    	// ------------------------------------------------------   
    				
    	// ------------------------------------------------------   
		public void startRunning() {

	    	// add myself to the ChatServer
			doodleInformer.addClient( _this() );
			String action; 
			String strVar; 
			Doodle doodle;
			
			// main loop
			while(true) {
			
				action = System.console().readLine();
				action = action.toLowerCase();
				int actionCode = 5;
				
				if( action.equals( "forward" ) ) actionCode = 0; 
				if( action.equals( "createdoodle" ) ) actionCode = 1;
				if( action.equals( "getdoodle" ) ) actionCode = 2;
				if( action.equals( "votedoodle" ) ) actionCode = 3;
				if( action.equals( "closedoodle" ) ) actionCode = 4;
	            	
		        switch ( actionCode ) {
	            case 0:
	            	System.out.print("Write message: ");
	            	strVar = System.console().readLine();
					doodleInformer.fwdMsg( strVar );
					break;                
	            case 1:
	            	System.out.println("%--- CREAR DOODLE ---%");
	            	System.out.print("Doodle name: ");
	            	strVar = System.console().readLine();            	
	            	doodle = doodleManager.createDoodle(strVar, this._this());
	            	if(doodle != null){
						doodleInformer.fwdMsg("There is a new Doodle: " + strVar);
					} else {						
						System.out.println("Doodle "+ strVar + " already exist!");		
						System.out.println("Choose an unique name");
					} 	            	
	                break;   
	            case 2:
	            	System.out.println("%--- GET DOODLE ---%");
	            	System.out.print("Doodle name: ");
	            	strVar = System.console().readLine();
	            	doodle = doodleManager.getDoodle(strVar);	            	
	            	if(doodle!=null) {						
						System.out.println("Doodle Schedule: " +doodle.details().scheduleDoodle);
					} else {
						System.out.println("Doodle "+ strVar + " does not exist!");
					}
	            	break;
					
	            case 3:
	            	System.out.println("%--- VOTE DOODLE ---%");
	            	System.out.print("Doodle name: ");
	            	strVar = System.console().readLine();
	            	doodle = doodleManager.getDoodle(strVar);
	            	if(doodle!=null) {						
						System.out.print("Your day/hour: ");
	                	strVar = System.console().readLine();	                	
						doodleManager.voteDoodle(doodle, strVar, this._this());						
	                	System.out.println( "Done-----" );
					} else{; 
	            		System.out.println("Doodle "+ strVar + " does not exist!");					
					}
	            	
	                break;
	            case 4:
	            	System.out.println("%--- CLOSE DOODLE ---%");
	            	System.out.print("Doodle name: ");
	            	strVar = System.console().readLine();
	            	doodle = doodleManager.getDoodle(strVar);
	            	if(doodle!=null){						
						System.out.print("Your day/hour: ");
	                	strVar = System.console().readLine();	                		                	
	                	doodle = doodleManager.closeDoodle(doodle, strVar, this._this() );
	                	if(doodle==null){
	                		System.out.println( "You are not the creator, Doodle NOT closed" );
	                	}else{
	                		System.out.println( "Done-----" );
	                	}
					} else {
						System.out.println("Doodle "+ strVar + " does not exist!");		
					}             		            	
	                break;   
	            default: 
	            	System.out.println("Command NOT valid");
	                break;
		        }
			}
		}
    	  
    // ------------------------------------------------------
    
    /* while whit exceptions
     
     // main loop
			while(true) {
				
				action = System.console().readLine();
				action = action.toLowerCase();
				int actionCode = 5;
				
				if( action.equals( "forward" ) ) actionCode = 0; 
				if( action.equals( "createdoodle" ) ) actionCode = 1;
				if( action.equals( "getdoodle" ) ) actionCode = 2;
				if( action.equals( "votedoodle" ) ) actionCode = 3;
				if( action.equals( "closedoodle" ) ) actionCode = 4;
	            	
		        switch ( actionCode ) {
	            case 0:
	            	System.out.print("Write message: ");
	            	strVar = System.console().readLine();
					doodleInformer.fwdMsg( strVar );
					break;                
	            case 1:
	            	System.out.println("%--- CREAR DOODLE ---%");
	            	System.out.print("Doodle name: ");
	            	strVar = System.console().readLine();            	
	            	try {
						doodle = doodleManager.createDoodle(strVar, this._this());
						doodleInformer.fwdMsg("There is a new Doodle: " + strVar);
					} catch (ManageDoodleException e1) {						
						System.out.print(e1.info);		
						//e1.printStackTrace();
					} 	            	
	                break;   
	            case 2:
	            	System.out.println("%--- GET DOODLE ---%");
	            	System.out.print("Doodle name: ");
	            	strVar = System.console().readLine();
	            	
	            	try {
						doodle = doodleManager.getDoodle(strVar);
						System.out.println("Doodle Schedule: " +doodle.details().scheduleDoodle);
					} catch ( ManageDoodleException  dnf) {
						System.out.println(dnf.info);
					}
	            	
	                break;   
	            case 3:
	            	System.out.println("%--- VOTE DOODLE ---%");
	            	System.out.print("Doodle name: ");
	            	strVar = System.console().readLine();
	            	try {
						doodle = doodleManager.getDoodle(strVar);
						System.out.print("Your day/hour: ");
	                	strVar = System.console().readLine();	                	
						doodleManager.voteDoodle(doodle, strVar, this._this());						
	                	System.out.println( "Done-----" );
					} catch (ManageDoodleException  dnf) {
	            		System.out.println(dnf.info);					
					}
	            	
	                break;
	            case 4:
	            	System.out.println("%--- CLOSE DOODLE ---%");
	            	System.out.print("Doodle name: ");
	            	strVar = System.console().readLine();
	            	try {
						doodle = doodleManager.getDoodle(strVar);
						System.out.print("Your day/hour: ");
	                	strVar = System.console().readLine();	                		                	
	                	doodle = doodleManager.closeDoodle(doodle, strVar, this._this() );
	                	System.out.println( "Done-----" );	                	
					} catch (ManageDoodleException dnf) {
						System.out.println(dnf.info);
					}             		            	
	                break;   
	            default: 
	            	System.out.println("Command NOT valid");
	                break;
		        }
			}
     * */
}