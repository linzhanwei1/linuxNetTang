#include <stdio.h>
#include <stdlib.h>

#include "msg_parser.h"

int main(int argc, char **argv)
{
    MParser *p = MParserNew();
    char buf[] = {0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04};
    char data[] = {0x11, 0x12, 0x13, 0x14};

    message_t *m = MParserReadMem(p, buf, sizeof(buf));

    if (!m) {
        printf("parse again...\n");
        m = MParserReadMem(p, data, sizeof(data));
    }

    printf("m = %p\n", m);
    if (m) {
        printf("type = %d\n", m->type);
        printf("cmd = %d\n", m->cmd);
        printf("index = %d\n", m->index);
        printf("total = %d\n", m->total);
        printf("length = %d\n", m->length);

        for (int i = 0; i < m->length; i++) {
            printf("0x%02X ", m->payload[i]);
        }
        printf("\n");
        free(m);
    }

    MParserDel(p);

    return 0;
}