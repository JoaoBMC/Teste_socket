#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define BUFF_SIZE 100
#define PROTOCOL 0
#define SOCKET_ERROR -1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int thrd_pause = 0;

void *client_comunicaion(void *args)
{
    int socket_FD = *((int *)args);
    char msg_buff[BUFF_SIZE];
    int buff_index;
    while (1)
    {
        memset(msg_buff, 0, sizeof(msg_buff));
        printf("To Server: ");
        fgets(msg_buff, sizeof(msg_buff), stdin);

        write(socket_FD, msg_buff, sizeof(msg_buff));
        // memset(msg_buff, 0,sizeof(msg_buff));

        if ((strncmp(msg_buff, "#", 1)) == 0)
        {
            printf("Client Exit...\n");
            exit(EXIT_SUCCESS);
        }

        read(socket_FD, msg_buff, sizeof(msg_buff));
        printf("From Server : %s\n", msg_buff);
    }
}

void *user_input(void *args)
{
    char msg_buff[BUFF_SIZE];
    fd_set fds;
    struct timeval timeout;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (thrd_pause)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        
        timeout.tv_sec = 1; 
        timeout.tv_usec = 0;

        int result = select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout);

        if (result > 0 && FD_ISSET(STDIN_FILENO, &fds) 
                        && fgets(msg_buff, sizeof(msg_buff), stdin) != NULL 
                        && strcmp(msg_buff, "#\n") == 0)
        {
            printf("Client Exit...\n");
            exit(EXIT_SUCCESS);
        }
    }
}

void *init_client_socket(void *args)
{
    struct sockaddr_in server_addr;
    pthread_t client_thrd, input_thrd;

    char ip_addr[16];
    int new_socket, port = 0, counter = 0;

    new_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (new_socket == SOCKET_ERROR)
    {
        printf("socket creation failed...\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created..\n");

    memset(&server_addr, 0, sizeof(server_addr));

    printf("IP Address: ");
    // fgets(ip_addr, sizeof(ip_addr), stdin);
    scanf("%s", ip_addr);

    printf("Port: ");
    scanf("%d", &port);

    printf("\n IP: %s Port: %d\n", ip_addr, port);

    while (1)
    {

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ip_addr);
        server_addr.sin_port = htons(port);

        // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        // server_addr.sin_port = htons(8080);

        if (connect(new_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
        {
            printf("connection with the server failed...\n");
            sleep(1);
            continue;
        }
        else
        {
            printf("connected to the server..\n");
            pthread_mutex_lock(&mutex);
            thrd_pause = 1;
            pthread_mutex_unlock(&mutex);
            break;
        }
        
    };

    pthread_create(&client_thrd, NULL, client_comunicaion, &new_socket);
    pthread_join(client_thrd, NULL);
    // client_comunicaion(new_socket);

    close(new_socket);
}

// int main(int argc, char const *argv[])
// {
//     init_client_socket();
//     return 0;
// }
