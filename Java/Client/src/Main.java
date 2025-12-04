import java.io.*;
import java.net.*;
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
      final String Server_ip="192.168.60.159";
      final int Server_port=12345;
      try {
          Socket socket = new Socket(Server_ip, Server_port);
          System.out.println("Connesso al Server...");
          PrintWriter writer = new PrintWriter(socket.getOutputStream(), true);
          BufferedReader in=new BufferedReader (new InputStreamReader(socket.getInputStream()));
          Scanner scanner1 = new Scanner(System.in);
          System.out.println("Inserisci una stringa: ");
          String msg=scanner1.nextLine();
          writer.println(msg);
          String risposta=in.readLine();
          System.out.println("Risposta dal server "+risposta);
          socket.close();
      }catch(IOException e){
          e.printStackTrace();
      }
    }
}