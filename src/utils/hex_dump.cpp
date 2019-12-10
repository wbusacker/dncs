#include <utils.h>
#include <stdio.h>

static const uint8_t word_wrap_line_length  = 32;
static const uint8_t hex_group_len_bytes    = 4;

void hex_dump(uint8_t *buff, uint16_t buf_len){
    uint32_t buf_index;
    for(buf_index = 0; buf_index < buf_len; buf_index++){
        /* Do we need to wrap to the next line */
        if((buf_index % word_wrap_line_length) == 0){
            printf("\n");
            fflush(stdout);
        }

        /* Do we need to start a new group */
        if((buf_index % hex_group_len_bytes) == 0){
            printf(" 0x");
        }

        printf("%02X", buff[buf_index]);
    }
    printf("\n");
    fflush(stdout);
}