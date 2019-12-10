#include <network_group.h>
#include <string.h>
#include <utils.h>

namespace NETG{

bool Network_Group::transmit_packet(struct NETG_packet_t *pkt, uint8_t node_num){
    /* Setup destination sockaddr */
    struct sockaddr_in  target;

    target.sin_addr.s_addr  = node_list[node_num].ip_address;
    target.sin_port         = node_list[node_num].ip_port;
    target.sin_family       = AF_INET;

    int ret_val;
    /* Put the local name in the packet */
    memcpy(pkt->name, local_name, NETG::MAX_STRING_ID_LENGTH);

    printf("Transmitting packet to %08X:%04X\n", ntohl(target.sin_addr.s_addr), target.sin_port);

    ret_val = sendto(socket_fd,
                     (const void*)pkt,
                     sizeof(NETG::NETG_packet_t),
                     0,
                     (struct sockaddr*)&target,
                     sizeof(struct sockaddr)
                     );

    if(ret_val == -1){
        perror("Failed to transmit packet");
        return false;
    }

    return true;
}

};