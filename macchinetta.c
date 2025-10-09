/* 
   Simulazione macchina del caffè in C
   Requisiti:
   - Caldaia che si riscalda all'accensione
   - Menu bevande (nome, ml)
   - Controlli: caldaia pronta, acqua >= 1.0 L, serbatoio cialde non pieno
   - Erogazione a 5 ml/s con countdown/progress bar testuale
   - Aggiornamento acqua e cialde usate
   - Blocco se serbatoio cialde pieno; possibilità di svuotarlo
   - Termina quando utente scrive "fine"
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  
#include <ctype.h>
#include <math.h>

#define MAX_BEV 10
#define NAME_LEN 40

typedef struct {
    char nome[NAME_LEN];
    int ml;
} Bevanda;

typedef struct {
    int caldaiaCalda;    // 0 = no, 1 = pronta
    double acqua;        // litri disponibili
    int cialdeUsate;
    int cialdeMax;
} MacchinaCaffe;

/* Prototipi */
void initMenu(Bevanda menu[], int *n);
void accendiMacchina(MacchinaCaffe *m);
void mostraMenu(Bevanda menu[], int n);
void mostraStato(MacchinaCaffe *m);
int controlliOk(MacchinaCaffe *m);
void preparaBevanda(MacchinaCaffe *m, Bevanda b);
void countdownProgress(int secondi);
void svuotaCialde(MacchinaCaffe *m);
void trim_newline(char *s);
void toLowerCase(char *s);

int main() {
    Bevanda menu[MAX_BEV];
    int nBev = 0;
    initMenu(menu, &nBev);

    MacchinaCaffe macchina = {0, 2.0, 0, 5}; // caldaia spenta, 2.0 L, 0 cialde usate, capacità 5
    char input[100];

    printf("=== Simulazione Macchina del Caffè ===\n");
    printf("Comandi: inserisci il numero della bevanda, 'stato', 'svuota', 'fine'\n\n");

    accendiMacchina(&macchina);

    while (1) {
        mostraMenu(menu, nBev);
        printf("\n> Inserisci scelta (numero | stato | svuota | fine): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nInput non valido. Esco.\n");
            break;
        }
        trim_newline(input);
        toLowerCase(input);

        if (strcmp(input, "fine") == 0) {
            printf("Terminazione richiesta. Arrivederci!\n");
            break;
        } else if (strcmp(input, "stato") == 0) {
            mostraStato(&macchina);
            continue;
        } else if (strcmp(input, "svuota") == 0) {
            svuotaCialde(&macchina);
            continue;
        }

        // tentativo di interpretare come numero
        char *endptr;
        long scelta = strtol(input, &endptr, 10);
        if (endptr == input || scelta < 1 || scelta > nBev) {
            printf("Scelta non valida. Riprova.\n");
            continue;
        }

        Bevanda sceltaBev = menu[scelta - 1];

        // controlli preliminari (nella specifica: acqua sufficiente >=1L, caldaia calda, serbatoio non pieno)
        if (!controlliOk(&macchina)) {
            // messaggi già forniti dalla funzione controlliOk
            continue;
        }

        // preparazione (erogazione)
        preparaBevanda(&macchina, sceltaBev);

        // dopo preparazione controlliamo se serbatoio è pieno e blocchiamo
        if (macchina.cialdeUsate >= macchina.cialdeMax) {
            printf("Attenzione: il serbatoio cialde è pieno. Per continuare svuotalo con 'svuota'.\n");
        }
    }

    return 0;
}

/* Inizializza un menu di esempio */
void initMenu(Bevanda menu[], int *n) {
    *n = 3;
    strncpy(menu[0].nome, "Espresso", NAME_LEN);
    menu[0].ml = 40;
    strncpy(menu[1].nome, "Lungo", NAME_LEN);
    menu[1].ml = 80;
    strncpy(menu[2].nome, "Tazza Grande", NAME_LEN);
    menu[2].ml = 150;
}

