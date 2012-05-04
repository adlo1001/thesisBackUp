package esempioBanane.grafica;


import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;
import javax.swing.filechooser.FileFilter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.URISyntaxException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import no.geosoft.cc.graphics.GImage;
import no.geosoft.cc.graphics.GInteraction;
import no.geosoft.cc.graphics.GObject;
import no.geosoft.cc.graphics.GPosition;
import no.geosoft.cc.graphics.GScene;
import no.geosoft.cc.graphics.GSegment;
import no.geosoft.cc.graphics.GStyle;
import no.geosoft.cc.graphics.GText;
import no.geosoft.cc.graphics.GWindow;
import soluzione.MultipleObserver;
import soluzione.Risolutore;
import soluzione.RouletteWheelActionSelectorCreator;
import esempioBanane.MyObserver;
import esempioBanane.ReinforcementFunBase;
import esempioBanane.Stato;


public class FrameMain extends JFrame implements GInteraction{
	
	private BufferedImage fBanane;
	private BufferedImage fPalma;
	private BufferedImage fErase;
	private BufferedImage fScimmia;
	
	private GScene scene;
	private GWindow window;
	int nBanane;
	
	private double bananePositionX[];
	private double bananePositionY[];
	private int nPalme;
	private double palmePositionX[];
	private double palmePositionY[];
	private WorldBoard worldBoard;
	private GSegment tempSegPlacement;
	private JButton cmdSettings;
	private JButton cmdPlaceBanane;
	private JButton cmdPlacePalme;
	private JButton cmdErase;
	private JButton cmdUse;

	private JButton cmdOpen;
	private JButton cmdSave;	

	private JButton cmdOneStep;
	private JButton cmdOneEpoch;
	private JButton cmdFast;
	private JButton cmdPause;
	
	private RisolutoreObserverThread rth;
	private int modeUse;

	private Settings settings;
	private JButton cmdResetQ;
	
	public void enableAllButtons(boolean b){
        cmdSettings.setEnabled(b);
        cmdPlacePalme.setEnabled(b);
        cmdErase.setEnabled(b);
        cmdPlaceBanane.setEnabled(b);
        cmdUse.setEnabled(b);
		cmdOneStep.setEnabled(b);
		cmdOneEpoch.setEnabled(b);
		cmdFast.setEnabled(b);
		cmdPause.setEnabled(b);
		/*cboNumCols.setEnabled(b);
		cboNumRows.setEnabled(b);
		sliderAlfa.setEnabled(b);
		sliderGamma.setEnabled(b);
		for(int i=0;i<tmpLabel.length;i++){
			tmpLabel[i].setEnabled(b);
		}*/
		cmdOpen.setEnabled(b);
		cmdSave.setEnabled(b);
		cmdResetQ.setEnabled(b);
	}
	
	public void enableButtonsRight(boolean b){
        cmdPlacePalme.setEnabled(b);
        cmdErase.setEnabled(b);
        cmdPlaceBanane.setEnabled(b);
		/*cboNumCols.setEnabled(b);
		cboNumRows.setEnabled(b);
		sliderAlfa.setEnabled(b);
		sliderGamma.setEnabled(b);
		for(int i=0;i<tmpLabel.length;i++){
			tmpLabel[i].setEnabled(b);
		}*/
		cmdOpen.setEnabled(b);
		cmdSave.setEnabled(b);
        //cmdReset.setEnabled(b);
        
	}

	public void enableButtonsDown(boolean b){
		cmdOneStep.setEnabled(b);
		cmdOneEpoch.setEnabled(b);
		cmdFast.setEnabled(b);
        cmdSettings.setEnabled(b);
        cmdUse.setEnabled(b);
		cmdResetQ.setEnabled(b);
	}
	
	private void reset(){
		nPalme=0;
		palmePositionX=new double[0];
		palmePositionY=new double[0];
		nBanane=0;
		bananePositionX=new double[0];
		bananePositionY=new double[0];
		
	}
	
