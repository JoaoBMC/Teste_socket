#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
    pthread_t server_thrd;
    pthread_create(&server_thrd, NULL, init_server_socket, NULL);
    pthread_join(server_thrd, NULL);

    return 0;
}
