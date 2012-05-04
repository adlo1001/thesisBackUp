package esempioBanane;

import java.util.Vector;

import classiAstratte.StatoInterface;
import soluzione.ObserverRisolutore;

public class MyObserver implements ObserverRisolutore {

	public void statoPartenza(StatoInterface st,Vector v) {
		if(v!=null){
			System.out.println("Stato partenza: "+ st +", Q: " +v);
		}
		else{
			System.out.println("Stato partenza: "+ st);
		}
	}

	public void azioneScelta(int a) {
		System.out.println("Azione: " + a);
	}

	public void statoRaggiunto(StatoInterface st2,Vector v) {
		if(v!=null){
			System.out.println("Stato raggiunto: " + st2+", Q: " +v);
		}
		else{
			System.out.println("Stato raggiunto: " + st2);
		}

	}

	public void rinforzoOttenuto(double r) {
		System.out.println("Rinforzo: " + r);
	}

	public void aggiornamentoQ(StatoInterface st,Vector v) {
		if(v!=null){
			System.out.println("Aggiornamento Q: " + st+", q: " +v);					
		}
		else{
			System.out.println("Aggiornamento Q: " + st);
		}
		
	}
	
	public void soglieImpostate(int[] actions, double[] soglie) {
		String s="";
		double prec=0;
		for(int i=0;i<actions.length;i++){
			s=s+actions[i] + ": ["+prec+","+soglie[i]+"]    ";
			prec=soglie[i];
		}
		System.out.println("Soglie: " + s);
	}

	public void statoPartenzaUse(StatoInterface stato) {
		System.out.println("Stato partenza: " + stato);				
	}

	public boolean moveToUse(StatoInterface stato) {
		System.out.println("Stato raggiunto: " + stato);
		return true;
	}

	public void terminato() {
		System.out.println("Terminato");				
	}

	public void terminatoUse() {
		System.out.println("Terminato");						
	}


}