	public FrameMain(){
		
		settings=new Settings(Stato.numCol,Stato.numRow,50,90,new RouletteWheelActionSelectorCreator(.1));
		
		palmePositionX=new double[0];
		palmePositionY=new double[0];
		bananePositionX=new double[0];
		bananePositionY=new double[0];
		
		rth=null;
		addWindowListener(new WindowListener(){

			public void windowOpened(WindowEvent arg0) {}
			public void windowClosing(WindowEvent arg0) {
				if(rth!=null){
					rth.exit();
				}
			}
			public void windowClosed(WindowEvent arg0) {}
			public void windowIconified(WindowEvent arg0) {}
			public void windowDeiconified(WindowEvent arg0) {}
			public void windowActivated(WindowEvent arg0) {	}
			public void windowDeactivated(WindowEvent arg0) {}
			
		});
		
		setTitle("QLearning monkey");
		JPanel panMain=new JPanel();
		JPanel panRight=new JPanel();
		JPanel panGrid=new JPanel();
		JPanel panDown=new JPanel();
		GridBagLayout gridMain = new GridBagLayout();
		GridBagLayout gridRight = new GridBagLayout();
		GridBagLayout gridDown = new GridBagLayout();
		GridBagConstraints limite = new GridBagConstraints();
		
		setContentPane(panMain);
		panMain.setLayout(gridMain);
		panRight.setLayout(gridRight);
		panGrid.setLayout(new BorderLayout());
		panDown.setLayout(gridDown);
		limite.insets=new Insets(2,2,2,2); 
		
		
		try {
			fBanane=ImageIO.read(ClassLoader.getSystemResource("images/ico/bananeico.png"));
			fPalma=ImageIO.read(ClassLoader.getSystemResource("images/ico/palmaico.png"));
			fErase=ImageIO.read(ClassLoader.getSystemResource("images/ico/forbidico.png"));
			fScimmia=ImageIO.read(ClassLoader.getSystemResource("images/ico/scimmiaico.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
//		--------------------------------
		//pannello a destra
		cmdSettings=new JButton("Settings...",	new ImageIcon(ClassLoader.getSystemResource("images/ico/settingsico.png")));       
		cmdSettings.setVerticalTextPosition(SwingConstants.BOTTOM);
		cmdSettings.setHorizontalTextPosition(SwingConstants.CENTER);
		cmdPlaceBanane=new JButton("Place goal",	new ImageIcon(ClassLoader.getSystemResource("images/ico/bananeico.png")));
		cmdPlaceBanane.setVerticalTextPosition(SwingConstants.BOTTOM);
		cmdPlaceBanane.setHorizontalTextPosition(SwingConstants.CENTER);
		cmdPlacePalme=new JButton("Place obstacle",	new ImageIcon(ClassLoader.getSystemResource("images/ico/palmaico.png")));
		cmdPlacePalme.setVerticalTextPosition(SwingConstants.BOTTOM);
		cmdPlacePalme.setHorizontalTextPosition(SwingConstants.CENTER);
		cmdErase=new JButton("Erase",	new ImageIcon(ClassLoader.getSystemResource("images/ico/forbidico.png")));
		cmdErase.setVerticalTextPosition(SwingConstants.BOTTOM);
		cmdErase.setHorizontalTextPosition(SwingConstants.CENTER);
		cmdUse=new JButton("Use!",new ImageIcon(ClassLoader.getSystemResource("images/ico/scimmiaico.png"))); 
		cmdUse.setVerticalTextPosition(SwingConstants.BOTTOM);
		cmdUse.setHorizontalTextPosition(SwingConstants.CENTER);
		modeUse=0;

		cmdOpen=new JButton("Open",new ImageIcon(ClassLoader.getSystemResource("images/ico/folderico.png"))); 
		cmdOpen.setVerticalTextPosition(SwingConstants.BOTTOM);
		cmdOpen.setHorizontalTextPosition(SwingConstants.CENTER);
		cmdSave=new JButton("Save",new ImageIcon(ClassLoader.getSystemResource("images/ico/saveico.png"))); 
		cmdSave.setVerticalTextPosition(SwingConstants.BOTTOM);
		cmdSave.setHorizontalTextPosition(SwingConstants.CENTER);

		cmdOpen.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				JFileChooser chooser = new JFileChooser();
				FileFilter filter=new FileFilter(){

					public boolean accept(File f) {
						if (f.isDirectory())
							return true;
						String ext = null;
						String s = f.getName();
						int i = s.lastIndexOf('.');
						if ((i > 0) && (i < s.length() - 1)) {
							ext = s.substring(i + 1);
						}
						if(ext!=null){
							if (ext.equalsIgnoreCase("qmonk"))return true;
							else return false;
						}
						else return false;
					}

					public String getDescription() {
						return "Q-Learning Mokey (*.qmonk)";
					}
					
				};
				chooser.setFileFilter(filter);
				chooser.setCurrentDirectory(new File("").getAbsoluteFile());
				int back = chooser.showOpenDialog(FrameMain.this);
				if (back == JFileChooser.APPROVE_OPTION) {
					File f= chooser.getSelectedFile();
					try {
						loadWorld(f.getAbsolutePath());
						//reset();
						if(rth!=null){
							rth.exit();
						}
						rth=null;
						worldBoard.resetQValues();
						worldBoard.removeScimmia();
						
						Stato.numRow=settings.getNumRows();
						Stato.numCol=settings.getNumCols();

						double w0[] = {0.0,0.0,0.0};
						double w1[] = {Stato.numCol,0.0,0.0};
						double w2[] = {0.0,Stato.numRow,0.0};
						worldBoard.resetObjectsPlaced();

						scene.setWorldExtent (w0, w1, w2);
						
						scene.redraw();
						worldBoard.redraw();
						scene.refresh();
						worldBoard.refresh();
						enableAllButtons(true);
						cmdPause.setEnabled(false);
					} 
					catch (Exception e) {
						JOptionPane.showMessageDialog(FrameMain.this,"Error opening file\n"+e.toString());
						e.printStackTrace();
					}
				}

			}			
		});
		
		cmdSave.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				JFileChooser chooser = new JFileChooser();
				FileFilter filter=new FileFilter(){

					public boolean accept(File f) {
						if (f.isDirectory())
							return true;
						String ext = null;
						String s = f.getName();
						int i = s.lastIndexOf('.');
						if ((i > 0) && (i < s.length() - 1)) {
							ext = s.substring(i + 1);
						}
						if(ext!=null){
							if (ext.equalsIgnoreCase("qmonk"))return true;
							else return false;
						}
						else return false;
					}

					public String getDescription() {
						return "Q-Learning Mokey (*.qmonk)";
					}
					
				};
				chooser.setFileFilter(filter);
				chooser.setCurrentDirectory(new File("").getAbsoluteFile());
				int back = chooser.showSaveDialog(FrameMain.this);
				
				if (back == JFileChooser.APPROVE_OPTION) {
					File f= chooser.getSelectedFile();
					String fileName = f.getName();
					if (fileName .lastIndexOf('.') == -1) {
						f= new File(f.getParentFile(), fileName + ".qmonk");
					}
					if (f.exists()) {
						int answer = JOptionPane.showConfirmDialog(chooser, "File '" + f.getName()
								+ "' already exists.\nOverwrite?", "'" + f.getName()
								+ "' already exists", JOptionPane.YES_NO_OPTION);

						if (answer != JOptionPane.OK_OPTION) {
							return;
						}
					}	
					try {
						saveWorld(f.getAbsolutePath());
					} 
					catch (Exception e) {
						JOptionPane.showMessageDialog(FrameMain.this,"Error saving file\n"+e.toString());
						e.printStackTrace();
					}
				}
			}			
		});		
		
		Utils.impostaLimite(limite, 0, 0, 1, 1, 0,0); 
		limite.fill = GridBagConstraints.BOTH;
		limite.anchor = GridBagConstraints.NORTHWEST;
		gridRight.setConstraints(cmdOpen, limite);
		panRight.add(cmdOpen);

		Utils.impostaLimite(limite, 0, 1, 1, 1, 0,0); 
		gridRight.setConstraints(cmdSave, limite);
		panRight.add(cmdSave);
				
		
		Utils.impostaLimite(limite, 0, 2, 1, 1, 0,0); 
		gridRight.setConstraints(cmdPlaceBanane, limite);
		panRight.add(cmdPlaceBanane);
		cmdPlaceBanane.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				tempSegPlacement=new GSegment();
				tempSegPlacement.setUserData("banana");
		        GStyle style = new GStyle();
		        style.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
		        tempSegPlacement.setStyle (style);
		        tempSegPlacement.setImage(new GImage(fBanane,GPosition.CENTER));
		        enableAllButtons(false);
			}
			
		});
		
		Utils.impostaLimite(limite, 0, 3, 1, 1, 0,0); 
		gridRight.setConstraints(cmdPlacePalme, limite);
		panRight.add(cmdPlacePalme);
		cmdPlacePalme.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				tempSegPlacement=new GSegment();
				tempSegPlacement.setUserData("palma");
		        GStyle style = new GStyle();
		        style.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
		        tempSegPlacement.setStyle (style);
		        tempSegPlacement.setImage(new GImage(fPalma,GPosition.CENTER));
		        enableAllButtons(false);
		        
			}
			
		});
		
		Utils.impostaLimite(limite, 0, 4, 1, 1, 0,0); 
		gridRight.setConstraints(cmdErase, limite);
		panRight.add(cmdErase);
		cmdErase.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				tempSegPlacement=new GSegment();
				tempSegPlacement.setUserData("erase");
		        GStyle style = new GStyle();
		        style.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
		        tempSegPlacement.setStyle (style);
		        tempSegPlacement.setImage(new GImage(fErase,GPosition.CENTER));
		        enableAllButtons(false);
		        
			}
			
		});
		
		Utils.impostaLimite(limite, 0, 5, 1, 1, 0,0); 
		gridRight.setConstraints(cmdUse, limite);
		panRight.add(cmdUse);
		
		cmdUse.addActionListener(new ActionListener(){

			public void actionPerformed(ActionEvent arg0) {			
				if(modeUse==0){
					worldBoard.removeScimmia();
					tempSegPlacement=new GSegment();
					tempSegPlacement.setUserData("scimmia");
			        GStyle style = new GStyle();
			        style.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
			        tempSegPlacement.setStyle (style);
			        tempSegPlacement.setImage(new GImage(fScimmia,GPosition.CENTER));
			        enableAllButtons(false);
			        cmdUse.setText("STOP");
			        modeUse=1;
				}
				else{
					cmdUse.setText("Use!");
					cmdUse.setEnabled(false);
					rth.stopUseMode();
					modeUse=0;
				}
			}
			
		});
		
		Utils.impostaLimite(limite, 0, 6, 1, 1, 0,0); 
		gridRight.setConstraints(cmdSettings, limite);
		panRight.add(cmdSettings);
		cmdSettings.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {

				FrameSettings fTmp=new FrameSettings(FrameMain.this,settings.getNumCols(),settings.getNumRows(),settings.getAlpha(),settings.getGamma(),settings.getCreator());
				fTmp.setVisible(true);
				if(fTmp.getConfirmed()){
					settings.setAlpha(fTmp.getAlpha());
					settings.setGamma(fTmp.getGamma());
					settings.setNumCols(fTmp.getNumCols());
					settings.setNumRows(fTmp.getNumRows());
					settings.setCreator(fTmp.getCreator());
					reset();
					if(rth!=null){
						rth.exit();
					}
					rth=null;
					worldBoard.resetQValues();
					worldBoard.removeScimmia();
					
					Stato.numRow=settings.getNumRows();
					Stato.numCol=settings.getNumCols();

					double w0[] = {0.0,              0.0,             0.0};
					double w1[] = {Stato.numCol,  0.0,             0.0};
					double w2[] = {0.0,              Stato.numRow, 0.0};
					worldBoard.resetObjectsPlaced();

					scene.setWorldExtent (w0, w1, w2);
					
					scene.redraw();
					scene.refresh();
					enableAllButtons(true);
					cmdPause.setEnabled(false);
					
				}
			}
		});

		
		
		
		/*
		 //Rimuovere il commento per avere i bottoni tutti in alto 
		  JPanel blankPan=new JPanel();
		  blankPan.setBackground(panRight.getBackground());
		  Utils.impostaLimite(limite, 0, 5, 1, 1, 0,1); 
		  gridRight.setConstraints(blankPan, limite);
		  panRight.add(blankPan);
		  */
