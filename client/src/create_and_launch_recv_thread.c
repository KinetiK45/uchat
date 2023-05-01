#include "../inc/client.h"

void create_and_launch_recv_thread(t_sock *sock) {
    pthread_t thread_id;
    t_thread_args *thread_args = (struct s_thread_args*)malloc(sizeof(t_thread_args));
    pthread_mutex_init(&sock->mutex, NULL);
    thread_args->client_sock = sock;
    pthread_create(&thread_id, NULL, recv_messages_from_server, (void *)thread_args);
    //free(thread_args);
}
