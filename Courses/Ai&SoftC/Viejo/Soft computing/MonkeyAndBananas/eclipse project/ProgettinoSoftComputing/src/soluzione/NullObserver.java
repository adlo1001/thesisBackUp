package soluzione;

import java.util.Vector;

import classiAstratte.StatoInterface;

public class NullObserver implements ObserverRisolutore {

	public void statoPartenza(StatoInterface st, Vector q) {
		// TODO Auto-generated method stub

	}

	public void azioneScelta(int a) {
		// TODO Auto-generated method stub

	}

	public void statoRaggiunto(StatoInterface st2, Vector q) {
		// TODO Auto-generated method stub

	}

	public void rinforzoOttenuto(double r) {
		// TODO Auto-generated method stub

	}

	public void soglieImpostate(int[] actions, double[] soglie) {
		// TODO Auto-generated method stub

	}

	public void aggiornamentoQ(StatoInterface st, Vector v) {
		// TODO Auto-generated method stub

	}

	public void statoPartenzaUse(StatoInterface stato) {
		// TODO Auto-generated method stub
		
	}

	public boolean moveToUse(StatoInterface stato) {
		return true;		
	}

	public void terminato() {
		// TODO Auto-generated method stub
		
	}

	public void terminatoUse() {
		// TODO Auto-generated method stub
		
	}

}
