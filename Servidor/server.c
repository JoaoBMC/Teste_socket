#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define BUFF_SIZE 100
#define PORT 8080
#define PROTOCOL 0
#define SOCKET_ERROR 0
#define NUMBER_CONNECTIONS 1

void *init_server_socket(void *args)
{
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    pthread_t thr_id;
    int new_socket;

    new_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (new_socket <= SOCKET_ERROR)
    {
        printf("socket creation failed...\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created..\n");
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if ((bind(new_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))) < SOCKET_ERROR)
    {
        printf("socket bind failed...\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(new_socket, NUMBER_CONNECTIONS)) != SOCKET_ERROR)
    {
        printf("Listen failed...\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Server listening..\n");

    socklen_t client_size = sizeof(client_addr);

    while (1)
    {
        int connection_socket = accept(new_socket, (struct sockaddr *)&client_addr, &client_size);
        if (connection_socket < SOCKET_ERROR)
        {
            printf("server accept failed...\n");
            exit(EXIT_FAILURE);
        }
        else
            printf("server accept the client...\n");

        if (pthread_create(&thr_id, NULL, server_communiction, &connection_socket) != 0)
        {
            perror("Erro Thread");
        }
        else
            pthread_join(thr_id, NULL);
    }
    // server_communiction(connection_socket);
    close(new_socket);
    pthread_exit(NULL);
}

void *server_communiction(void *args)
{
    int new_socket = *((int *)args);
    char msg_buff[BUFF_SIZE];

    while (1)
    {

        memset(msg_buff, 0, sizeof(msg_buff));
        int valread = read(new_socket, msg_buff, sizeof(msg_buff));
        printf("From client: %s\t To client : ", msg_buff);
        fgets(msg_buff, sizeof(msg_buff), stdin);
        if (valread <= 0)
        {
            close(new_socket);
            pthread_exit(NULL);
        }

        send(new_socket, msg_buff, valread, 0);
        if (strncmp("#", msg_buff, 1) == 0)
        {
            printf("Server Exit...\n");
            exit(EXIT_SUCCESS);
            break;
        }
    }
}

// int main(int argc, char const *argv[])
// {
//     init_server_socket();

//     return 0;
// }
