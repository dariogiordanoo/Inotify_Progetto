// Librerie
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/inotify.h>
#include <limits.h>

#define MAX_PATH_LENGTH 256
#define MAX_FILENAME_LENGTH 100
#define BUFFER_SIZE 1024

// Sequenze ANSI per i colori
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define RED     "\033[0;31m"
#define RESET   "\033[0m"

// Funzione per creare un file
void create_file() {
    char filename[MAX_FILENAME_LENGTH];
    printf(GREEN"Inserisci il nome del file da creare: "RESET);
    if (scanf("%99s", filename) != 1) {
        printf(RED"Input non valido. Operazione annullata.\n"RESET);
        while (getchar() != '\n'); // Svuota buffer
        return;
    }
    getchar(); // Consuma il newline residuo

    FILE *fp = fopen(filename, "w");
    if (fp != NULL) {
        printf(YELLOW"File '%s' creato con successo.\n"RESET, filename);
        fclose(fp);
    } else {
        printf(RED"Non è stato possibile generare il file '%s'.\n"RESET, filename);
        perror(RED"Errore"RESET);
    }
}

// Funzione per scrivere/modificare un file
void write_file() {
    char filename[MAX_FILENAME_LENGTH];
    char content[BUFFER_SIZE];
    printf(GREEN"Inserisci il nome del file da aggiornare: "RESET);
    if (scanf("%99s", filename) != 1) {
        printf(RED"Input non valido. Operazione annullata.\n"RESET);
        while (getchar() != '\n');
        return;
    }
    getchar(); // Pulisce newline

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf(RED"Impossibile aprire il file '%s' per la scrittura.\n"RESET, filename);
        perror(RED"Errore"RESET);
        return;
    }

    printf(GREEN"Inserisci il contenuto da salvare (CTRL+D per terminare):\n"RESET);
    while (fgets(content, sizeof(content), stdin) != NULL) {
        fputs(content, fp);
    }

    if (ferror(stdin)) {
        printf(RED"\nErrore durante la lettura dell'input.\n"RESET);
    } else {
        printf(YELLOW"\nContenuto scritto con successo nel file '%s'.\n"RESET, filename);
    }

    fclose(fp);
}

// Funzione per visualizzare il contenuto di un file
void view_file() {
    char filename[MAX_FILENAME_LENGTH];
    char buffer[BUFFER_SIZE];
    printf(GREEN"Inserisci il nome del file da consultare: "RESET);
    if (scanf("%99s", filename) != 1) {
        printf(RED"Input non valido. Operazione interrotta.\n"RESET);
        while (getchar() != '\n');
        return;
    }
    getchar(); // Consuma newline

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf(RED"Impossibile aprire il file '%s' in lettura.\n"RESET, filename);
        perror(RED"Errore"RESET);
        return;
    }

    printf(YELLOW"\n--- Contenuto del file '%s' ---\n"RESET, filename);
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        fputs(buffer, stdout);
    }

    if (ferror(fp)) {
        printf(RED"\nErrore durante la lettura del file.\n"RESET);
    }

    printf(YELLOW"\n--------------------------------\n"RESET);
    fclose(fp);
}
