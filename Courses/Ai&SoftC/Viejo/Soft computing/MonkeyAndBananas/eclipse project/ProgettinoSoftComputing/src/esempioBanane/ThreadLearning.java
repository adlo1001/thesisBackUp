package esempioBanane;

import classiAstratte.StatoInterface;
import esempioBanane.grafica.FrameMain;
import soluzione.Risolutore;

public class ThreadLearning extends Thread {
	private Risolutore qLearningSolution;
	private StatoInterface s;
	
	public ThreadLearning(Risolutore qLearningSolution){
		this.qLearningSolution=qLearningSolution;
	}
	
	public void setStatoStart(StatoInterface s){
		this.s=s;
	}
	
	public void run(){
		qLearningSolution.startQLearning(s);
	}
}
