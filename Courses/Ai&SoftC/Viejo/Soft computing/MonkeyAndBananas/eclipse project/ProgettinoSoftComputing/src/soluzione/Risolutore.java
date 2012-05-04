package soluzione;

import java.util.Iterator;
import java.util.TreeMap;
import java.util.Vector;

import classiAstratte.ActionSelector;
import classiAstratte.ActionSelectorCreatorInterface;
import classiAstratte.ReinforcementFunInterface;
import classiAstratte.StatoInterface;
import esempioBanane.Stato;

public class Risolutore {
	
	private double g;
	private double alfa;
	private TreeMap stati;
	private TreeMap q;
	private ReinforcementFunInterface rf;
	private ActionSelectorCreatorInterface actionSelectorCreator;
	private ObserverRisolutore observer;
	
	public void setObserver(ObserverRisolutore observer){
		this.observer=observer;		
	}
	
	public Risolutore(ReinforcementFunInterface r,double g,double alfa,ActionSelectorCreatorInterface actionSelectorCreator,ObserverRisolutore observer){
		stati=new TreeMap();
		q=new TreeMap();
		this.rf=r;
		this.g=g;
		this.alfa=alfa;
		this.actionSelectorCreator=actionSelectorCreator;
		this.observer=observer;
	}
	
	public void addStato(StatoInterface s){
		stati.put(new Integer(s.getUniqueID()),s);
		Vector v=new Vector();
		for(int j=0;j<s.getNumAzioni();j++){
			v.add(new Double(0));
		}
		q.put(new Integer(s.getUniqueID()),v);
	}

	public void startQLearning(StatoInterface statoStart){
		int t=0;		
		double r=0;
		int a=0;

		StatoInterface st=statoStart;
		StatoInterface st2=null;
		Integer uniqueID=new Integer(st.getUniqueID());
		observer.statoPartenza(st,(Vector) q.get(uniqueID));
		do{
			if(t>0){
				double delta=r+g*maxQ(st2);
				double qOrig=((Double)(((Vector) q.get(uniqueID)).elementAt(a))).doubleValue();
				double qEnd=(1-alfa)*qOrig+alfa*delta;
				((Vector) q.get(uniqueID)).setElementAt(new Double(qEnd),a);
				observer.aggiornamentoQ(st,(Vector) q.get(uniqueID));				
				st=st2;
				uniqueID=new Integer(st.getUniqueID());
			}
			if(st.isStop())break;
			a=selezionaAzione((Vector) q.get(uniqueID));
			observer.azioneScelta(a);
			st2=st.doAction(a);
			observer.statoRaggiunto(st2,(Vector) q.get(new Integer(st2.getUniqueID())));
			r=rf.getRinforzo(st2);
			observer.rinforzoOttenuto(r);
			t++;
		}while(st.isStop()==false);
		observer.terminato();
	}
	
	private double maxQ(StatoInterface st2) {
		int action=0;
		double max=0;
		double val=0;
		int i=0;
		Integer ID=new Integer(st2.getUniqueID());
		max=((Double)((((Vector) q.get(ID)).elementAt(i)))).doubleValue();
		for(i=1;i<st2.getNumAzioni();i++){
			val=((Double)((((Vector) q.get(ID)).elementAt(i)))).doubleValue();
			if(val>max){
				max=val;
				action=i;
			}
		}
		return max;
	}

	private int migliorAzioneAppresa(StatoInterface st2) {
		int action=0;
		double max=0;
		double val=0;
		int i=0;
		Integer ID=new Integer(st2.getUniqueID());
		max=((Double)((((Vector) q.get(ID)).elementAt(i)))).doubleValue();
		for(i=1;i<st2.getNumAzioni();i++){
			val=((Double)((((Vector) q.get(ID)).elementAt(i)))).doubleValue();
			if(val>max){
				max=val;
				action=i;
			}
		}
		return action;
	}
	
	private int selezionaAzione(Vector vector) {
		ActionSelector a=actionSelectorCreator.create(vector);
		observer.soglieImpostate(a.getActions(),a.getSoglie());
		return a.select();
	}

	public String toString(){
		String s="";
		if(q==null)return "QLearning not started";
		Iterator i=q.keySet().iterator();
		while(i.hasNext()){
			Integer st=(Integer) i.next();
			
			s=s+"Stato: "+stati.get(st)+" reinf:"+ rf.getRinforzo((StatoInterface)stati.get(st))+"\n";
			s=s+"\tAzioni:\n";
			Vector v=(Vector)q.get(st);
			for(int j=0;j<v.size();j++){
				s=s+"\t"+j +": "+ ((Double)v.elementAt(j)) +"\n";				
			}
			s=s+"\n";
		}
		return s;
	}

	public void useWhatLearned(StatoInterface stato) {
		observer.statoPartenzaUse(stato);
		while(stato.isStop()==false){
			int a=migliorAzioneAppresa(stato);
			stato=stato.doAction(a);
			if(observer.moveToUse(stato)==false)break;
		}
		observer.terminatoUse();
		
	}
}
