package soluzione;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.TreeSet;
import java.util.Vector;

import classiAstratte.StatoInterface;

public class MultipleObserver implements ObserverRisolutore {

	private ArrayList t;
	
	public MultipleObserver(){
		t=new  ArrayList();
	}
	
	public void addObserver(ObserverRisolutore o){
		t.add(o);
	}

	public void removeObserver(ObserverRisolutore o){
		t.remove(o);
	}
	
	public void statoPartenza(StatoInterface st, Vector q) {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).statoPartenza(st,q);
		}
	}

	public void azioneScelta(int a) {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).azioneScelta(a);
		}
	}

	public void statoRaggiunto(StatoInterface st2, Vector q) {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).statoRaggiunto(st2,q);
		}
	}

	public void rinforzoOttenuto(double r) {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).rinforzoOttenuto(r);
		}

	}

	public void soglieImpostate(int[] actions, double[] soglie) {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).soglieImpostate(actions,soglie);
		}

	}

	public void aggiornamentoQ(StatoInterface st, Vector v) {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).aggiornamentoQ(st,v);
		}

	}

	public void statoPartenzaUse(StatoInterface stato) {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).statoPartenzaUse(stato);
		}		
	}

	public boolean moveToUse(StatoInterface stato) {
		boolean Continue=true;
		Iterator i=t.iterator();
		while(i.hasNext()){
			Continue=Continue&&((ObserverRisolutore)i.next()).moveToUse(stato);
		}				
		return Continue;
	}

	public void terminato() {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).terminato();
		}						
	}

	public void terminatoUse() {
		Iterator i=t.iterator();
		while(i.hasNext()){
			((ObserverRisolutore)i.next()).terminatoUse();
		}								
	}

}
