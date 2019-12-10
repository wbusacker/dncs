#include <network_group.h>
#include <stdio.h>
#include <utils.h>

namespace NETG{

void Network_Group::listener(){

    /* Grab a single packet */
    struct NETG::NETG_packet_t  remote_pkt;
    struct sockaddr_in          remote_addr;
    socklen_t                   sock_len;
    sock_len = sizeof(struct sockaddr_in);

    int32_t bytes_back;

    bytes_back = recvfrom(socket_fd,
                &remote_pkt,
                sizeof(struct NETG::NETG_packet_t),
                0,
                (struct sockaddr*)&remote_addr,
                &sock_len
                );

    if(bytes_back == -1){
        perror("Malformed incoming packet");
    }

    inspect_packet(&remote_pkt);

    /* Do something with the packet */
    switch(remote_pkt.type){
        case NETG::JOIN_REQUEST:
            printf("Processing Join Request\n");
            perform_join_request(&remote_pkt, &remote_addr);
            break;
        case NETG::JOIN_COMMAND:
            printf("Processing Join Command\n");
            perform_join_command(&remote_pkt);
            break;
        default:
            break;
    };

    return;

}

};