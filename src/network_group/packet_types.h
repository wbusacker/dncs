#ifndef PACKET_TYPES_H
#define PACKET_TYPES_H

namespace NETG{

const uint8_t   MAX_STRING_ID_LENGTH    = 32;   /* Don't let nodes give themselves huge names   */
const uint16_t  MAX_DATA_LENGTH         = 1024; /* Keep the UDP packets under most MTU sizes    */

enum Packet_types: uint8_t {
    JOIN_REQUEST,                                   /* New computers is joining the group               */
    JOIN_COMMAND,                                   /* Tell other computers about who is joining        */
    JOIN_RESPONSE,                                  /* Tell the new computer local information          */
    DATA_COMMAND,                                   /* Transmit data                                    */
    RESPONSE_ACK                                    /* Acknowledge packet recieved                      */
};


/* Use to join a network */
struct Join_request_t{
    uint8_t     name[NETG::MAX_STRING_ID_LENGTH];   /* Name of computer requesting to be in the group   */
};

/* Use to tell other computers in the network to link with a new computer */
struct Join_command_t{  
    uint32_t    ip_addr;                            /* IP Address of computer that is joining           */
    uint16_t    port;                               /* Port of computer that is joining                 */
    uint8_t     name[NETG::MAX_STRING_ID_LENGTH];   /* Name of computer that is joining                 */
};

/* Tell the new computer how to send to local computer */
struct Join_response_t{
    uint16_t    num_computers_in_group;             /* Number of computers currently known in the group */
    bool        can_join;                           /* Flag to idenfity if the computer can join        */
};

/* Transmit data */
struct Data_command_t{
    uint16_t    data_length;                        /* Length of data being traded                      */
    uint8_t     data[NETG::MAX_DATA_LENGTH];        /* Data being traded                                */
};

union Packet_body_u{
    struct Join_request_t   join_request;
    struct Join_response_t  join_response;
    struct Join_command_t   join_command;
    struct Data_command_t   data_command;
};

struct NETG_packet_t{
    uint8_t             name[NETG::MAX_STRING_ID_LENGTH];
    enum Packet_types   type;
    union Packet_body_u body;
};

};

#endif