#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
    pthread_t client_thread, input_thrd;
    pthread_create(&client_thread, NULL, init_client_socket, NULL);
    pthread_create(&input_thrd, NULL, user_input, NULL);
    pthread_join(client_thread, NULL);
    pthread_join(input_thrd, NULL);
    // init_client_socket();

    return 0;
}
