import java.net.*;
import java.io.*;


public class Main {
    public static void main(String[] args) {
        final int PORT = 12345;
        try(ServerSocket serverSocket=new ServerSocket(PORT)){
            System.out.println("Server in ascolto sulla porta "+PORT);
            //accettazione connessione al client
            Socket clientSocket = serverSocket.accept();

            PrintWriter writer = new PrintWriter(clientSocket.getOutputStream(), true);
            BufferedReader in=new BufferedReader (new InputStreamReader(clientSocket.getInputStream()));
            //legge dalla socket
            String ricevuto=in.readLine();
            System.out.println("Ricevuto dal client: "+ricevuto+" "+clientSocket.getRemoteSocketAddress());
            //risponde al client
            writer.println("Messaggio ricevuto");


        }catch (IOException e){
            System.out.println(e.getMessage());
        }

    }
}