CC = gcc
CFLAGS = -Wall -Wextra

all: useLibNet

install:
	@echo "Installation du projet"
	mkdir -p ./bin
	mkdir -p ./exe

data.o: ./lib/data.c
	$(CC) $(CFLAGS) -c ./lib/data.c -o ./bin/data.o

session.o: ./lib/session.c
	$(CC) $(CFLAGS) -c ./lib/session.c -o ./bin/session.o

useLibNet: data.o session.o ./app/useLibNet.c
	$(CC) $(CFLAGS) -DCLIENT ./bin/data.o ./bin/session.o ./app/useLibNet.c  -o ./exe/useLibNetClient.exe
	$(CC) $(CFLAGS) -DSERVEUR ./bin/data.o ./bin/session.o ./app/useLibNet.c  -o ./exe/useLibNetServeur.exe

clean:
	rm -f *.o
	rm -f ./bin/*.o
	rm -f ./exe/*.exe
	