package esempioBanane.grafica;

import java.io.Serializable;

import classiAstratte.ActionSelectorCreatorInterface;

public class Settings implements Serializable{
	private int numCols;
	private int numRows;
	private int alpha;
	private int gamma;
	private ActionSelectorCreatorInterface creator;

	public Settings(int numCols,int numRows,int alpha,int gamma,ActionSelectorCreatorInterface creator){
		this.numCols=numCols;
		this.numRows=numRows;
		this.alpha=alpha;
		this.gamma=gamma;
		this.creator=creator;
	}
		
	public int getNumCols(){
		return numCols;
	}
	public int getNumRows(){
		return numRows;
	}
	public int getAlpha(){
		return alpha;
	}
	public int getGamma(){
		return gamma;
	}
	public ActionSelectorCreatorInterface getCreator(){
		return creator;
	}

	public void setAlpha(int alpha) {
		this.alpha = alpha;
	}

	public void setCreator(ActionSelectorCreatorInterface creator) {
		this.creator = creator;
	}

	public void setGamma(int gamma) {
		this.gamma = gamma;
	}

	public void setNumCols(int numCols) {
		this.numCols = numCols;
	}

	public void setNumRows(int numRows) {
		this.numRows = numRows;
	}

}
