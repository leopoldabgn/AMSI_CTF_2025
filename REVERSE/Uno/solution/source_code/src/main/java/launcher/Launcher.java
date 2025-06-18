package launcher;

import model.Help;
import view.Window;

public class Launcher
{
    /*
     +4 ne fonctionne pas + changement de couleur
     Enlever bouton GO.
     */
    private static Window window;
    
    public static void main(String[] args)
    {
        Help help = new Help();
        help.initialize();
        GameState.sethelp(help);
        window = new Window(1000, 650);
    }
    
    public static void showFlagView(String flag) {
        if (window != null) {
            window.setHelpView(flag);
        }
    }
    
    public static class GameState {
        private static Help help;
        
        public static void sethelp(Help system) {
            help = system;
        }
        
        public static Help gethelp() {
            return help;
        }
    }
}