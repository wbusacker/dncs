#include <network_group.h>
#include <string.h>
#include <stdio.h>

namespace NETG{

void Network_Group::perform_join_request(struct NETG_packet_t *pkt, struct sockaddr_in *addr){

    /* Setup a response, but don't officially add the computer to the table */
    struct NETG::NETG_packet_t reply_pkt;

    reply_pkt.type = NETG::JOIN_RESPONSE;

    /* Use Node 0 as a temp */
    node_list[0].ip_address = addr->sin_addr.s_addr;
    node_list[0].ip_port    = addr->sin_port;

    /* Can we handle more nodes? */
    if(num_known_computers == NETG::MAX_NODES_ALLOWABLE){
        /* Transmit a join response with can_join set false */
        reply_pkt.body.join_response.can_join = false;
    } else {
        reply_pkt.body.join_response.can_join = true;
    }

    reply_pkt.body.join_response.num_computers_in_group = num_known_computers;

    transmit_packet(&reply_pkt, 0);

    if(reply_pkt.body.join_response.can_join == false){
        /* If we told the computer it can't join, don't do anything else */
        printf("Cannot add any new users\n");
        fflush(stdout);
        return;
    }

    /* Add the packet to the list of known nodes */
    struct NETG::Node *node;
    node = &(node_list[num_known_computers]);

    node->ip_address = addr->sin_addr.s_addr;
    node->ip_port    = addr->sin_port;
    memcpy(node->name, pkt->name, NETG::MAX_STRING_ID_LENGTH);

    /* Tell all other known nodes to add */
    struct NETG::NETG_packet_t cmd_pkt;

    cmd_pkt.type                        = NETG::JOIN_COMMAND;
    cmd_pkt.body.join_command.ip_addr   = addr->sin_addr.s_addr;
    cmd_pkt.body.join_command.port      = addr->sin_port;
    memcpy(cmd_pkt.body.join_command.name,
           pkt->name,
           NETG::MAX_STRING_ID_LENGTH);

    uint8_t i;
    for(i = 1; i < num_known_computers; i++){
        transmit_packet(&cmd_pkt, i);
    }

    printf("Joined with %s from %08X:%X\n", node->name, ntohl(node->ip_address), node->ip_port);

    /* Bump up known computers */
    num_known_computers++;

}

};