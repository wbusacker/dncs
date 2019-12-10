#include <network_group.h>
#include <string.h>

namespace NETG{

void Network_Group::perform_join_command(struct NETG_packet_t *pkt){
    /* Append the new node to the table */
    struct NETG::Node *node;
    node = &(node_list[num_known_computers]);

    node->ip_address    = pkt->body.join_command.ip_addr;
    node->ip_port       = pkt->body.join_command.port;
    memcpy(node->name,
           pkt->body.join_command.name,
           NETG::MAX_STRING_ID_LENGTH);

    /* Send a join response back to the computer */
    struct NETG::NETG_packet_t response;

    response.type = NETG::JOIN_RESPONSE;
    response.body.join_response.can_join = true;
    response.body.join_response.num_computers_in_group = num_known_computers;

    transmit_packet(&response, num_known_computers);

    /* Up the table counter */
    num_known_computers++;

    printf("Joined with %s\n", node->name);

    return;
}

};