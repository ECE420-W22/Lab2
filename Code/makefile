CC = gcc
SERVER_NAMES = main1 main2
SERVER_NAME = server
CLIENT_NAME = client

main1: main1.o
	$(CC) -pthread -o main1 main1.o -lm
main2: main2.o
	$(CC) -pthread -o main2 main2.o -lm
$(SERVER_NAME): server.o
	$(CC) -pthread -o $(SERVER_NAME) server.o -lm
$(CLIENT_NAME): client.o
	$(CC) -pthread -o $(CLIENT_NAME) client.o -lm

main2.o: main2.c common.h timer.h
	$(CC) -c main2.c -o main2.o
main1.o: main1.c common.h timer.h
	$(CC) -c main1.c -o main1.o
server.o : mainServer.c common.h timer.h
	$(CC) -c mainServer.c -o server.o
client.o : mainClient.c common.h timer.h
	$(CC) -c mainClient.c -o client.o


clean:
	rm *.o $(SERVER_NAMES) $(SERVER_NAME) $(CLIENT_NAME)