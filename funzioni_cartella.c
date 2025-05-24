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

// Pulisce il buffer d’input per evitare problemi con scanf
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funzione per elencare il contenuto di una directory
void list_directory(const char *path) {
    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    char full_path[MAX_PATH_LENGTH];

    d = opendir(path);
    if (d == NULL) {
        printf(RED"  Accesso alla cartella '%s' non riuscito.\n"RESET, path);
        perror(RED" Dettagli"RESET);
        return;
    }

    printf(GREEN"\n Contenuto della cartella: %s\n"RESET, path);
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, dir->d_name);
        if (stat(full_path, &file_stat) == 0) {
            if (S_ISDIR(file_stat.st_mode)) {
                printf(YELLOW"  %s/\n"RESET, dir->d_name);
            } else {
                printf(YELLOW"  %s\n"RESET, dir->d_name);
            }
        } else {
            printf(RED" Impossibile ottenere dettagli per '%s'\n"RESET, dir->d_name);
            perror(RED" Dettagli"RESET);
        }
    }
    closedir(d);
}

// Funzione per cambiare directory
void change_directory() {
    char dirname[MAX_FILENAME_LENGTH];
    printf(GREEN"Inserisci il nome della cartella di destinazione (es. '..' per tornare indietro): "RESET);
    scanf("%s", dirname);
    clear_input_buffer();

    if (chdir(dirname) != 0) {
        printf(RED"Impossibile accedere alla cartella '%s'.\n"RESET, dirname);
        perror(RED"Dettagli"RESET);
        return;
    }

    char current_path[MAX_PATH_LENGTH];
    if (getcwd(current_path, sizeof(current_path)) != NULL) {
        printf(YELLOW"Cartella corrente: %s\n"RESET, current_path);
    } else {
        printf(RED"Errore nel recuperare il percorso corrente.\n"RESET);
        perror(RED"Dettagli"RESET);
    }
}

// Funzione per creare una directory
void create_directory() {
    char dirname[MAX_FILENAME_LENGTH];
    printf(GREEN"Inserisci il nome della nuova cartella da creare: "RESET);
    scanf("%s", dirname);
    clear_input_buffer();

    if (mkdir(dirname, 0777) == 0) {
        printf(YELLOW"Cartella '%s' generata correttamente.\n"RESET, dirname);
    } else {
        printf(RED"Creazione della cartella '%s' fallita.\n"RESET, dirname);
        perror(RED"Dettagli"RESET);
    }
}
