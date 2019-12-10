#include <network_group.h>

namespace NETG{

Network_Group::Network_Group(){
    /* Clear all internal data */
    uint16_t node_index;
    uint16_t name_index;
    struct Node *node;
    for(node_index = 0; node_index < NETG::MAX_NODES_ALLOWABLE; node_index++){
        node = &(node_list[node_index]);

        node->ip_address    = 0;
        node->data_buffer   = NULL;
        node->data_length   = 0;
        for(name_index = 0; name_index < NETG::MAX_STRING_ID_LENGTH; name_index++){
            node->name[name_index] = '\0';
        }
    }

    /* Get a socket */
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(socket_fd < 0){
        perror("Socket failed to open");
        exit(-1);
    }

    num_known_computers = 1;

    /* Set name to the system hostname */
    gethostname((char*)local_name, NETG::MAX_STRING_ID_LENGTH);
    /* Force the last character to be a null terminator */
    local_name[NETG::MAX_STRING_ID_LENGTH-1] = '\0';

    printf("Starting on %s\n", local_name);

}


Network_Group::~Network_Group(){

    /* Close the socket */
    close(socket_fd);

}

};