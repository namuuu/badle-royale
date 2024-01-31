CC = gcc
CFLAGS = -Wall -Wextra -lpthread -lrt
IMPORTS = ./bin/data.o ./bin/session.o

all: user hub
	@echo "\033[32m"
	@echo "\tProject Badle-Royale has been compiled!"
	@echo "\033[0m"

install:
	@echo "Project install"
	mkdir -p ./bin
	mkdir -p ./exe

data.o: ./lib/data.c
	@$(CC) $(CFLAGS) -c ./lib/data.c -o ./bin/data.o

session.o: ./lib/session.c
	@$(CC) $(CFLAGS) -c ./lib/session.c -o ./bin/session.o

useLibNet: data.o session.o ./app/useLibNet.c # Compile tous les fichiers de useLibNet
	$(CC) $(CFLAGS) -DCLIENT -DSTREAM $(IMPORTS) ./app/useLibNet.c  -o ./exe/useLibNetClientStream.exe
	$(CC) $(CFLAGS) -DCLIENT -DDGRAM $(IMPORTS) ./app/useLibNet.c  -o ./exe/useLibNetClientDgram.exe
	$(CC) $(CFLAGS) -DSERVEUR -DSTREAM $(IMPORTS) ./app/useLibNet.c  -o ./exe/useLibNetServeurStream.exe
	$(CC) $(CFLAGS) -DSERVEUR -DDGRAM $(IMPORTS) ./app/useLibNet.c  -o ./exe/useLibNetServeurDGram.exe

user : data.o session.o ./app/user.c # Compile tous les fichiers de user
	@$(CC) $(CFLAGS) $(IMPORTS) ./app/user.c -o ./exe/user.exe
  
hub: data.o session.o ./app/hub.c # Compile tous les fichiers de hub
	@$(CC) $(CFLAGS) $(IMPORTS) ./app/hub.c  -o ./exe/hub.exe

clean:
	rm -f *.o
	rm -f ./bin/*.o
	rm -f ./exe/*.exe

doxi :
	doxygen doxifile