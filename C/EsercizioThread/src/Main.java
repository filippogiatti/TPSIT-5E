import java.util.Random;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Inserisci il numero di elementi del vettore: ");
        int nElementi = scanner.nextInt();

        System.out.print("Inserisci il numero di thread da utilizzare: ");
        int nThread = scanner.nextInt();

        System.out.print("Inserisci l'elemento da ricercare: ");
        int ricerca = scanner.nextInt();

        int[] vettore = new int[nElementi];
        Random random = new Random();

        // Riempimento del vettore con numeri casuali
        for (int i = 0; i < nElementi; i++) {
            vettore[i] = random.nextInt(1000); // valori tra 0 e 999
        }

        Search[] threads = new Search[nThread];
        int intervallo = nElementi / nThread;

        long tempoIniziale = System.currentTimeMillis();

        // Creazione e avvio dei thread
        for (int i = 0; i < nThread; i++) {
            int a = i * intervallo;
            int b = a + intervallo;

            // Ultimo thread prende gli eventuali elementi rimanenti
            if (b > nElementi) {
                b = nElementi;
            }

            threads[i] = new Search(vettore, a, b,ricerca,0);
            threads[i].start();
        }

        // Attesa della fine di tutti i thread e conteggio totale
        int totaleOccorrenze = 0;
        for (int i = 0; i < nThread; i++) {
            try {
                threads[i].join();
                totaleOccorrenze += threads[i].conteggio();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        long tempoFinale = System.currentTimeMillis();

        System.out.println("\nNumero totale di occorrenze di " + ricerca + ": " + totaleOccorrenze);
        System.out.println("Tempo di esecuzione: " + (tempoFinale - tempoIniziale) + " ms");

       /*("All’aumentare del numero di thread, il tempo di esecuzione può diminuire, "
                + "poiché il lavoro viene suddiviso tra più core. Tuttavia, se il numero di thread è troppo alto "
                + "rispetto al numero di core disponibili, l’overhead di gestione può rallentare il programma.");
        */
    }
}
