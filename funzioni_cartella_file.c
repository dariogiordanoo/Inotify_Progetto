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

// Funzione per pulire il buffer di input
void flush_input() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Funzione per eliminare un file o una directory vuota
void delete_item() {
    char item_name[MAX_FILENAME_LENGTH];

    printf(GREEN"Inserisci il nome del file o della cartella da cancellare: "RESET);
    if (scanf("%99s", item_name) != 1) {
        printf(RED"Errore durante l'acquisizione del nome.\n"RESET);
        flush_input();
        return;
    }
    flush_input();

    struct stat st;
    if (stat(item_name, &st) == -1) {
        printf(RED"Non è stato possibile accedere a '%s'.\n"RESET, item_name);
        perror(RED"Descrizione errore"RESET);
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        if (rmdir(item_name) == 0) {
            printf(YELLOW"Cartella '%s' rimossa con successo.\n"RESET, item_name);
        } else {
            printf(RED"Errore: la cartella '%s' non è stata eliminata. È vuota?\n"RESET, item_name);
            perror(RED"Dettaglio"RESET);
        }
    } else if (S_ISREG(st.st_mode)) {
        if (unlink(item_name) == 0) {
            printf(YELLOW"File '%s' eliminato correttamente.\n"RESET, item_name);
        } else {
            printf(RED"Errore durante la rimozione del file '%s'.\n"RESET, item_name);
            perror(RED"Dettaglio"RESET);
        }
    } else {
        printf(RED"Il tipo di elemento '%s' non è supportato per l'eliminazione.\n"RESET, item_name);
    }
}

// Funzione per visualizzare informazioni su file/directory
void show_file_info() {
    char item_name[MAX_FILENAME_LENGTH];

    printf(GREEN"Inserisci il nome del file o della cartella da analizzare: "RESET);
    if (scanf("%99s", item_name) != 1) {
        printf(RED"Errore durante la lettura del nome.\n"RESET);
        flush_input();
        return;
    }
    flush_input();

    struct stat file_stat;
    if (stat(item_name, &file_stat) == -1) {
        printf(RED"Impossibile ottenere dettagli per '%s'.\n"RESET, item_name);
        perror(RED"Motivo"RESET);
        return;
    }

    printf(YELLOW"\n-- Dettagli su '%s' --\n"RESET, item_name);
    printf("  ▸ Dimensione: %ld byte\n", file_stat.st_size);
    printf("  ▸ Tipo: ");
    if (S_ISREG(file_stat.st_mode)) {
        printf("File ordinario\n");
    } else if (S_ISDIR(file_stat.st_mode)) {
        printf("Cartella\n");
    } else {
        printf("Altro\n");
    }
    printf("  ▸ Permessi (ottali): %o\n", file_stat.st_mode & 0777);
    printf("  ▸ Ultimo accesso: %s", ctime(&file_stat.st_atime));
    printf("  ▸ Ultima modifica: %s", ctime(&file_stat.st_mtime));
    printf("  ▸ Ultimo aggiornamento dei metadati: %s", ctime(&file_stat.st_ctime));
}
