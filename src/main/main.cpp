#include <stdio.h>
#include <network_group.h>
#include <pthread.h>

NETG::Network_Group ng;

void *dncs_listener(void *);

volatile bool kill_thread = false;

int main(int argc, char *argv[]){

    bool status;

    /* Get address information */

    uint32_t ip_addr = 0;
    uint32_t ip_port = NETG::NODE_RECEIVE_PORT;

    /* If there is a second argument, treat it as IP address */
    if(argc >= 2){
        ip_addr = inet_addr(argv[1]);
    }

    /* If there is a third argument, treat it as port */
    if(argc >= 3){
        ip_port = atoi(argv[2]);
    }

    /* Join the group */

    status = ng.join_network(ip_addr, ip_port);

    if(status != true){
        printf("Initialization failed\n");
        return -1;
    }

    /* Start up the listener thread */

    pthread_t thread_handle;

    pthread_create(&thread_handle,
                    NULL,
                    dncs_listener,
                    NULL
    );

    /* Get a random character */
    char dummy;
    dummy = getchar();
    dummy++;

    kill_thread = true;
    ng.dump_table();

    return 0;
}

void *dncs_listener(void *){
    while(!kill_thread){
        ng.listener();
    }

    return NULL;
}