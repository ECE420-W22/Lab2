CC = gcc
CFLAGS = -std=c99
SERVER_NAMES = main1 main2 main3 main4
SERVER_NAME = server
CLIENT_NAME = client

all: $(SERVER_NAMES)

main1: main1.o
	$(CC) $(CFLAGS) -pthread -o main1 main1.o -lm
main2: main2.o
	$(CC) $(CFLAGS) -pthread -o main2 main2.o -lm
main3: main3.o
	$(CC) $(CFLAGS) -pthread -o main3 main3.o -lm
main4: main4.o
	$(CC) $(CFLAGS) -pthread -o main4 main4.o -lm
$(SERVER_NAME): server.o
	$(CC) $(CFLAGS) -pthread -o $(SERVER_NAME) server.o -lm
$(CLIENT_NAME): client.o
	$(CC) $(CFLAGS) -pthread -o $(CLIENT_NAME) client.o -lm

main1.o: main1.c common.h timer.h
	$(CC) $(CFLAGS) -c main1.c -o main1.o
main2.o: main2.c common.h timer.h
	$(CC) $(CFLAGS) -c main2.c -o main2.o
main3.o: main3.c common.h timer.h rwLock.o
	$(CC) $(CFLAGS) -c main3.c -o main3.o
main4.o: main4.c common.h timer.h rwLock.o
	$(CC) $(CFLAGS) -c main4.c -o main4.o
rwLock.o: rwLock.c 
	$(CC) $(CFLAGS) -c rwLock.c -o rwLock.o
server.o : mainServer.c common.h timer.h
	$(CC) $(CFLAGS) -c mainServer.c -o server.o
client.o : mainClient.c common.h timer.h
	$(CC) $(CFLAGS) -c mainClient.c -o client.o


clean:
	rm *.o $(SERVER_NAMES) $(SERVER_NAME) $(CLIENT_NAME)