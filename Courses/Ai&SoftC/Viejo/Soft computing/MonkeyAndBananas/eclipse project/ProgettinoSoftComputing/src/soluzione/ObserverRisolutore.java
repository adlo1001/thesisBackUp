package soluzione;

import java.util.Vector;

import classiAstratte.StatoInterface;

public interface ObserverRisolutore {

	void statoPartenza(StatoInterface st,Vector q);

	void azioneScelta(int a);

	void statoRaggiunto(StatoInterface st2,Vector q);

	void rinforzoOttenuto(double r);

	void soglieImpostate(int[] actions, double[] soglie);
	
	public void aggiornamentoQ(StatoInterface st,Vector v) ;

	void statoPartenzaUse(StatoInterface stato);

	boolean moveToUse(StatoInterface stato);
	
	void terminato();

	void terminatoUse();

}
