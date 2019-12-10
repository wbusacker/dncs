#include <network_group.h>

namespace NETG{

void Network_Group::set_name(char * name){

    /* Copy characters one at a time but up to the end */
    uint8_t i;
    for(i = 0; i < NETG::MAX_STRING_ID_LENGTH; i++){
        if(name[i] == '\0'){
            break;
        }
        local_name[i] = name[i];
    }
}

};