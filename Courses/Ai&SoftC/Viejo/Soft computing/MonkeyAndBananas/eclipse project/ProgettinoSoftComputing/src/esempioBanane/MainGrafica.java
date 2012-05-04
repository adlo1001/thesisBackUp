package esempioBanane;

import esempioBanane.grafica.FrameMain;

public class MainGrafica {

	public static void main(String[] args) {
		FrameMain fr=new FrameMain();
		fr.setVisible(true);
		fr.enableAllButtons(true);
		fr.disablePause();
	}

}
