package view;

import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class HelpView extends JPanel {
    private static final long serialVersionUID = 1L;
    private Window window;
    
    public HelpView(Window window, String flag) {
        this.window = window;
        setOpaque(false);
        initComponents(flag);
    }
    
    private void initComponents(String flag) {
        setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        
        // Label titre
        JLabel titleLabel = new JLabel("FÉLICITATIONS !");
        titleLabel.setFont(new Font("Arial", Font.BOLD, 36));
        titleLabel.setForeground(Color.YELLOW);
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);
        
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 1;
        gbc.anchor = GridBagConstraints.CENTER;
        gbc.insets = new Insets(10, 10, 30, 10);
        add(titleLabel, gbc);
        
        // Panel pour le flag
        JPanel flagPanel = new JPanel();
        flagPanel.setOpaque(true);
        flagPanel.setBackground(new Color(0, 0, 0, 180));
        flagPanel.setBorder(BorderFactory.createLineBorder(Color.WHITE, 3));
        flagPanel.setLayout(new GridBagLayout());
        
        // Label du flag
        JLabel flagLabel = new JLabel("Vous avez trouvé le flag:");
        flagLabel.setFont(new Font("Arial", Font.BOLD, 20));
        flagLabel.setForeground(Color.WHITE);
        
        GridBagConstraints flagGbc = new GridBagConstraints();
        flagGbc.gridx = 0;
        flagGbc.gridy = 0;
        flagGbc.anchor = GridBagConstraints.CENTER;
        flagGbc.insets = new Insets(20, 20, 10, 20);
        flagPanel.add(flagLabel, flagGbc);
        
        // Valeur du flag
        JLabel flagValueLabel = new JLabel(flag);
        flagValueLabel.setFont(new Font("Monospaced", Font.BOLD, 24));
        flagValueLabel.setForeground(new Color(0, 255, 0));
        
        flagGbc.gridx = 0;
        flagGbc.gridy = 1;
        flagGbc.insets = new Insets(10, 40, 30, 40);
        flagPanel.add(flagValueLabel, flagGbc);
        
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.insets = new Insets(10, 10, 30, 10);
        add(flagPanel, gbc);
        
        // Bouton retour menu
        JButton menuButton = new JButton("Retour au menu");
        menuButton.setFont(new Font("Arial", Font.BOLD, 16));
        menuButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                window.setHomeView();
            }
        });
        
        gbc.gridx = 0;
        gbc.gridy = 2;
        gbc.insets = new Insets(20, 10, 10, 10);
        add(menuButton, gbc);
        
        // Bouton quitter
        JButton quitButton = new JButton("Quitter");
        quitButton.setFont(new Font("Arial", Font.BOLD, 16));
        quitButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                window.quit();
            }
        });
        
        gbc.gridx = 0;
        gbc.gridy = 3;
        gbc.insets = new Insets(10, 10, 10, 10);
        add(quitButton, gbc);
    }
}