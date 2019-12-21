#include <network_group.h>
#include <string.h>

namespace NETG{

void Network_Group::sync_data_command(uint8_t *buffer, uint32_t data_length){

    /* If data length is longer than allowable, truncate */
    if(data_length > NETG::MAX_DATA_LENGTH){
        data_length = NETG::MAX_DATA_LENGTH;
    }

    /* Setup the data packet */
    struct NETG_packet_t data_pkt;
    data_pkt.type = NETG::DATA_COMMAND;
    memcpy(data_pkt.body.data_command.data, buffer, data_length);
    data_pkt.body.data_command.data_length = data_length;

    /* Send the packet to every known node */
    uint8_t i;
    for(i = 1; i < num_known_computers; i++){
        transmit_packet(&data_pkt, i);
    }

}

};