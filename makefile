all: main

clean: main main.o funzioni_cartella_file.o funzioni_cartella.o funzioni_file.o funzioni_inotify.o 
		rm -rf main main.o funzioni_cartella_file.o funzioni_cartella.o funzioni_file.o funzioni_inotify.o 

main: main.o funzioni_cartella_file.o funzioni_cartella.o funzioni_file.o funzioni_inotify.o logo.o
		gcc -o main main.o funzioni_cartella_file.o funzioni_cartella.o funzioni_file.o funzioni_inotify.o logo.o

main.o: main.c funzioni_cartella_file.h funzioni_cartella.h funzioni_file.h funzioni_inotify.h	logo.h	
		gcc -c -o main.o main.c 

funzioni_cartella_file.o: funzioni_cartella_file.c funzioni_cartella_file.h
		gcc -c -o funzioni_cartella_file.o funzioni_cartella_file.c 

funzioni_cartella.o: funzioni_cartella.c funzioni_cartella.h
		gcc -c -o funzioni_cartella.o funzioni_cartella.c 

funzioni_file.o: funzioni_file.c funzioni_file.h
		gcc -c -o funzioni_file.o funzioni_file.c 

funzioni_inotify.o: funzioni_inotify.c funzioni_inotify.h
		gcc -c -o funzioni_inotify.o funzioni_inotify.c 

logo.o: logo.c logo.h
		gcc -c -o logo.o logo.c

