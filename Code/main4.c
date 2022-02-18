#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "common.h"
#include "timer.h"
#include "rwLock.c"

char **stringArray;
double timeArray[COM_NUM_REQUEST];
int arraySize;
mylib_rwlock_t *arrayRWLock;

static void sigintHandler(int sig)
{
    int i;
    for (i = 0; i < arraySize; i++)
    {
        free(&stringArray[i]);
    }
    free(arrayRWLock);
    free(stringArray);
}

void *ServerHandle(void *args)
{
    int clientFileDescriptor = (int)args;
    char str[COM_BUFF_SIZE];
    ClientRequest req;
    char *arrayVal = malloc(COM_BUFF_SIZE * sizeof(char));
    read(clientFileDescriptor, str, COM_BUFF_SIZE);
    ParseMsg(str, &req);
    // Critical section begin
    if (!req.is_read)
    {
        mylib_rwlock_wlock(&arrayRWLock[req.pos]);
        setContent(req.msg, req.pos, stringArray);
        mylib_rwlock_unlock(&arrayRWLock[req.pos]);
        strcpy(arrayVal, req.msg);
    }
    else
    {
        mylib_rwlock_rlock(&arrayRWLock[req.pos]);
        getContent(arrayVal, req.pos, stringArray);
        mylib_rwlock_unlock(&arrayRWLock[req.pos]);
    }
    // Critical section end
    return (void *)arrayVal;
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
    arrayRWLock = malloc(arraySize * sizeof(mylib_rwlock_t));

    int i;
    for (i = 0; i < arraySize; i++)
    {
        stringArray[i] = malloc(COM_BUFF_SIZE * sizeof(char));
        mylib_rwlock_init(&arrayRWLock[i]);
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
            for (i = 0; i < COM_NUM_REQUEST; i++)
            {
                char *returnVal; // The return value for the write
                double start;    // Time after server accepts client
                double end;      // Time before server writes to client
                clientFileDescriptor = accept(serverFileDescriptor, NULL, NULL);
                // Don't include print to the measure
                printf("Connected to client %d\n", clientFileDescriptor);
                // Start measuring time
                GET_TIME(start);
                pthread_create(&t[i], NULL, ServerHandle, (void *)(long)clientFileDescriptor);
                pthread_join(t[i], (void **)&returnVal);
                // Stop measuring time
                GET_TIME(end);
                write(clientFileDescriptor, returnVal, COM_BUFF_SIZE);
                close(clientFileDescriptor);
                timeArray[i] = end - start;
                free(returnVal);
            }
            saveTimes(timeArray, COM_NUM_REQUEST);
        }
        close(serverFileDescriptor);
    }
    else
    {
        printf("socket creation failed\n");
    }
    return 0;
}