//		---------------------------------------------------------
		//pannello in basso
		cmdOneStep=new JButton(">");
		cmdOneEpoch=new JButton(">|");
		cmdFast=new JButton(">>");
		cmdPause=new JButton("||");
		cmdResetQ=new JButton("Reset Q-values");
		
		cmdOneStep.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				enableAllButtons(false);
				if(rth==null){
					initLearning();
					rth.setStepMode();
					rth.start();
				}
				else{
					rth.setStepMode();
					rth.continueWork();
				}
			}
		});

		cmdOneEpoch.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				enableAllButtons(false);
				if(rth==null){
					initLearning();
					rth.setEpochMode();
					rth.start();
				}
				else{
					rth.setEpochMode();
					rth.continueWork();
				}
			}

		});
		
		cmdFast.addActionListener(new ActionListener(){

			public void actionPerformed(ActionEvent arg0) {
				enableAllButtons(false);
				cmdPause.setEnabled(true);
				if(rth==null){
					initLearning();
					rth.setFWMode();
					rth.start();
				}
				else{
					rth.setFWMode();
					rth.continueWork();
				}				
			}
			
		});
		
		Utils.impostaLimite(limite, 0, 0, 1, 1, 0,0); 
		limite.fill = GridBagConstraints.BOTH;
		limite.anchor = GridBagConstraints.CENTER;
		gridDown.setConstraints(cmdOneStep, limite);
		panDown.add(cmdOneStep);
		
		Utils.impostaLimite(limite, 1, 0, 1, 1, 0,0); 
		gridDown.setConstraints(cmdOneEpoch, limite);
		panDown.add(cmdOneEpoch);
		
		Utils.impostaLimite(limite, 2, 0, 1, 1, 0,0); 
		gridDown.setConstraints(cmdFast, limite);
		panDown.add(cmdFast);
		
		Utils.impostaLimite(limite, 3, 0, 1, 1, 0,0); 
		gridDown.setConstraints(cmdPause, limite);
		panDown.add(cmdPause);
		cmdPause.addActionListener(new ActionListener(){

			public void actionPerformed(ActionEvent arg0) {
				//cmdUse.setEnabled(true);
				rth.stopFWMode();
				enableButtonsDown(true);
				cmdPause.setEnabled(false);
				
			}
			
		});

		Utils.impostaLimite(limite, 4, 0, 1, 1, 0,0); 
		gridDown.setConstraints(cmdResetQ, limite);
		panDown.add(cmdResetQ);
		cmdResetQ.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				if(rth!=null){
					rth.exit();
				}
				rth=null;
				worldBoard.resetQValues();
				worldBoard.removeScimmia();
				worldBoard.refresh();
				enableAllButtons(true);
				cmdPause.setEnabled(false);				
			}
			
		});
		
