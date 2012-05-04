package esempioBanane;

import java.text.DecimalFormat;
import java.util.Vector;

import javax.swing.plaf.basic.BasicSliderUI.ActionScroller;

import classiAstratte.ActionSelector;
import classiAstratte.ActionSelectorCreatorInterface;
import classiAstratte.ReinforcementFunInterface;
import soluzione.NullObserver;
import soluzione.Risolutore;
import soluzione.RouletteWheelActionSelectorCreator;
import soluzione.TournamentActionSelectorCreator;

public class Main {

	public static void main(String[] args) {
		ReinforcementFunBase rf=new ReinforcementFunBase();
		/*
		rf.addGoal(3,5);
		rf.addOstacolo(0,2);
		rf.addOstacolo(1,4);
		rf.addOstacolo(2,3);
		rf.addOstacolo(3,0);
		
		Stato.addGoal(3,5);
		Stato.addOstacolo(0,2);
		Stato.addOstacolo(1,4);
		Stato.addOstacolo(2,3);
		Stato.addOstacolo(3,0);
		*/
		rf.addGoal(0,3);
		rf.addOstacolo(0,1);
		rf.addOstacolo(0,2);
		rf.addOstacolo(1,3);
		rf.addOstacolo(1,4);
		rf.addOstacolo(2,1);
		rf.addOstacolo(2,3);
		rf.addOstacolo(3,0);
		rf.addOstacolo(3,5);

		Stato.addGoal(0,3);
		Stato.addOstacolo(0,1);
		Stato.addOstacolo(0,2);
		Stato.addOstacolo(1,3);
		Stato.addOstacolo(1,4);
		Stato.addOstacolo(2,1);
		Stato.addOstacolo(2,3);
		Stato.addOstacolo(3,0);
		Stato.addOstacolo(3,5);
		
		
		
		
		
		//Risolutore r=new Risolutore(rf,.9,.5,new TournamentActionSelectorCreator(),new MyObserver());
		//Risolutore r=new Risolutore(rf,.9,.5,new RouletteWheelActionSelectorCreator(0.2),new MyObserver());
		//Risolutore r=new Risolutore(rf,.9,.5,new RouletteWheelActionSelectorCreator(0.1),new NullObserver());
		Risolutore r=new Risolutore(rf,.9,.5,new TournamentActionSelectorCreator(),new MyObserver());
		for(int i=0;i<Stato.numRow;i++){
			for(int j=0;j<Stato.numCol;j++){
				Stato s=new Stato(i,j);
				r.addStato(s);
			}			
		}
		r.addStato(new Stato(-1,-1));

		r.startQLearning(new Stato(0,0));
		/*for(int i=0;i<100000;i++){
			r.startQLearning(new Stato((int)Math.ceil(Math.random()*(Stato.numRow-1)),(int)Math.ceil(Math.random()*(Stato.numCol-1))));
			//System.out.println(r);
		}*/	
		
		System.out.println(r);
		/*r.setObserver(new MyObserver());
		r.useWhatLearned(new Stato(2,0));*/
	}
	

}
