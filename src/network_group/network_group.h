#ifndef NETWORK_GROUP_H
#define NETWORK_GROUP_H

#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <packet_types.h>

namespace NETG{

const uint16_t  NODE_RECEIVE_PORT   = 0xE31E;
const uint16_t  MAX_NODES_ALLOWABLE = 32;

struct Node{
    uint32_t    ip_address;
    uint16_t    ip_port;
    uint8_t     name[NETG::MAX_STRING_ID_LENGTH];
    uint8_t     data[NETG::MAX_DATA_LENGTH];
    uint32_t    data_length;
    bool        stale_data;
};

class Network_Group{
    public:
        Network_Group();
        ~Network_Group();

        void        set_name(char * name);

        bool        join_network(uint32_t target_ip, uint16_t target_port);
        void        listener();
        void        sync_data_command(uint8_t *buffer, uint32_t data_length);
        void        dump_table(void);
        uint32_t    read_data(uint8_t *buffer,
                              char name[NETG::MAX_STRING_ID_LENGTH]
                              );
        uint8_t     get_group_names(char names[NETG::MAX_NODES_ALLOWABLE][NETG::MAX_STRING_ID_LENGTH]);

        inline void shutdown(void){
            should_shutdown = true;
        }

        inline bool check_shutdown(void){
            return should_shutdown;
        }

    private:

        void        perform_join_request(struct NETG_packet_t *pkt, struct sockaddr_in *addr);
        void        perform_join_command(struct NETG_packet_t *pkt);
        void        perform_data_command(struct NETG_packet_t *pkt);


        bool        transmit_packet(struct NETG_packet_t *pkt, uint8_t node_num);
        void        get_packet(struct NETG_packet_t *pkt);

        void        inspect_packet(struct NETG_packet_t *pkt);

        uint8_t     local_name[NETG::MAX_STRING_ID_LENGTH];
        struct Node node_list[NETG::MAX_NODES_ALLOWABLE];
        uint8_t     num_known_computers;

        int                 socket_fd;
        struct sockaddr_in  socket_port;

        volatile bool       should_shutdown;

};

};

#endif