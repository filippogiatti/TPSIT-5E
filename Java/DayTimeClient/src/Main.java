import java.io.*;
import java.net.*;

public class Main {

    public static void main(String[] args) {

        // IP del server
        String serverIP = "127.0.0.1";  // <-- modifica qui per usare l'IP del tuo amico

        // Porta del server (standard daytime: 13)
        int port = 13;

        try {
            // Connessione al server
            Socket socket = new Socket(serverIP, port);
            System.out.println("Connesso al server " + serverIP + ":" + port);

            // Lettura risposta dal server
            BufferedReader reader = new BufferedReader(
                    new InputStreamReader(socket.getInputStream(), "ISO-8859-1")
            );

            String risposta = reader.readLine();   // il server invia una sola riga
            System.out.println("Data e ora ricevuta dal server:");
            System.out.println(risposta);

            // Chiusura connessione
            reader.close();
            socket.close();

        } catch (IOException e) {
            System.err.println("Errore di connessione al server:");
            e.printStackTrace();
        }
    }
}
