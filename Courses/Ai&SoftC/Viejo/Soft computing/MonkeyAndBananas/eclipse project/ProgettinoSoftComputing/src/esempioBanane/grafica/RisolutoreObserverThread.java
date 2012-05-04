package esempioBanane.grafica;

import java.util.Vector;

import classiAstratte.StatoInterface;
import esempioBanane.Stato;
import esempioBanane.grafica.FrameMain.WorldBoard;
import soluzione.ObserverRisolutore;
import soluzione.Risolutore;

public class RisolutoreObserverThread implements ObserverRisolutore{
	public static int MODE_STEP=0;
	public static int MODE_ONE_EPOCH=1;
	public static int MODE_FW=2;
	private boolean stopFW=false;
	private boolean stopUse=false;
	
	private int mode;
	private WorldBoard worldBoard;
	private Risolutore qLearning;
	private FrameMain frame;
	
	//private boolean started;
	private Thread th;
	
	public void setStepMode(){
		mode=MODE_STEP;
	}

	public void setEpochMode(){
		mode=MODE_ONE_EPOCH;
	}

	public void setFWMode(){
		mode=MODE_FW;
		worldBoard.removeScimmia();
	}
	
	public RisolutoreObserverThread(Risolutore r,WorldBoard w,FrameMain frame){
		this.qLearning=r;
		this.worldBoard=w;
		this.frame=frame;
		mode=MODE_STEP;
		//started=false;
	}
	
	public void continueWork(){
		synchronized (th) {
			if(th.getState()==Thread.State.TERMINATED){
				init();
			}
			else{
				th.notify();
			}
		}
		
	}
	
	public void exit(){
		stopFWMode();
		stopUseMode();
	}
	
	public void start(){
		init();
	}

	private void init(){
		th=new RisolutoreThread(qLearning,new Stato((int)(Math.random()*Stato.numRow),(int)(Math.random()*Stato.numCol)));
		stopFW=false;
		th.start();
	}
	
	public void statoPartenza(StatoInterface st, Vector q) {
		if(mode==MODE_FW)return;
		worldBoard.placeScimmia(((Stato)st).getCol(),((Stato)st).getRow());
		if (mode==MODE_STEP){
			try{
				synchronized (th) {
					frame.enableButtonsDown(true);
					th.wait();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	public void azioneScelta(int a) {
		// TODO Auto-generated method stub
		
	}

	public void statoRaggiunto(StatoInterface st, Vector q) {
		if(mode==MODE_FW)return;
		if((((Stato)st).getCol()<0)||(((Stato)st).getRow()<0)){
			worldBoard.removeScimmia();
			frame.enableButtonsDown(true);
		}
		else{
			if (mode==MODE_STEP){
				if (!st.isStop()){
					worldBoard.moveScimmia(((Stato)st).getCol(),((Stato)st).getRow());
					try {
						synchronized (th) {
							frame.enableButtonsDown(true);
							th.wait();
						}
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
				else{
					worldBoard.removeScimmia();
					frame.enableButtonsDown(true);
				}
			}
			else if(mode==MODE_ONE_EPOCH){
				if (!st.isStop())worldBoard.placeScimmia(((Stato)st).getCol(),((Stato)st).getRow());
				else{ 
					worldBoard.removeScimmia();
					frame.enableButtonsDown(true);
				}
			}
		}
		
	}

	public void rinforzoOttenuto(double r) {
		// TODO Auto-generated method stub
		
	}

	public void soglieImpostate(int[] actions, double[] soglie) {
		// TODO Auto-generated method stub
		
	}

	public void aggiornamentoQ(StatoInterface st, Vector v) {
		worldBoard.aggiornaQ(((Stato)st).getCol(),((Stato)st).getRow(),v);
	}

	public void statoPartenzaUse(StatoInterface st) {
		worldBoard.placeScimmia(((Stato)st).getCol(),((Stato)st).getRow());		
	}

	public synchronized boolean moveToUse(StatoInterface st) {
		worldBoard.moveScimmia(((Stato)st).getCol(),((Stato)st).getRow());
		synchronized (th) {
			return !stopUse;
		}
	}

	public synchronized void terminato() {
		if(mode!=MODE_FW)frame.enableButtonsDown(true);
		if((mode==MODE_FW)&&(stopFW==false))init();		
	}
	
	public synchronized void stopFWMode(){
		synchronized (th) {
			stopFW=true;
		}
	}

	public synchronized void stopUseMode(){
		synchronized (th) {
			stopUse=true;
		}
	}

	public void useWhatLearned(Stato stato) {
		th=new UsoThread(qLearning,stato);
		stopUse=false;
		th.start();		
	}

	public void terminatoUse() {
		worldBoard.removeScimmia();	
		frame.resetModeUse();
		frame.enableButtonsDown(true);
	}
	
}
