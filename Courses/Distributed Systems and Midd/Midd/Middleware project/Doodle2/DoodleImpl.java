import java.util.ArrayList;
import java.util.List;
import _DoodlePackage.*;

public class DoodleImpl extends DoodlePOA {
  
	private DoodleDetails details;	
	private List<DoodleParticipant> participants;
	
	// ********************************************************
	public DoodleImpl(DoodleParticipant creatorDoodle, String nameDoodle) {
		
		this.participants = new ArrayList<DoodleParticipant>();				
		details = new DoodleDetails(creatorDoodle, nameDoodle, " $ ");					
	}
	// ********************************************************			
	
	//--------------------------------------------------------
	public void addDayHour(String vote) {
		details.scheduleDoodle = details.scheduleDoodle + vote + " $ ";		
	}
	//--------------------------------------------------------
	
	//--------------------------------------------------------
	public DoodleDetails details() {
		return details;
	}	
	//--------------------------------------------------------

	//--------------------------------------------------------
    public boolean containsParticipant( DoodleParticipant participant ) {
    	return participants.contains( participant );
    }
	//--------------------------------------------------------

	//--------------------------------------------------------
	public void addParticipant( DoodleParticipant participant ) {
		if( !containsParticipant( participant ) )			
			participants.add( participant );		
	}
	//--------------------------------------------------------
	
	public synchronized DoodleParticipant[] getParticipants() {
		
		Object[] arr = this.participants.toArray();		
		DoodleParticipant[] parts = new DoodleParticipant[arr.length];
		
		for(int i=0; i<arr.length; i++){
			parts[0] = (DoodleParticipant) arr[0];
		}
				
		return parts;				
	}	
	
}
