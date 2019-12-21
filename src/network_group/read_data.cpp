#include <network_group.h>
#include <string.h>

namespace NETG{

uint32_t Network_Group::read_data(uint8_t *buffer,
                                  char name[NETG::MAX_STRING_ID_LENGTH])
{

    /* Find the Node that has this name */
    uint16_t i;
    struct Node *node = NULL;
    for(i = 1; i < num_known_computers; i++){
        node = &(node_list[i]);

        if(strcmp(name, (char*)node->name) == 0){
            break;
        }
    }

    if(node == NULL){
        /* No one has that name, so return zero */
        return 0;
    }

    /* If the Node's buffer is stale, return zero */
    if(node->stale_data == true){
        return 0;
    }

    memcpy(buffer, node->data, node->data_length);
    return node->data_length;
}

/* End Namespace */
};