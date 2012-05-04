package soluzione;

import java.io.Serializable;
import java.util.Comparator;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Vector;

import classiAstratte.ActionSelector;
import classiAstratte.ActionSelectorCreatorInterface;

public class TournamentActionSelectorCreator implements ActionSelectorCreatorInterface,Serializable {

	private class MyDoubleComparator implements Comparator{

		public int compare(Object arg0, Object arg1) {
			double x1=((Double)arg0).doubleValue();
			double x2=((Double)arg1).doubleValue();
			if((x1==x2)||(x1>x2))return 1;
			return -1;
		}
		
	}
	
	public ActionSelector create(Vector v) {
		TreeMap t=new TreeMap(new MyDoubleComparator());
		double soglie[];
		int  actions[];
	
		for(int i=0;i<v.size();i++){
			t.put(v.elementAt(i),new Integer(i));
		}
		Iterator i=t.values().iterator();
		soglie=new double[v.size()];
		actions=new int[v.size()];
		int k=0;
		double val=Math.pow(2,v.size());
		actions[k]=((Integer)i.next()).intValue();
		soglie[k]=1/val;
		val=val/2;
		k++;
		while(i.hasNext()){
			actions[k]=((Integer)i.next()).intValue();
			soglie[k]=soglie[k-1]+1/val;
			val=val/2;
			k++;
		}
		soglie[k-1]=1;
		return new ActionSelector(soglie,actions);
	}


}
