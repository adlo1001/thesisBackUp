public class DoodleClient {
	
	public static void main( String args[] ) {
	
		try{
			//String[] info = {"-ORBInitialPort","1050","-ORBInitialHost","localhost"};			
			DoodleParticipantImpl client = new DoodleParticipantImpl( args);////info );
			client.startRunning();			
		} catch (Exception e) { 
			e.printStackTrace(); 
		}
  }
	
}
