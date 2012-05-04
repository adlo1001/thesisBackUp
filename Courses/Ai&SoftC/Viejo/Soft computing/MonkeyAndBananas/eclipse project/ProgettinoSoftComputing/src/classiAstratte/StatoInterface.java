package classiAstratte;

public interface StatoInterface {

	public int getUniqueID();
	public int getNumAzioni();
	public StatoInterface doAction(int i);
	public boolean isStop();
}
