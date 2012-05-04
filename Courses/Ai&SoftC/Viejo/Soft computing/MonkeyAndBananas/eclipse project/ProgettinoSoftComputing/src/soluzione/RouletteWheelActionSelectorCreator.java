package soluzione;

import java.io.Serializable;
import java.util.Comparator;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Vector;

import classiAstratte.ActionSelector;
import classiAstratte.ActionSelectorCreatorInterface;

public class RouletteWheelActionSelectorCreator implements ActionSelectorCreatorInterface,Serializable {

	private class MyDoubleComparator implements Comparator{

		public int compare(Object arg0, Object arg1) {
			double x1=((Double)arg0).doubleValue();
			double x2=((Double)arg1).doubleValue();
			if((x1==x2)||(x1>x2))return 1;
			return -1;
		}
		
	}

	private double valueForZero;
	
	public RouletteWheelActionSelectorCreator(double valueForZero){
		this.valueForZero=valueForZero;
	}
	
	public ActionSelector create(Vector v) {
		double soglie[];
		int  actions[];
		double val=((Double)v.elementAt(0)).doubleValue();
		double min=val;
		double sum=0;
		int nZero=0;

		soglie=new double[v.size()];
		actions=new int[v.size()];;
		for(int i=0;i<v.size();i++){
			actions[i]=i;
			val=((Double)v.elementAt(i)).doubleValue();
			soglie[i]=val;
			if(val<min)min=val;
		}
		for(int i=0;i<v.size();i++){
			soglie[i]=soglie[i]-min;
		}
		for(int i=0;i<v.size();i++){
			if(soglie[i]==0)nZero++;
			sum+=soglie[i];
		}

		if(nZero==v.size()){
			for(int i=0;i<v.size();i++){
				soglie[i]=(1.0/v.size())+i*1.0/v.size();
			}
			return new ActionSelector(soglie,actions);
		}
		double one=1-valueForZero*nZero;

		int i=0;
		if(soglie[i]==0){
			soglie[i]=valueForZero;
		}
		else{
			soglie[i]=one/(sum/soglie[i]);
		}
		for(i=1;i<v.size();i++){
			if(soglie[i]==0){
				soglie[i]=soglie[i-1]+valueForZero;
			}
			else{
				soglie[i]=soglie[i-1]+one/(sum/soglie[i]);
			}
		}		
		return new ActionSelector(soglie,actions);
	}


}
