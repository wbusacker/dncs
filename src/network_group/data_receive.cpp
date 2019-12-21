#include <network_group.h>
#include <string.h>
#include <utils.h>

namespace NETG{

void Network_Group::perform_data_command(struct NETG_packet_t *pkt){

    /* Find which index in the Node list the sender is */

    uint16_t i;
    struct Node *node = NULL;
    for(i = 1; i < num_known_computers; i++){
        node = &(node_list[i]);

        if(strcmp((char*)pkt->name, (char*)node->name) == 0){
            break;
        }
    }

    /* If the node wasn't found, drop the packet */
    if(node == NULL){
        return;
    }

    /* Check to make sure the data length of the packet makes sense */
    if(pkt->body.data_command.data_length >= MAX_DATA_LENGTH){
        printf("\n\nFatal, malformed packet\n");
        hex_dump((uint8_t*)pkt, sizeof(struct NETG_packet_t));
        exit(-1);
    }

    // printf("Got packet from %s length %d that contains", pkt->name, pkt->body.data_command.data_length);
    // hex_dump(pkt->body.data_command.data, pkt->body.data_command.data_length);

    /* Copy the data over into the node */
    memcpy(node->data,
           pkt->body.data_command.data,
           pkt->body.data_command.data_length);

    /* Set the data lenth & mark not stale */
    node->data_length = pkt->body.data_command.data_length;
    node->stale_data = false;

    return;
}

};