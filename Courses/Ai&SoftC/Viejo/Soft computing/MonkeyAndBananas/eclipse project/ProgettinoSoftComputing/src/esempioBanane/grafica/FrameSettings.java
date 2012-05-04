package esempioBanane.grafica;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;

import soluzione.RouletteWheelActionSelectorCreator;
import soluzione.TournamentActionSelectorCreator;

import classiAstratte.ActionSelector;
import classiAstratte.ActionSelectorCreatorInterface;
import esempioBanane.Stato;

public class FrameSettings extends JDialog {
	private int numCols;
	private int numRows;
	private int alpha;
	private int gamma;
	private ActionSelectorCreatorInterface creator;
	
	private boolean confirmed;
	
	private JLabel tmpLabel[];
	private JComboBox cboNumCols;
	private JComboBox cboNumRows;
	private JComboBox cboActionSelector;
	private JSlider sliderAlfa;
	private JSlider sliderGamma;

	
	public FrameSettings(JFrame own,final int numCols,final int numRows,int alpha,int gamma,ActionSelectorCreatorInterface creator){
		super(own);
		this.numCols=numCols;
		this.numRows=numRows;
		this.alpha=alpha;
		this.gamma=gamma;
		this.creator=creator;
		
		setTitle("Settings...");
		setModal(true);
		setResizable(false);
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		
		GridBagConstraints limite = new GridBagConstraints();
		JPanel panMain=new JPanel();
		GridBagLayout gridMain = new GridBagLayout();
		setContentPane(panMain);
		panMain.setLayout(gridMain);
		limite.insets=new Insets(4,4,4,4); 
		limite.fill = GridBagConstraints.BOTH;
		limite.anchor = GridBagConstraints.NORTHWEST;

		tmpLabel=new JLabel[5];
		
		String values[]=new String[]{"4","5","6","7","8","9","10","11","12"};
		cboNumCols=new JComboBox(values);
		cboNumCols.setSelectedItem(new Integer(numCols).toString());
		tmpLabel[0]=new JLabel("Columns:");
		Utils.impostaLimite(limite, 0, 0, 1, 1, 0,0); 
		gridMain.setConstraints(tmpLabel[0], limite);
		panMain.add(tmpLabel[0]);
		Utils.impostaLimite(limite, 1, 0, 1, 1, 0,0); 
		gridMain.setConstraints(cboNumCols, limite);
		panMain.add(cboNumCols);
		cboNumCols.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				/*reset();
				Stato.numCol=new Integer((String)cboNumCols.getSelectedItem()).intValue();

				double w0[] = {0.0,              0.0,             0.0};
				double w1[] = {Stato.numCol,  0.0,             0.0};
				double w2[] = {0.0,              Stato.numRow, 0.0};
				worldBoard.resetObjectsPlaced();

				scene.setWorldExtent (w0, w1, w2);
				
				scene.redraw();
				scene.refresh();*/
			}
		});
		
		tmpLabel[1]=new JLabel("Rows:");
		Utils.impostaLimite(limite, 0, 1, 1, 1, 0,0); 
		gridMain.setConstraints(tmpLabel[1], limite);
		panMain.add(tmpLabel[1]);
		cboNumRows=new JComboBox(values);
		cboNumRows.setSelectedItem(new Integer(numRows).toString());
		Utils.impostaLimite(limite, 1, 1, 1, 1, 0,0); 
		gridMain.setConstraints(cboNumRows, limite);
		panMain.add(cboNumRows);
		cboNumRows.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				/*reset();
				Stato.numRow=new Integer((String)cboNumRows.getSelectedItem()).intValue();

				double w0[] = {0.0,              0.0,             0.0};
				double w1[] = {Stato.numCol,  0.0,             0.0};
				double w2[] = {0.0,              Stato.numRow, 0.0};
				worldBoard.resetObjectsPlaced();

				scene.setWorldExtent (w0, w1, w2);
				
				scene.redraw();
				scene.refresh();*/
			}
		});

		tmpLabel[2]=new JLabel("Alpha (x 100):");
		Utils.impostaLimite(limite, 0, 2, 1, 1, 0,0); 
		gridMain.setConstraints(tmpLabel[2], limite);
		panMain.add(tmpLabel[2]);		
		sliderAlfa=new JSlider(0,100);
		sliderAlfa.setMinorTickSpacing(5);
		sliderAlfa.setMajorTickSpacing(20);
		sliderAlfa.setPaintTicks(true);
		sliderAlfa.setSnapToTicks(true);
		sliderAlfa.setPaintTrack(false);
		sliderAlfa.setPaintLabels(true);
		sliderAlfa.setValue(alpha);
		Utils.impostaLimite(limite, 1, 2, 1, 1, 0,0); 
		gridMain.setConstraints(sliderAlfa, limite);
		panMain.add(sliderAlfa);

		
		tmpLabel[3]=new JLabel("Gamma (x 100):");
		Utils.impostaLimite(limite, 0, 3, 1, 1, 0,0); 
		gridMain.setConstraints(tmpLabel[3], limite);
		panMain.add(tmpLabel[3]);
		sliderGamma=new JSlider(0,100);
		sliderGamma.setMinorTickSpacing(5);
		sliderGamma.setMajorTickSpacing(20);
		sliderGamma.setPaintTicks(true);
		sliderGamma.setSnapToTicks(true);
		sliderGamma.setPaintTrack(false);
		sliderGamma.setPaintLabels(true);
		sliderGamma.setValue(gamma);
		Utils.impostaLimite(limite, 1, 3, 1, 1, 0,0); 
		gridMain.setConstraints(sliderGamma, limite);
		panMain.add(sliderGamma);

		tmpLabel[4]=new JLabel("Action Selector:");
		Utils.impostaLimite(limite, 0, 4, 1, 1, 0,0); 
		gridMain.setConstraints(tmpLabel[4], limite);
		panMain.add(tmpLabel[4]);
		cboActionSelector=new JComboBox(new String[]{"Tournament","Roulette Wheel"});
		Utils.impostaLimite(limite, 1, 4, 1, 1, 0,0); 
		gridMain.setConstraints(cboActionSelector, limite);
		panMain.add(cboActionSelector);
		if(creator instanceof TournamentActionSelectorCreator){
			cboActionSelector.setSelectedIndex(0);
		}
		else{
			cboActionSelector.setSelectedIndex(1);
		}
		
		JPanel endPanel=new JPanel();
		JButton cmdOk=new JButton("Ok");
		JButton cmdCancel=new JButton("Cancel");
		endPanel.add(cmdOk);
		endPanel.add(cmdCancel);
		Utils.impostaLimite(limite, 0, 5, 2, 1, 0,0); 
		gridMain.setConstraints(endPanel, limite);
		panMain.add(endPanel);
		
		cmdCancel.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				dispose();
			}
		});
		
		cmdOk.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				doOk();
			}			
		});
		
		confirmed=false;	
		pack();
		setLocationRelativeTo(own);
		
		
	}
	
	private void doOk(){
		confirmed=true;
		numRows=new Integer((String)cboNumRows.getSelectedItem()).intValue();
		numCols=new Integer((String)cboNumCols.getSelectedItem()).intValue();
		alpha=sliderAlfa.getValue();
		gamma=sliderGamma.getValue();
		if(cboActionSelector.getSelectedIndex()==0){
			creator=new TournamentActionSelectorCreator();
		}
		else{
			creator=new RouletteWheelActionSelectorCreator(0.1);
		}
		dispose();
	}
	
	public boolean getConfirmed(){
		return confirmed;
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
}
