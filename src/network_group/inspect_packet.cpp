#include <network_group.h>
#include <stdio.h>
#include <utils.h>

namespace NETG{

void Network_Group::inspect_packet(struct NETG_packet_t *pkt){

    printf("\n========= Packet Inspection =========\n");

    printf("Sender Name: %s\n", pkt->name);

    printf("Packet Type: ");

    switch(pkt->type){
        case NETG::JOIN_COMMAND:
            printf("Join Command\n");
            printf("Joinee IP %08X:%04X\n",
                   pkt->body.join_command.ip_addr,
                   pkt->body.join_command.port);
            printf("Joinee name %s\n", pkt->body.join_command.name);
            break;

        case NETG::JOIN_REQUEST:
            printf("Join Request\n");
            printf("Joinee name %s\n", pkt->body.join_request.name);
            break;

        case NETG::JOIN_RESPONSE:
            printf("Join Response\n");
            printf("Computers in group %d\n", pkt->body.join_response.num_computers_in_group);
            if(pkt->body.join_response.can_join){
                printf("Can join: True\n");
            } else {
                printf("Can join: False\n");
            }
            break;

        case NETG::DATA_COMMAND:
            printf("Data Command\n");
            printf("Bytes in packet: %d\n", pkt->body.data_command.data_length);
            hex_dump(pkt->body.data_command.data, pkt->body.data_command.data_length);
            break;

        case NETG::RESPONSE_ACK:
            printf("Response Acknowledgement\n");
            break;

        default:
            printf("Unknown\n");
    }

    
    printf("=====================================\n\n");

    return;

}

};