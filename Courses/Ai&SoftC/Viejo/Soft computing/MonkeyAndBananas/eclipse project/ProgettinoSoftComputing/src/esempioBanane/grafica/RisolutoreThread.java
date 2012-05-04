package esempioBanane.grafica;

import soluzione.Risolutore;
import classiAstratte.StatoInterface;

public class RisolutoreThread extends Thread {
	
	private StatoInterface s;	
	private Risolutore r;	
	
	public RisolutoreThread(Risolutore r,StatoInterface s){
		this.s=s;
		this.r=r;
	}
	
	public void run(){
		r.startQLearning(s);
	}
}
