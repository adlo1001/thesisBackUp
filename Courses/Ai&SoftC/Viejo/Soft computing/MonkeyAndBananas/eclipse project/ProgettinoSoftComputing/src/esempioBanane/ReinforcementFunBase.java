package esempioBanane;

import java.util.Vector;

import classiAstratte.ReinforcementFunInterface;
import classiAstratte.StatoInterface;

public class ReinforcementFunBase implements ReinforcementFunInterface {

	private Vector ostacoliI;
	private Vector ostacoliJ;

	private Vector goalI;
	private Vector goalJ;


	public ReinforcementFunBase(){
		ostacoliI=new Vector();
		ostacoliJ=new Vector();
		goalI=new Vector();
		goalJ=new Vector();
		
		//aggiungo lo stato non valido come ostacolo
		ostacoliI.add(new Integer(-1));
		ostacoliJ.add(new Integer(-1));
	}
	
	public void addOstacolo(int i,int j){
		ostacoliI.add(new Integer(i));
		ostacoliJ.add(new Integer(j));		
	}

	public void addGoal(int i,int j){
		goalI.add(new Integer(i));
		goalJ.add(new Integer(j));		
	}

	
	public double getRinforzo(StatoInterface s) {
		Integer ii=new Integer(((Stato)s).getRow());
		Integer jj=new Integer(((Stato)s).getCol());
		for(int k=0;k<goalI.size();k++){
			if( (ii.equals(goalI.elementAt(k))) &&(jj.equals(goalJ.elementAt(k)) ))return 1000;
		}
		
		for(int k=0;k<ostacoliI.size();k++){
			if( (ii.equals(ostacoliI.elementAt(k))) &&(jj.equals(ostacoliJ.elementAt(k)) ))return -1000;
		}
		return 0;
	}

}
