package esempioBanane;

import java.util.Vector;

import classiAstratte.StatoInterface;

public class Stato implements StatoInterface {

	private int i,j;
	/*---------------------------*/
	public static int numRow=10;//[4-12]
	public static int numCol=8;//[4-12]
	/*---------------------------*/
	
	public static int numAzioni=4;
	
	private static Vector ostacoliI;
	private static Vector ostacoliJ;

	private static Vector goalI;
	private static Vector goalJ;
	
	public Stato(int i,int j){
		this.i=i;
		this.j=j;

		
		if(ostacoliI==null){
			ostacoliI=new Vector();
			ostacoliJ=new Vector();
			ostacoliI.add(new Integer(-1));
			ostacoliJ.add(new Integer(-1));
		}
		if(goalI==null){
			goalI=new Vector();
			goalJ=new Vector();			
		}		
	}
	
	public static void addOstacolo(int i,int j){
		if(ostacoliI==null){
			ostacoliI=new Vector();
			ostacoliJ=new Vector();
			ostacoliI.add(new Integer(-1));
			ostacoliJ.add(new Integer(-1));
		}
		ostacoliI.add(new Integer(i));
		ostacoliJ.add(new Integer(j));		
	}

	public static void addGoal(int i,int j){
		if(goalI==null){
			goalI=new Vector();
			goalJ=new Vector();			
		}
		goalI.add(new Integer(i));
		goalJ.add(new Integer(j));		
	}
	
	public int getUniqueID() {
		return i*numCol+j;
	}

	public int getNumAzioni() {
		return numAzioni;
	}

	public StatoInterface doAction(int x) {
		//schema delle azioni:
		//  0
		//3    1
		//  2
		
		//azione nello stato non valido non porta da nessuna parte
		if( (i==-1)&&(j==-1)){
			return this;
		}
		
		Stato statoNonValido=new Stato(-1,-1);
		if(x==0){
			int ii=i-1;
			if(ii<0)return statoNonValido;
			else return new Stato(ii,j);
		}
		else if(x==1){
			int jj=j+1;
			if(jj>=numCol)return statoNonValido;
			else return new Stato(i,jj);
		}
		else if(x==2){
			int ii=i+1;
			if(ii>=numRow)return statoNonValido;
			else return new Stato(ii,j);
		}
		else if(x==3){
			int jj=j-1;
			if(jj<0)return statoNonValido;
			else return new Stato(i,jj);
		}
		else return statoNonValido;
	}

	public String toString(){
		return "("+i+","+j+")";
	}
	
	public int getRow(){ return i;}
	public int getCol(){ return j;}

	public boolean isStop() {
		//banane!
		Integer ii=new Integer(i);
		Integer jj=new Integer(j);
		for(int k=0;k<goalI.size();k++){
			if( (ii.equals(goalI.elementAt(k))) &&(jj.equals(goalJ.elementAt(k)) ))return true;
		}
		
		for(int k=0;k<ostacoliI.size();k++){
			if( (ii.equals(ostacoliI.elementAt(k))) &&(jj.equals(ostacoliJ.elementAt(k)) ))return true;
		}
		return false;
	}
	
	public static void reset(){
		ostacoliI=new Vector();
		ostacoliJ=new Vector();
		ostacoliI.add(new Integer(-1));
		ostacoliJ.add(new Integer(-1));
		goalI=new Vector();
		goalJ=new Vector();			
	}
	
	
	
}
