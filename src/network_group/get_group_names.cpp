#include <network_group.h>
#include <string.h>

namespace NETG{

uint8_t Network_Group::get_group_names(char names[NETG::MAX_NODES_ALLOWABLE][NETG::MAX_STRING_ID_LENGTH])
{
    /* Copy each known Node's name into the buffer */
    uint8_t i;
    for(i = 1; i < num_known_computers; i++){

        memcpy(names[i-1], 
               node_list[i].name,
               NETG::MAX_STRING_ID_LENGTH);
    }

    return num_known_computers - 1;

}

/* End Namespace */
};