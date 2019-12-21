#include <network_group.h>
#include <string.h>
#include <utils.h>

namespace NETG{

bool Network_Group::join_network(uint32_t target_ip, uint16_t target_port){

    /* Make sure we have a name */
    if(local_name[0] == '\0'){
        return false;
    }

    /* Construct socket and bind*/
    socket_port.sin_family      = AF_INET;
    socket_port.sin_addr.s_addr = INADDR_ANY;
    socket_port.sin_port        = NETG::NODE_RECEIVE_PORT;

    int ret_val;
    ret_val = bind(socket_fd, (struct sockaddr *)&socket_port, sizeof(struct sockaddr_in));

    if(ret_val != 0){
        perror("Failed to bind socket\n");
        return false;
    } else {
        printf("Bound on port %d\n", socket_port.sin_port);
    }

    /* If the target address is 0, then we're setting up a network
        and have no one to target
    */
    if(target_ip == 0){
        num_known_computers = 1;
        return true;
    }

    /* Build up a join request packet */
    struct NETG::NETG_packet_t join_packet;

    join_packet.type = NETG::JOIN_REQUEST;
    memcpy(join_packet.body.join_request.name,  local_name, NETG::MAX_STRING_ID_LENGTH);

    /* Use the zeroth node to target */
    node_list[0].ip_address = target_ip;
    node_list[0].ip_port    = target_port;

    bool ret_bool;
    ret_bool = transmit_packet(&join_packet, 0);
    if(ret_bool == false){
        printf("Failed to transmit packet\n");
        return false;
    }

    /* Get the first response */
    struct NETG::NETG_packet_t  response;
    struct sockaddr_in          remote_addr;
    socklen_t                   sock_len;
    sock_len = sizeof(struct sockaddr_in);
    /* Set expected computers to two because we do expect to join the group */
    uint8_t expected_computers  = 2;
    uint8_t discovered_computers = 0;
    num_known_computers = 1;

    do{
        int32_t bytes_back;
        /* Get the response */
        bytes_back = recvfrom(socket_fd,
                &response,
                sizeof(struct NETG::NETG_packet_t),
                0,
                (struct sockaddr*)&remote_addr,
                &sock_len);

        if(bytes_back == -1){
            perror("Malformed Response:");
        }

        /* Check to make sure we got a join response */
        if(response.type == NETG::JOIN_RESPONSE){

            /* Check if we're allowed in the group */
            if(response.body.join_response.can_join == false){
                printf("Failed to join group\n");
                return false;
            }

            /* Populate the node list */
            struct Node *node;
            node = &(node_list[num_known_computers]);

            node->ip_address    = remote_addr.sin_addr.s_addr;
            node->ip_port       = remote_addr.sin_port;
            memcpy(node->name, response.name, NETG::MAX_STRING_ID_LENGTH);

            /* Updated expected computers based upon the response */
            expected_computers = response.body.join_response.num_computers_in_group;
            printf("Joined with %s from %08X:%X\n", node->name, ntohl(node->ip_address), node->ip_port);
            // printf("Told to look for %d computers\n", expected_computers);

            num_known_computers++;
            discovered_computers++;
        }

    }while(discovered_computers < expected_computers);

    printf("In Group\n");

    /* We're in the group! */
    return true;
}

};