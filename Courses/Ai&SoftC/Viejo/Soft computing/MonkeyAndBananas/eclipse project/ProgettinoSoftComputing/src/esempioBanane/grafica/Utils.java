/*
 * Created on 20-apr-2006
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package esempioBanane.grafica;


import java.awt.GridBagConstraints;

/*
 * DaneseCeriani::
 */
/**
 * Classe statica con alcuni metodi di utilità
 */
public class Utils {
	/**
	 * Metodo di utilità per l'utilizzo di GridBagConstraint
	 */
	public static void impostaLimite(GridBagConstraints gbc, int gx, int gy, int gw, int gh,double wx, double wy)
	{
		gbc.gridx = gx;
		gbc.gridy = gy;
		gbc.gridwidth = gw;
		gbc.gridheight = gh;
		gbc.weightx = wx;
		gbc.weighty = wy;
	}


}