//		---------------------------------------------------------
		//pannello della griglia in cui si muove la scimmia
		window = new GWindow (panDown.getBackground());
		panGrid.add (window.getCanvas(), BorderLayout.CENTER);
		
		scene = new GScene (window);
		double w0[] = {0.0,              0.0,             0.0};
		double w1[] = {Stato.numCol,  0.0,             0.0};
		double w2[] = {0.0,              Stato.numRow, 0.0};
		scene.setWorldExtent (w0, w1, w2);
		
		worldBoard = new WorldBoard();
		scene.add (worldBoard );			
		
//		---------------------------------------------------------
		//pannello principale
		
		Utils.impostaLimite(limite, 0, 0, 1, 1, 1,1); 
		limite.fill = GridBagConstraints.BOTH;
		limite.anchor = GridBagConstraints.NORTHWEST;
		gridMain.setConstraints(panGrid, limite);
		panMain.add(panGrid);
		
		Utils.impostaLimite(limite, 0, 1, 1, 1, 0,0); 
		limite.fill = GridBagConstraints.BOTH;
		limite.anchor = GridBagConstraints.NORTHWEST;
		gridMain.setConstraints(panDown, limite);
		panMain.add(panDown);
		
		
		Utils.impostaLimite(limite, 1, 0, 1, 2, 0,0); 
		limite.fill = GridBagConstraints.BOTH;
		limite.anchor = GridBagConstraints.NORTHWEST;
		gridMain.setConstraints(panRight, limite);
		panMain.add(panRight);
		
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		//setBounds(0,0,1000,700);
		setExtendedState(MAXIMIZED_BOTH);
		setResizable(false);
		setLocationRelativeTo(null);

		enableAllButtons(false);

		window.startInteraction(this);
		
	}
	
	public void disablePause(){
		cmdPause.setEnabled(false);
	}

	private int GiaPresente(double x[],double y[],int i,int j){
		for(int k=0;k<x.length;k++){
			if((x[k]==i)&&(y[k]==j))return k;
		}
		return -1;
	}
	
	public void event(GScene scene, int event, int x, int y) {
	    if (scene == null) return;
	    if (tempSegPlacement== null) return;
	    
	    GObject interaction = scene.find ("interaction");
	    if (interaction == null) {
	      interaction = new GObject ("interaction");
	      scene.add (interaction);
	    }

	    interaction.addSegment(tempSegPlacement);


	    double[] w = scene.getTransformer().deviceToWorld (x, y);

	    //System.out.println(w[0] + " " +w[1]);
	    if (w[0] < 0 || w[0] >= Stato.numCol|| w[1] < 0 || w[1] >= Stato.numRow) 
	      return;

	    w[0]=w[0]-.5;
	    w[1]=w[1]-.5;
		switch (event) {
	      case GWindow.MOTION     :
		        double xy[] = new double[]{
	        		w[0]+.5,w[1]+.5,
	        		w[0]+.25,w[1]+.25,
	        		w[0]+.25,w[1]+.75,
	        		w[0]+.75,w[1]+.75,
	        		w[0]+.75,w[1]+.25,
	        		w[0]+.25,w[1]+.25,
	        };

		    tempSegPlacement.setGeometryXy (xy);
	        break;
	        
	      case GWindow.BUTTON1_UP :
	  	    int i = (int) (w[1]+.5) ;
	  	    int j = (int) (w[0]+.5) ;
		    //System.out.println(w[0] + ":"+j+" , " +w[1]+": "+i);
		    
		    if(tempSegPlacement.getUserData().equals("banana")){
				if((GiaPresente(bananePositionX,bananePositionY,j,i)==-1)&&(GiaPresente(palmePositionX,palmePositionY,j,i)==-1)){
			    	
			    	nBanane++;
					double x1[]=new double[nBanane];
					double y1[]=new double[nBanane];
					for(int k=0;k<nBanane-1;k++){
						x1[k]=bananePositionX[k];
						y1[k]=bananePositionY[k];
					}
					x1[nBanane-1]=j;
					y1[nBanane-1]=i;
					bananePositionX=x1;
					bananePositionY=y1;
	
					scene.redraw();
				}
				scene.remove(interaction);
		        enableButtonsRight(true);
		        enableButtonsDown(true);
		    }
		    else if(tempSegPlacement.getUserData().equals("palma")){
				if((GiaPresente(bananePositionX,bananePositionY,j,i)==-1)&&(GiaPresente(palmePositionX,palmePositionY,j,i)==-1)){
					nPalme++;
					double x1[]=new double[nPalme];
					double y1[]=new double[nPalme];
					for(int k=0;k<nPalme-1;k++){
						x1[k]=palmePositionX[k];
						y1[k]=palmePositionY[k];
					}
					x1[nPalme-1]=j;
					y1[nPalme-1]=i;
					palmePositionX=x1;
					palmePositionY=y1;
	
					scene.redraw();
				}
				scene.remove(interaction);
		        enableButtonsRight(true);
		        enableButtonsDown(true);
		    }
		    else if(tempSegPlacement.getUserData().equals("erase")){
				int b=GiaPresente(bananePositionX,bananePositionY,j,i);
		    	if(b>=0){
					nBanane--;
					double x1[]=new double[nBanane];
					double y1[]=new double[nBanane];
					int v=0;
					for(int k=0;k<nBanane+1;k++){
						if(k!=b){
							x1[v]=bananePositionX[k];
							y1[v]=bananePositionY[k];
							v++;
						}
					}
					bananePositionX=x1;
					bananePositionY=y1;	
					scene.redraw();
				}
		    	else{
		    		b=GiaPresente(palmePositionX,palmePositionY,j,i);
			    	if(b>=0){
						nPalme--;
						double x1[]=new double[nPalme];
						double y1[]=new double[nPalme];
						int v=0;
						for(int k=0;k<nPalme+1;k++){
							if(k!=b){
								x1[v]=palmePositionX[k];
								y1[v]=palmePositionY[k];
								v++;
							}
						}
						palmePositionX=x1;
						palmePositionY=y1;	
						scene.redraw();
					}
		    		
		    	}
				scene.remove(interaction);
		        enableButtonsRight(true);
		        enableButtonsDown(true);
					
		    }
		    else if(tempSegPlacement.getUserData().equals("scimmia")){
				scene.remove(interaction);
				worldBoard.placeScimmia(j,i);
		        enableAllButtons(false);
		        if(rth==null)initLearning();
		        cmdUse.setEnabled(true);
		        rth.useWhatLearned(new Stato(i,j));
		    }
			tempSegPlacement=null;
  
	    }
	  scene.refresh();  
	
	}
	
	private void initLearning() {
		Risolutore qLearningSolution;
		ReinforcementFunBase rf=new ReinforcementFunBase();
		Stato.reset();
		for(int i=0;i<nBanane;i++){
			rf.addGoal((int)bananePositionY[i],(int)bananePositionX[i]);
			Stato.addGoal((int)bananePositionY[i],(int)bananePositionX[i]);
		}
		for(int i=0;i<nPalme;i++){
			rf.addOstacolo((int)palmePositionY[i],(int)palmePositionX[i]);
			Stato.addOstacolo((int)palmePositionY[i],(int)palmePositionX[i]);
		}
		
		MultipleObserver mo=new MultipleObserver();
		mo.addObserver(new MyObserver());		
		
		qLearningSolution=new Risolutore(rf,0.9,0.5,new RouletteWheelActionSelectorCreator(0.1),mo);
		//qLearningSolution=new Risolutore(rf,sliderGamma.getValue()/100.0,sliderAlfa.getValue()/100.0,new RouletteWheelActionSelectorCreator(0.1),mo);
		rth=new RisolutoreObserverThread(qLearningSolution,worldBoard,this);
		mo.addObserver(rth);
		for(int i=0;i<Stato.numRow;i++){
			for(int j=0;j<Stato.numCol;j++){
				Stato s=new Stato(i,j);
				qLearningSolution.addStato(s);
			}			
		}
		qLearningSolution.addStato(new Stato(-1,-1));
	}
	
	class WorldBoard extends GObject
	{
		private DecimalFormat format;

		private GSegment    board_;
		private GSegment[]  gridx;
		private GSegment[]  gridy;
		private List banane;
		private List palme;
		private GSegment label[][][];
		private GSegment    scimmia;
		private double scimmiaX;
		private double scimmiaY;
		
		public WorldBoard()
		{
			format=new DecimalFormat();
			format.setMaximumFractionDigits(1);
			format.setMinimumFractionDigits(1);
			format.setMaximumIntegerDigits(4);
			format.setMinimumIntegerDigits(1);
			format.setGroupingUsed(false);

			board_ = new GSegment();
			GStyle boardStyle = new GStyle();
			boardStyle.setBackgroundColor (new Color (255,255, 255));
			boardStyle.setForegroundColor (new Color (0,0, 0));
			board_.setStyle (boardStyle);
			addSegment (board_);

			creaGriglia();
			
		}
		
		public void resetQValues() {
			for(int i=0;i<Stato.numRow;i++){
				for(int j=0;j<Stato.numCol;j++){
					for(int k=0;k<Stato.numAzioni;k++){
						label[i][j][k].getText().setText(format.format(0));
					}
				}
			}
		}

		public void placeScimmia(int i,int j){
			if(i<0)return;
			if(j<0)return;
			if(scimmia==null){
				scimmia=new GSegment();
		        GStyle style = new GStyle();
		        style.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
		        scimmia.setStyle (style);
		        scimmia.setImage(new GImage(fScimmia,GPosition.CENTER));
		        addSegment(scimmia);
			}
	        scimmiaX=i;
	        scimmiaY=j;
	        redraw();
	        refresh();
		}
		
		public void moveScimmia(int i,int j){
			if(i<0)return;
			if(j<0)return;
			if(scimmia==null){
				scimmia=new GSegment();
		        GStyle style = new GStyle();
		        style.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
		        scimmia.setStyle (style);
		        scimmia.setImage(new GImage(fScimmia,GPosition.CENTER));
		        addSegment(scimmia);
			}
			double step;
			int n=Math.max(Stato.numRow,Stato.numCol);
			if(n<=4)step=0.1;
			else if(n>10)step=0.5;
			else step=(n-4)*.4/6+0.1;
			if(scimmiaX<i){
				for(double v=scimmiaX+step;v<=i;v+=step){
					scimmiaX=v;
					redraw();
					refresh();
				}
			}
			else{
				for(double v=scimmiaX-step;v>=i;v-=step){
					scimmiaX=v;
					redraw();
					refresh();

				}				
			}
			if(scimmiaY<j){
				for(double v=scimmiaY+step;v<=j;v+=step){
					scimmiaY=v;
					redraw();
					refresh();

				}
			}
			else{
				for(double v=scimmiaY-step;v>=j;v-=step){
					scimmiaY=v;
					redraw();
					refresh();
				}				
			}

	        scimmiaX=i;
	        scimmiaY=j;
	        redraw();
			refresh();
			
		}
		

		public void removeScimmia(){
			if(scimmia!=null){
				scimmia.removeImages();
		        removeSegment(scimmia);
		        scimmia=null;
			}
	        redraw();
	        refresh();
		}

		public void draw()
		{
			int sizex = Stato.numCol;
			int sizey = Stato.numRow;
			
			// Board
			board_.setGeometryXy (new double[] {0, 0,
					sizex , 0,
					sizex , sizey ,
					0, sizey,
					0, 0});
			
			// Grid lines
			
			//NB: 0.0 serve perchè se prende "int" non usa le cordinate utente
			//ma come pixel
			for (int i = 0; i <= sizey; i++) {
				gridy[i].setGeometry (0.0, i+0.0 , sizex+0.0 , i+0.0 );
			}
			for (int i = 0; i <= sizex; i++) {
				gridx[i].setGeometry (i+0.0 , 0.0, i+0.0 , sizey+0.0 );
			}
			
			for(int i=0;i<Stato.numRow;i++){
				for(int j=0;j<Stato.numCol;j++){
					label[i][j][0].setGeometry(j+0.5,i+0.01);
					label[i][j][2].setGeometry(j+0.5,i+0.99);
					
					label[i][j][3].setGeometry(j+0.01,i+0.5);
					label[i][j][1].setGeometry(j+0.99,i+0.5);
					
				}
			}

			
			if(nBanane<banane.size()){
				GSegment piece=(GSegment) banane.get(0);
				banane.remove(0);
				piece.removeImages();
				removeSegment(piece);
			}
			if(nPalme<palme.size()){
				GSegment piece=(GSegment) palme.get(0);
				palme.remove(0);
				piece.removeImages();
				removeSegment(piece);
			}
			
			for(int i=0;i<nBanane;i++){
				GSegment piece=null;
				if(i<banane.size()){
					piece=(GSegment) banane.get(i);
				}
				else{
					piece=new GSegment();
			        GStyle style = new GStyle();
			        style.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
			        piece.setStyle (style);
			        piece.setImage(new GImage(fBanane,GPosition.CENTER));
			        addSegment(piece);
			        banane.add(piece);

				}
		        double xy[] = new double[]{
		        		bananePositionX[i]+.5,bananePositionY[i]+.5,
		        		bananePositionX[i]+.25,bananePositionY[i]+.25,
		        		bananePositionX[i]+.25,bananePositionY[i]+.75,
		        		bananePositionX[i]+.75,bananePositionY[i]+.75,
		        		bananePositionX[i]+.75,bananePositionY[i]+.25,
		        		bananePositionX[i]+.25,bananePositionY[i]+.25,
		        };
				piece.setGeometryXy(xy);

			}
			for(int i=0;i<nPalme;i++){
				GSegment piece=null;
				if(i<palme.size()){
					piece=(GSegment) palme.get(i);
				}
				else{
					piece=new GSegment();
			        GStyle style = new GStyle();
			        style.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
			        piece.setStyle (style);
			        piece.setImage(new GImage(fPalma,GPosition.CENTER));
			        addSegment(piece);
			        palme.add(piece);

				}
		        double xy[] = new double[]{
		        		palmePositionX[i]+.5,palmePositionY[i]+.5,
		        		palmePositionX[i]+.25,palmePositionY[i]+.25,
		        		palmePositionX[i]+.25,palmePositionY[i]+.75,
		        		palmePositionX[i]+.75,palmePositionY[i]+.75,
		        		palmePositionX[i]+.75,palmePositionY[i]+.25,
		        		palmePositionX[i]+.25,palmePositionY[i]+.25,
		        };
				piece.setGeometryXy(xy);

			}
			
			if(scimmia!=null){
				scimmia.setGeometry(scimmiaX+.5,scimmiaY+.5);
			}
		}

		public void aggiornaQ(int col, int row, Vector v) {
			if(col<0)return;
			if(row<0)return;

			for(int k=0;k<Stato.numAzioni;k++){
				//label[row][col][k].getText().setText(String.format("0000.0",new Object[]{v.elementAt(k)}));
				label[row][col][k].getText().setText(format.format(  ((Double)(v.elementAt(k))).doubleValue()  ));
			}
			refresh();			
		}
		
		public void aggiornaQ(int i, int j, int k, double d) {
			if(i<0)return;
			if(j<0)return;
			
			label[i][j][k].getText().setText(format.format(d));
			refresh();
		}
		
		public void resetObjectsPlaced(){
			for(int i=0;i<gridx.length;i++){
				removeSegment(gridx[i]);
			}
			for(int i=0;i<gridy.length;i++){
				removeSegment(gridy[i]);				
			}
			for(int i=0;i<label.length;i++){
				for(int j=0;j<label[i].length;j++){
					for(int k=0;k<label[i][j].length;k++){
						label[i][j][k].removeImages();
						removeSegment(label[i][j][k]);
					}
				}
			}
			
			Iterator i=banane.iterator();
			while(i.hasNext()){
				GSegment g=(GSegment)i.next();
				g.removeImages();
				removeSegment(g);
			}

			i=palme.iterator();
			while(i.hasNext()){
				GSegment g=(GSegment)i.next();
				g.removeImages();
				removeSegment(g);
			}
			
			creaGriglia();
			
		}

		private void creaGriglia(){
			scimmia=null;
			
			GStyle gridStyle = new GStyle();
			gridStyle.setForegroundColor (new Color (0, 0,0));
			gridStyle.setLineWidth (2);
			gridx = new GSegment[Stato.numCol+1];
			gridy = new GSegment[Stato.numRow+1];
			
			for (int i = 0; i < Stato.numCol+1; i++) {
				gridx[i] = new GSegment();
				gridx[i].setStyle (gridStyle);
				addSegment (gridx[i]);
			}
			for (int i = 0; i < Stato.numRow+1; i++) {
				gridy[i] = new GSegment();
				gridy[i].setStyle (gridStyle);
				addSegment (gridy[i]);
			}
			
			GStyle labelStyle = new GStyle();
			labelStyle.setLineStyle(GStyle.LINESTYLE_INVISIBLE);
			label=new GSegment[Stato.numRow][Stato.numCol][Stato.numAzioni];
			for(int i=0;i<Stato.numRow;i++){
				for(int j=0;j<Stato.numCol;j++){
					for(int k=0;k<Stato.numAzioni;k++){
						label[i][j][k]=new GSegment();
						label[i][j][k].setStyle(labelStyle);
						GText s=null;
						if(k==0){
							s=new GText(format.format(0),GPosition.TOP);
						}
						else if(k==2){
							s=new GText(format.format(0),GPosition.BOTTOM);
						}
						else if(k==3){
							s=new GText(format.format(0),GPosition.RIGHT);
						}
						else if(k==1){
							s=new GText(format.format(0),GPosition.LEFT);
						}
						label[i][j][k].setText(s);
						addSegment(label[i][j][k]);
					}					
				}
			}
			
			
			banane=new ArrayList();
			palme=new ArrayList();

		}
	}

	public void resetModeUse() {
		enableButtonsDown(true);
		cmdUse.setText("Use!");
		modeUse=0;
	}

	private void saveWorld(String path) throws FileNotFoundException, IOException{
		ObjectOutputStream saveFile;
		saveFile = new ObjectOutputStream(new FileOutputStream(path));
		saveFile.writeObject(settings);
		saveFile.writeObject(new Integer(nBanane));
		for(int i=0;i<bananePositionX.length;i++){
			saveFile.writeObject(new Double(bananePositionX[i]));
			saveFile.writeObject(new Double(bananePositionY[i]));
		}
		saveFile.writeObject(new Integer(nPalme));
		for(int i=0;i<palmePositionX.length;i++){
			saveFile.writeObject(new Double(palmePositionX[i]));
			saveFile.writeObject(new Double(palmePositionY[i]));			
		}
		saveFile.flush();
		saveFile.close();
	}
	
	private void loadWorld(String path) throws FileNotFoundException, IOException, ClassNotFoundException{
		ObjectInputStream loadFile;
		loadFile = new ObjectInputStream (new FileInputStream(path));
		settings=(Settings)(loadFile.readObject());
		nBanane=((Integer)(loadFile.readObject())).intValue();
		bananePositionX=new double[nBanane];
		bananePositionY=new double[nBanane];
		for(int i=0;i<bananePositionX.length;i++){
			bananePositionX[i]=((Double)loadFile.readObject()).doubleValue();
			bananePositionY[i]=((Double)loadFile.readObject()).doubleValue();
		}
		nPalme=((Integer)(loadFile.readObject())).intValue();
		palmePositionX=new double[nPalme];
		palmePositionY=new double[nPalme];		
		for(int i=0;i<palmePositionX.length;i++){
			palmePositionX[i]=((Double)loadFile.readObject()).doubleValue();
			palmePositionY[i]=((Double)loadFile.readObject()).doubleValue();
		}
		loadFile.close();
	}
}
