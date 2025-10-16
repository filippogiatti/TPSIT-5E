/*Scrivere un programma in C che dopo aver dichiarato una struttura denominata Persona con i
seguenti campi: Nome, Cognome, età, codice fiscale, reddito annuo, anno_nascita,residenza(città) ;
determini:
1) La persona con reddito annuo più alto e quella con reddito annuo più basso.Visualizzare
per entrambe: nome, cognome, città e reddito.
2) Ordinare la lista delle persone in base all’anno di nascita** (dal più anziano al più
giovane), e visualizzarla.
3) Consentire all’utente di cercare una persona inserendo il codice fiscale..
4) Visualizzare tutte le persone con reddito superiore o uguale a un valore scelto dall’utente.
5) Calcolare la media dei redditi delle persone registrate nel sistema.
6) Visualizzare tutte le persone nate in un anno specifico, scelto dall’utente;
7) Visualizzare l'elenco delle persone raggruppate per città di residenza, indicando per
ciascuna città: Quante persone vi risiedono, L’età media dei residenti*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PERSONE 100
#define STRLEN 100

struct Persona {
    char nome[STRLEN];
    char cognome[STRLEN];
    int eta;
    char codice_fiscale[STRLEN];
    double reddito_annuo;
    int anno_nascita;
    char residenza[STRLEN];
};

static int cmp_anno(const void *a, const void *b) {
    const struct Persona *pa = a;
    const struct Persona *pb = b;
    return pa->anno_nascita - pb->anno_nascita; /* anziano (anno piccolo) -> giovane (anno grande) */
}

static void stampa_persona(const struct Persona *p) {
    printf("Nome: %s\n", p->nome);
    printf("Cognome: %s\n", p->cognome);
    printf("Eta': %d\n", p->eta);
    printf("Codice fiscale: %s\n", p->codice_fiscale);
    printf("Reddito annuo: %.2f\n", p->reddito_annuo);
    printf("Anno di nascita: %d\n", p->anno_nascita);
    printf("Residenza: %s\n", p->residenza);
}

int main(void) {
    int n;
    struct Persona p[MAX_PERSONE];

    printf("Quante persone vuoi inserire? ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PERSONE) {
        printf("Numero non valido.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("\n--- Persona %d ---\n", i + 1);

        /* uso scanset con width per evitare overflow e per permettere spazi nei campi */
        printf("Nome: ");
        if (scanf(" %99[^\n]", p[i].nome) != 1) p[i].nome[0] = '\0';

        printf("Cognome: ");
        if (scanf(" %99[^\n]", p[i].cognome) != 1) p[i].cognome[0] = '\0';

        printf("Eta': ");
        if (scanf("%d", &p[i].eta) != 1) {
            p[i].eta = 0;
            /* svuota linea residua se input invalido */
            int c; while ((c = getchar()) != '\n' && c != EOF) { }
        }

        printf("Codice fiscale: ");
        if (scanf(" %99s", p[i].codice_fiscale) != 1) p[i].codice_fiscale[0] = '\0';

        printf("Reddito annuo: ");
        if (scanf("%lf", &p[i].reddito_annuo) != 1) {
            p[i].reddito_annuo = 0.0;
            int c; while ((c = getchar()) != '\n' && c != EOF) { }
        }

        printf("Anno di nascita: ");
        if (scanf("%d", &p[i].anno_nascita) != 1) {
            p[i].anno_nascita = 0;
            int c; while ((c = getchar()) != '\n' && c != EOF) { }
        }

        printf("Residenza (citta'): ");
        if (scanf(" %99[^\n]", p[i].residenza) != 1) p[i].residenza[0] = '\0';
    }

    /* 1) persona con reddito max e min */
    int idx_max = 0, idx_min = 0;
    for (int i = 1; i < n; i++) {
        if (p[i].reddito_annuo > p[idx_max].reddito_annuo) idx_max = i;
        if (p[i].reddito_annuo < p[idx_min].reddito_annuo) idx_min = i;
    }

    printf("\n--- Reddito massimo ---\n");
    printf("Nome: %s\nCognome: %s\nCitta: %s\nReddito: %.2f\n",
           p[idx_max].nome, p[idx_max].cognome, p[idx_max].residenza, p[idx_max].reddito_annuo);

    printf("\n--- Reddito minimo ---\n");
    printf("Nome: %s\nCognome: %s\nCitta: %s\nReddito: %.2f\n",
           p[idx_min].nome, p[idx_min].cognome, p[idx_min].residenza, p[idx_min].reddito_annuo);

    /* 2) ordinare per anno di nascita (dal più anziano al più giovane) e visualizzare */
    qsort(p, n, sizeof(struct Persona), cmp_anno);
    printf("\n--- Elenco ordinato per anno di nascita (anziano -> giovane) ---\n");
    for (int i = 0; i < n; i++) {
        printf("\nPersona %d:\n", i + 1);
        stampa_persona(&p[i]);
    }

    /* 3) cercare persona per codice fiscale */
    char codice_ricerca[STRLEN];
    printf("\nInserisci codice fiscale da cercare: ");
    if (scanf(" %99s", codice_ricerca) == 1) {
        int trovato = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(p[i].codice_fiscale, codice_ricerca) == 0) {
                printf("\nPersona trovata:\n");
                stampa_persona(&p[i]);
                trovato = 1;
                break;
            }
        }
        if (!trovato) printf("Nessuna persona trovata con codice fiscale %s\n", codice_ricerca);
    } else {
        printf("Ricerca annullata o input non valido.\n");
    }

    /* 4) visualizzare persone con reddito >= valore scelto dall'utente */
    double soglia;
    printf("\nInserisci valore di reddito soglia: ");
    if (scanf("%lf", &soglia) == 1) {
        int count = 0;
        printf("\nPersone con reddito >= %.2f:\n", soglia);
        for (int i = 0; i < n; i++) {
            if (p[i].reddito_annuo >= soglia) {
                printf("\nPersona %d:\n", i + 1);
                stampa_persona(&p[i]);
                count++;
            }
        }
        if (count == 0) printf("Nessuna persona ha reddito >= %.2f\n", soglia);
    } else {
        printf("Valore soglia non valido.\n");
    }

    /* 5) calcolare la media dei redditi */
    double somma = 0.0;
    for (int i = 0; i < n; i++) somma += p[i].reddito_annuo;
    double media = (n > 0) ? (somma / n) : 0.0;
    printf("\nMedia dei redditi: %.2f\n", media);

    return 0;
}