/* Simula accensione e riscaldamento caldaia */
void accendiMacchina(MacchinaCaffe *m) {
    printf("Accensione macchina...\n");
    sleep(1);
    printf("Riscaldamento caldaia in corso...\n");
    // simuliamo tempo di riscaldamento
    sleep(2);
    m->caldaiaCalda = 1;
    printf("Caldaia pronta!\n\n");
}

void mostraMenu(Bevanda menu[], int n) {
    printf("----- MENU -----\n");
    for (int i = 0; i < n; i++) {
        printf("%d) %s (%d ml)\n", i + 1, menu[i].nome, menu[i].ml);
    }
}

/* Mostra stato macchina */
void mostraStato(MacchinaCaffe *m) {
    printf("\n--- STATO MACCHINA ---\n");
    printf("Caldaia: %s\n", m->caldaiaCalda ? "PRONTA" : "NON PRONTA");
    printf("Acqua disponibile: %.2f L\n", m->acqua);
    printf("Cialde usate: %d / %d\n", m->cialdeUsate, m->cialdeMax);
    printf("----------------------\n\n");
}

/* Controlli: caldaia pronta, acqua >= 1L, serbatoio non pieno */
int controlliOk(MacchinaCaffe *m) {
    if (!m->caldaiaCalda) {
        printf("Errore: caldaia non pronta!\n");
        return 0;
    }
    if (m->acqua < 1.0 - 1e-9) {
        printf("Errore: acqua insufficiente (serve almeno 1.0 L). Stato attuale: %.2f L\n", m->acqua);
        return 0;
    }
    if (m->cialdeUsate >= m->cialdeMax) {
        printf("Errore: il serbatoio cialde è pieno. Svuotarlo prima di procedere.\n");
        return 0;
    }
    return 1;
}

/* Preparazione bevanda:
   - calcola tempo = ceil( ml / 5 )
   - mostra countdown/progress bar
   - decrementa acqua e incrementa cialdeUsate
*/
void preparaBevanda(MacchinaCaffe *m, Bevanda b) {
    printf("\nPreparazione: %s (%d ml)\n", b.nome, b.ml);

    // tempo in secondi (erogazione 5 ml/s). usiamo ceil per coprire resti
    int tempo = (b.ml + 4) / 5; 
    countdownProgress(tempo);

    // Aggiornamento stato
    double consumo = (double)b.ml / 1000.0;
    if (consumo > m->acqua) {
        printf("Attenzione: acqua non sufficiente per completare la bevanda. Preparazione interrotta.\n");
        return;
    }
    m->acqua -= consumo;
    m->cialdeUsate++;

    printf("Bevanda pronta! Goditi il tuo %s.\n", b.nome);
    printf("Acqua rimanente: %.2f L | Cialde usate: %d/%d\n\n", m->acqua, m->cialdeUsate, m->cialdeMax);
}

/* Countdown testuale con progress bar */
void countdownProgress(int secondi) {
    int width = 30; // larghezza barra
    for (int t = secondi; t > 0; t--) {
        int elapsed = secondi - t;
        int filled = (int)((double)elapsed / secondi * width);
        if (filled < 0) filled = 0;
        if (filled > width) filled = width;

        // costruzione barra
        printf("\r[");
        for (int i = 0; i < filled; i++) putchar('=');
        for (int i = filled; i < width; i++) putchar(' ');
        printf("] %2d s rimanenti  ", t);
        fflush(stdout);
        sleep(1);
    }
    // Fine
    printf("\r[");
    for (int i = 0; i < width; i++) putchar('=');
    printf("]  0 s rimanenti\n");
}

/* Svuota serbatoio cialde */
void svuotaCialde(MacchinaCaffe *m) {
    m->cialdeUsate = 0;
    printf("Serbatoio cialde svuotato. Puoi continuare a preparare bevande.\n\n");
}

/* Rimuove newline finale da fgets */
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len - 1] == '\n') s[len - 1] = '\0';
}

/* Converte in minuscolo */
void toLowerCase(char *s) {
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}
