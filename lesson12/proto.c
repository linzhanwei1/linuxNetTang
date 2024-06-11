#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

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

int message_size(message_t *m)
{
    int ret = 0;

    if (m) {
        ret = sizeof(message_t) + m->length;
    }

    return ret;
}
message_t *message_n2h(message_t *m)
{
    if (m) {
        m->type = ntohs(m->type);
        m->cmd = ntohs(m->cmd);
        m->index = ntohs(m->index);
        m->total = ntohs(m->total);
        m->length = ntohl(m->length);
    }

    return m;
}
message_t *message_h2n(message_t *m)
{
    if (m) {
        m->type = htons(m->type);
        m->cmd = htons(m->cmd);
        m->index = htons(m->index);
        m->total = htons(m->total);
        m->length = htonl(m->length);
    }

    return m;
}
