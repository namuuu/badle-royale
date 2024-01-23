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
	$(CC) $(CFLAGS) -DCLIENT -DSTREAM ./bin/data.o ./bin/session.o ./app/useLibNet.c  -o ./exe/useLibNetClientStream.exe
	$(CC) $(CFLAGS) -DCLIENT -DDGRAM ./bin/data.o ./bin/session.o ./app/useLibNet.c  -o ./exe/useLibNetClientDgram.exe
	$(CC) $(CFLAGS) -DSERVEUR -DSTREAM ./bin/data.o ./bin/session.o ./app/useLibNet.c  -o ./exe/useLibNetServeurStream.exe
	$(CC) $(CFLAGS) -DSERVEUR -DDGRAM ./bin/data.o ./bin/session.o ./app/useLibNet.c  -o ./exe/useLibNetServeurDGram.exe

hub: data.o session.o ./app/hub.c
	$(CC) $(CFLAGS) ./bin/data.o ./bin/session.o ./app/hub.c  -o ./exe/hub.exe

clean:
	rm -f *.o
	rm -f ./bin/*.o
	rm -f ./exe/*.exe

doxi :
	doxygen doxifile
	