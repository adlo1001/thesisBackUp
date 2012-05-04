package classiAstratte;


public class ActionSelector {

	private double[] soglie;
	private int[] actions;
	public ActionSelector(double[] soglie, int[] actions) {
		this.soglie=soglie;
		this.actions=actions;
	}

	public double[] getSoglie(){
		return soglie;
	}
	public int[] getActions(){
		return actions;
	}
	
	public int select() {
		double x=Math.random();
		int i=0;
		while(i<actions.length){
			if(x<soglie[i]){
				return i;
			}
			i++;
		}
		return i-1;
	}
	
	public String toString(){
		int i=0;
		String s="";
		double prec=0;
		while(i<actions.length){
			s=s+actions[i]+" ["+prec+":"+soglie[i]+"]"+"\n";
			prec=soglie[i];
			i++;
		}
		return s;
	}


}
