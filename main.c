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

// Librerie personalizzate
#include "funzioni_cartella_file.h"
#include "funzioni_cartella.h"
#include "funzioni_file.h"
#include "funzioni_inotify.h"

// Definizione costanti
#define MAX_PATH_LENGTH 256
#define MAX_FILENAME_LENGTH 100
#define BUFFER_SIZE 1024

// Sequenze ANSI per i colori
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define RED     "\033[0;31m"
#define RESET   "\033[0m"

// Funzione per stampare il menu
void print_menu() {
    printf(GREEN "\n=== Gestione Avanzata File ===\n" RESET);
    printf(YELLOW "a.  Mostra contenuto directory\n");
    printf("b.  Cambia cartella attiva\n");
    printf("c.  Crea un nuovo file\n");
    printf("d.  Modifica o scrivi su file\n");
    printf("e.  Visualizza contenuto file\n");
    printf("f.  Rimuovi file o directory vuota\n");
    printf("g.  Crea una nuova directory\n");
    printf("h.  Mostra dettagli file/directory\n");
    printf("i.  Avvia monitoraggio modifiche\n");
    printf("j.  Pulisci lo schermo\n");
    printf("x.  Termina programma\n");
    printf("-------------------------------\n" RESET);
    printf(GREEN "\nSeleziona una voce dal menu: " RESET);
}

// Funzione per pulire lo schermo
void clear_screen() {
    printf("\033[2J\033[H");
}

int main() {
    char choice;
    char initial_path[MAX_PATH_LENGTH]; 

    // Ottiene la directory iniziale di lavoro
    if (getcwd(initial_path, sizeof(initial_path)) == NULL) {
        fprintf(stderr, RED "Impossibile ottenere la directory corrente.\n" RESET);
        perror(RED "Errore" RESET);
        return EXIT_FAILURE;
    }

    printf(GREEN "Programma avviato nella cartella: %s\n" RESET, initial_path);

    // Loop principale del menu
    do {
        print_menu();

        choice = getchar();
        while (getchar() != '\n');  // Pulisce il buffer da input extra

        switch (choice) {
            case 'a':
                list_directory(".");
                break;
            case 'b':
                change_directory();
                break;
            case 'c':
                create_file();
                break;
            case 'd':
                write_file();
                break;
            case 'e':
                view_file();
                break;
            case 'f':
                delete_item();
                break;
            case 'g':
                create_directory();
                break;
            case 'h':
                show_file_info();
                break;
            case 'i':
                monitor_changes();
                break;
            case 'j':
                clear_screen();
                break;
            case 'x':
                printf(GREEN "Chiusura del programma. A presto!\n" RESET);
                break;
            default:
                printf(RED "Scelta non riconosciuta. Inserisci una lettera valida.\n" RESET);
                break;
        }
    } while (choice != 'x');

    return EXIT_SUCCESS;
}
