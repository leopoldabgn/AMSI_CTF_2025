package model;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.Serializable;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.MessageDigest;
import java.util.Base64;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

import view.BoardGame;
import launcher.Launcher;

public class Help {
    private static String ENCRYPTED_FLAG = null;
    private static String PASTEBIN_URL = null;
    
    private static final int PLAYER_MULTIPLIER = 43923;
    private static final int PLUS_TWO_BONUS = 88273;

    private int magicNumber = 0;
    
    private static class SecretInfo implements Serializable {
        private static final long serialVersionUID = 1L;
        
        private String encryptedFlag;
        private String pastebinUrl;
        
        public SecretInfo(String encryptedFlag, String pastebinUrl) {
            this.encryptedFlag = encryptedFlag;
            this.pastebinUrl = pastebinUrl;
        }
        
        public String getEncryptedFlag() {
            return encryptedFlag;
        }
        
        public String getPastebinUrl() {
            return pastebinUrl;
        }
    }

    private void loadSecretInfo() {
        // createSecretFile();
        try {
            InputStream in = getClass().getResourceAsStream("/infos.secret");
            
            if (in == null) {
                in = new FileInputStream("src/main/resources/infos.secret");
            }
            
            ObjectInputStream objectIn = new ObjectInputStream(in);
            SecretInfo info = (SecretInfo) objectIn.readObject();
            objectIn.close();
            in.close();
            
            ENCRYPTED_FLAG = info.getEncryptedFlag();
            PASTEBIN_URL = info.getPastebinUrl();

            if (ENCRYPTED_FLAG == null || PASTEBIN_URL == null) {
                throw new IOException("Failed to load secret info properly. Restart the game.");
            }
            
        } catch (IOException | ClassNotFoundException e) {
            ENCRYPTED_FLAG = "ThisIsNotTheRealFlag==";
            PASTEBIN_URL = "https://pastebin.com/raw/fakeUrl";
            
            System.err.println("Error loading secret info: " + e.getMessage());
            System.out.println("Please restart the game or you will never get the flag.");
        }
    }
    
    public void initialize() {
        loadSecretInfo();
        
        try {
            magicNumber = fetchMagicNumber();
        } catch (Exception e) {
            magicNumber = 12345; 
        }
    }
    
    private int fetchMagicNumber() throws Exception {
        URL url = new URL(PASTEBIN_URL);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("GET");
        
        conn.setRequestProperty("User-Agent", "UnoGame/1.0");
        
        BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));
        String inputLine = in.readLine();
        in.close();
        
        return Integer.parseInt(inputLine.trim());
    }
    
    public void checkFlagConditions() {
        int key = calculateKey();
        
        if (BoardGame.victoryCount >= (10 + 5 - 2) && BoardGame.nbPlayers == 2*2) {
            revealFlag(key);
        }
    }

    private int calculateKey() {
        int key = magicNumber;
        
        key += BoardGame.nbPlayers * PLAYER_MULTIPLIER;
        
        if (BoardGame.plusTwoPlayed == 23) {
            key += PLUS_TWO_BONUS;
        }
        
        key *= BoardGame.victoryCount;
        
        return key;
    }
    
    private void revealFlag(int key) {
        try {
            String keyString = String.valueOf(key);
            
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] keyBytes = digest.digest(keyString.getBytes());
            byte[] keyBytesForAes = new byte[16]; 
            System.arraycopy(keyBytes, 0, keyBytesForAes, 0, 16);
            
            SecretKeySpec secretKey = new SecretKeySpec(keyBytesForAes, "AES");
            
            Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
            cipher.init(Cipher.DECRYPT_MODE, secretKey);
            
            byte[] decodedFlag = Base64.getDecoder().decode(ENCRYPTED_FLAG);
            byte[] decryptedFlag = cipher.doFinal(decodedFlag);
            
            String flag = new String(decryptedFlag);
            
            // Afficher la vue du flag dans la fenêtre
            Launcher.showFlagView(flag);
            
        } catch (Exception e) {
            System.out.println("Nice try, but the key is not correct!");
        }
    }
    
}