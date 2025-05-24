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

// Colori ANSI
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define RED     "\033[0;31m"
#define RESET   "\033[0m"

#define INOTIFY_EVENT_BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

// Funzione per monitorare modifiche in una directory con inotify
void monitor_changes() {
    char dirname[MAX_PATH_LENGTH];
    printf(GREEN"Inserisci il percorso della cartella da osservare: "RESET);
    if (scanf("%255s", dirname) != 1) {
        printf(RED"Errore nella lettura dell'input.\n"RESET);
        return;
    }
    getchar(); // Consuma newline extra

    struct stat st;
    if (stat(dirname, &st) != 0 || !S_ISDIR(st.st_mode)) {
        printf(RED"Il percorso '%s' non è una cartella valida.\n"RESET, dirname);
        return;
    }

    int fd = inotify_init();
    if (fd < 0) {
        perror(RED"Errore durante l'inizializzazione di inotify"RESET);
        return;
    }

    int wd = inotify_add_watch(fd, dirname, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO | IN_ISDIR);
    if (wd < 0) {
        perror(RED"Errore nell'aggiungere la cartella al monitoraggio"RESET);
        close(fd);
        return;
    }

    printf(YELLOW"Inizio monitoraggio su '%s'. Premi Ctrl+C per interrompere.\n"RESET, dirname);

    char buffer[INOTIFY_EVENT_BUF_LEN];
    struct inotify_event *event;
    ssize_t length;

    while (1) {
        length = read(fd, buffer, sizeof(buffer));
        if (length < 0) {
            if (errno == EINTR) {
                printf(GREEN"\nMonitoraggio terminato.\n"RESET);
                break;
            }
            perror(RED"Errore durante la lettura degli eventi"RESET);
            break;
        }

        for (char *ptr = buffer; ptr < buffer + length; ptr += sizeof(struct inotify_event) + event->len) {
            event = (struct inotify_event *)ptr;

            if (event->len > 0) {
                printf("\nEvento su: %s\n", event->name);
            }

            if (event->mask & IN_CREATE)      printf("  ▸ Creato\n");
            if (event->mask & IN_DELETE)      printf("  ▸ Eliminato\n");
            if (event->mask & IN_MODIFY)      printf("  ▸ Modificato\n");
            if (event->mask & IN_MOVED_FROM)  printf("  ▸ Spostato da\n");
            if (event->mask & IN_MOVED_TO)    printf("  ▸ Spostato a\n");
            if (event->mask & IN_ISDIR)       printf("  ▸ (Cartella)\n");
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}
