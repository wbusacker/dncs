#include <network_group.h>
#include <string.h>

namespace NETG{

void Network_Group::dump_table(void){

    uint16_t i;    
    struct NETG::Node *node;
    uint8_t ip0;
    uint8_t ip1;
    uint8_t ip2;
    uint8_t ip3;

    for(i = 1; i < num_known_computers; i++){
        /* Shorthand reference the node */
        node = &(node_list[i]);

        ip0 = (node->ip_address >> 0)  & 0xff;
        ip1 = (node->ip_address >> 8)  & 0xff;
        ip2 = (node->ip_address >> 16) & 0xff;
        ip3 = (node->ip_address >> 24) & 0xff;

        char ip_addr_str[18];
        sprintf(ip_addr_str, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);

        printf("IP Addr: %15s:%-5d | ", ip_addr_str, node->ip_port);
        printf("Hostname %s\n", node->name);
    }

    fflush(stdout);

    return;
}

};