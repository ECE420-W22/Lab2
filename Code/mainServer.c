#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <common.h>

char **stringArray;

void *ServerEcho(void *args)
{
    int clientFileDescriptor = (int)args;
    char str[20];

    read(clientFileDescriptor, str, 20);
    printf("reading from client:%s\n", str);
    write(clientFileDescriptor, str, 20);
    close(clientFileDescriptor);
    return NULL;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in sock_var;
    int serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    int clientFileDescriptor;
    pthread_t t[COM_NUM_REQUEST];

    int arraySize = strtol(argv[1], NULL, 10);
    char *ip = argv[2];
    int port = strtol(argv[3], NULL, 10);

    stringArray = malloc(arraySize * sizeof(char *));
    for (int i = 0; i < arraySize; i++)
    {
        stringArray[i] = malloc(sizeof(char *));
    }

    sock_var.sin_addr.s_addr = inet_addr(ip);
    sock_var.sin_port = port;
    sock_var.sin_family = AF_INET;
    if (bind(serverFileDescriptor, (struct sockaddr *)&sock_var, sizeof(sock_var)) >= 0)
    {
        printf("socket has been created\n");
        listen(serverFileDescriptor, 2000);
        while (1) // loop infinity
        {
            for (int i = 0; i < COM_NUM_REQUEST; i++)
            {
                clientFileDescriptor = accept(serverFileDescriptor, NULL, NULL);
                printf("Connected to client %d\n", clientFileDescriptor);
                pthread_create(&t[i], NULL, ServerEcho, (void *)(long)clientFileDescriptor);
            }
        }
        close(serverFileDescriptor);
    }
    else
    {
        printf("socket creation failed\n");
    }
    return 0;
}