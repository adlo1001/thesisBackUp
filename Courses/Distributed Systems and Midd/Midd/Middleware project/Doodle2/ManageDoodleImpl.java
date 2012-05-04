import _DoodlePackage.*;

import java.util.*;

import org.omg.PortableServer.POAPackage.ServantNotActive;
import org.omg.PortableServer.POAPackage.WrongPolicy;

public class ManageDoodleImpl extends ManageDoodlePOA {
	
	private List<Doodle> doodles;	
	private DoodleInformer doodleInformer;
	
	// ********************************************************
	public ManageDoodleImpl( DoodleInformer doodleInformer ) {
				
    	this.doodleInformer = doodleInformer;
    	this.doodles = new ArrayList<Doodle>();
		
	}
	
	// ********************************************************
	
	// --------------------------------------------------------
		public Doodle createDoodle(String nameDoodle, DoodleParticipant creatorDoodle) {
			
			Doodle doodle = this.getDoodle(nameDoodle);
			
			if(doodle == null){												

				DoodleImpl doodleImpl = new DoodleImpl(creatorDoodle, nameDoodle);
				
				// Get a reference for the new doodle
				org.omg.CORBA.Object ref = null;
				try {
					ref = _poa().servant_to_reference( doodleImpl );
				} catch (ServantNotActive e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (WrongPolicy e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				// Linking the ref with the doodle
				doodle = DoodleHelper.narrow( ref );      		
				doodles.add( doodle );	
			} else {
				doodle = null;
			}					
			return doodle;
		}
		// --------------------------------------------------------

		//--------------------------------------------------------
		public Doodle getDoodle(String nameDoodle) {
			
			boolean sw = false;
			int i=0;
			Doodle temp;
			while( !sw && i<doodles.size() ) {
				temp = doodles.get(i);
				if(!temp.details().nameDoodle.equals(nameDoodle)) {
					i++;
				} else {
					sw = true;
				}
			}
			
			if(sw) return doodles.get(i);
			else return null;
		}
		// --------------------------------------------------------
		
		//--------------------------------------------------------
		public void voteDoodle( Doodle doodle, String dayHour, DoodleParticipant participant) {
			
			int position = doodles.indexOf( doodle );
			if(position==-1){
				 System.out.println("Already deleted");
			}else{
				synchronized(this){
					doodle.addDayHour( dayHour );
					doodle.addParticipant( participant );
					doodles.set( position, doodle );
				}
			}
		}
		// --------------------------------------------------------
		
		// --------------------------------------------------------
		public Doodle closeDoodle( Doodle doodle, String finalDayHour, DoodleParticipant creatorDoodle ) {				
			
			DoodleParticipant creator = doodle.details().creatorDoodle;
					
			if (!creator.equals( creatorDoodle ) ) {
				doodle = null;				
			} else {
				int position = doodles.indexOf( doodle );
				doodleInformer.informParticipants( doodle, finalDayHour);
				doodles.remove( position );						
			}			
			return doodle;
		}
		// --------------------------------------------------------	

	/*
	
	// ********************************************************
	
	// --------------------------------------------------------
	@SuppressWarnings("finally")
	public Doodle createDoodle(String nameDoodle, DoodleParticipant creatorDoodle) throws ManageDoodleException {
		
		Doodle dood = null;
		try{
			this.getDoodle(nameDoodle);
			throw new ManageDoodleException("DoodleAlreadyExist");
		}catch( ManageDoodleException ok){					
			try {	
				DoodleImpl doodleImpl = new DoodleImpl(creatorDoodle, nameDoodle);
				// Get a reference for the new doodle
				org.omg.CORBA.Object ref = _poa().servant_to_reference( doodleImpl );
				// Linking the ref with the doodle
				Doodle doodle = DoodleHelper.narrow( ref );      		
				doodles.add( doodle );		
				dood = doodle;				
			} catch (ServantNotActive e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (WrongPolicy e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}			
		} finally { return dood; }
				
	}
	// --------------------------------------------------------

	//--------------------------------------------------------
	public Doodle getDoodle(String nameDoodle) throws ManageDoodleException {
		
		boolean sw = false;
		int i=0;
		Doodle temp;
		while( !sw && i<doodles.size() ) {
			temp = doodles.get(i);
			if(!temp.details().nameDoodle.equals(nameDoodle)) {
				i++;
			} else {
				sw = true;
			}
		}
		
		if(sw) return doodles.get(i);
		else throw new ManageDoodleException("DoodleNotFound");
	}
	// --------------------------------------------------------
	
	//--------------------------------------------------------
	public void voteDoodle( Doodle doodle, String dayHour, DoodleParticipant participant) 
			throws ManageDoodleException  {
		
		int position = doodles.indexOf( doodle );
		if(position==-1){
			 throw new ManageDoodleException("DoodleAlreadyClose");
		}else{
			synchronized(this){
				doodle.addDayHour( dayHour );
				doodle.addParticipant( participant );
				doodles.set( position, doodle );
			}
		}
	}
	// --------------------------------------------------------
	
	// --------------------------------------------------------
	public Doodle closeDoodle( Doodle doodle, 
			String finalDayHour, DoodleParticipant creatorDoodle ) throws ManageDoodleException {				
		
		DoodleParticipant creator = doodle.details().creatorDoodle;
				
		if (!creator.equals( creatorDoodle ) ) {
			doodle = null;
			throw new ManageDoodleException("DoodleNotCreator"); 
		} else {
			int position = doodles.indexOf( doodle );
			doodles.remove( position );
			doodleInformer.informParticipants( doodle, finalDayHour);			
		}			
		return doodle;
	}
	// --------------------------------------------------------
	 * 	
	 */
}
