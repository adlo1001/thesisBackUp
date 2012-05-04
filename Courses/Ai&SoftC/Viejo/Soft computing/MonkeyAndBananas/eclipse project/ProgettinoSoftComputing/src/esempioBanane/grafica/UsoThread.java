package esempioBanane.grafica;

import soluzione.Risolutore;
import classiAstratte.StatoInterface;

public class UsoThread extends Thread {

	private StatoInterface s;	
	private Risolutore r;	
	
	public UsoThread(Risolutore r,StatoInterface s){
		this.s=s;
		this.r=r;
	}
	
	public void run(){
		r.useWhatLearned(s);
	}

	
}
