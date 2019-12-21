#include <stdio.h>
#include <network_group.h>
#include <pthread.h>
#include <unistd.h>

NETG::Network_Group ng;

void *dncs_listener(void *net_g);
void *dncs_transmitter(void *);

volatile bool kill_thread = false;
volatile bool thread_dead = false;

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

    /* Launch data thread */
    pthread_t th_sender;
    pthread_create(&th_sender,
                    NULL,
                    dncs_transmitter,
                    NULL
    );

    /* While that thread is still alive, try to get readings*/

    while(thread_dead == false){
        /* Get a list of updated names */
        char names[NETG::MAX_NODES_ALLOWABLE][NETG::MAX_STRING_ID_LENGTH];
        uint8_t name_count;
        name_count = ng.get_group_names(names);

        /* For each name, get its data and print */
        printf("\r");
        uint8_t ni;
        uint8_t buf;
        uint16_t len;
        for(ni = 0; ni < name_count; ni++){
            len = ng.read_data(&buf, names[ni]);
            if(len != 0){
                printf("%s %d\t", names[ni], buf);
            }
            // printf("%s \t", names[ni]);
        }
        fflush(stdout);
    }

    /* Get a random character */
    dummy = getchar();
    dummy++;

    kill_thread = true;
    ng.dump_table();

    return 0;
}

void *dncs_listener(void *net_g){

    if(net_g == NULL){
        return NULL;
    }

    NETG::Network_Group *net_group;
    net_group = (NETG::Network_Group*)net_g;
    while(!net_group->check_shutdown()){
        net_group->listener();
    }

    return NULL;
}

void *dncs_transmitter(void *){
    uint16_t i;
    uint8_t b;
    for(i = 0; i < 10; i++){
        b = i;
        ng.sync_data_command(&b, 1);
        sleep(1);
    }

    thread_dead = true;

    pthread_exit(NULL);
}