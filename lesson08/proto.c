#include <stdlib.h>
#include <string.h>

#include "proto.h"

message_t *message_new(unsigned short type, unsigned short cmd, unsigned short index, unsigned short total, unsigned int length, const char *payload)
{
    message_t *ret = malloc(sizeof(message_t) + length);
    if (ret) {
        ret->type = type;
        ret->cmd = cmd;
        ret->index = index;
        ret->total = total;
        ret->length = length;

        if (payload) {
            memcpy(ret + 1, payload, length);
        }
    }

    return ret;
}
