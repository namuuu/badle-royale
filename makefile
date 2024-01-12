CC = gcc
CFLAGS = -Wall -Wextra

all: useLibNet

client: CFLAGS += -DCLIENT
client: all
	@echo "Compilation du client"

serveur: CFLAGS += -DSERVEUR
serveur: all
	@echo "Compilation du serveur"

install:
	@echo "Installation du projet"
	mkdir -p ./bin
	mkdir -p ./exe

data.o: ./lib/data.c
	$(CC) $(CFLAGS) -c ./lib/data.c -o ./bin/data.o

session.o: ./lib/session.c
	$(CC) $(CFLAGS) -c ./lib/session.c -o ./bin/session.o

useLibNet: client.o data.o session.o ./app/useLibNet.c
	$(CC) $(CFLAGS) ./bin/client.o ./bin/data.o ./bin/session.o ./app/useLibNet.c  -o ./exe/useLibNet.exe

clean:
	rm -f *.o
	rm -f ./bin/*.o
	rm -f ./exe/*.exe
	