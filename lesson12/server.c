#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tcp_server.h"

void EventListener(TcpClient *client, int evt)
{
    if (evt == EVT_CONN) {
        printf("Connect: %p\n", client);
    } else if (evt == EVT_DATA) {
        message_t *m = TcpClientRecvMsg(client);

        if (m) {
            char *s = TcpClientGetData(client);

            if (m->index == 0) {
                s = malloc(m->total + 1);
                TcpClientSetData(client, s);
            }

            strcpy(s + m->index, m->payload);
            if ((m->index + 1) == m->total) {
                printf("Data: %s\n", s);
                free(s);
            }

            free(m);
        }
    } else if (evt = EVT_CLOSE) {
        printf("Close: %p\n", client);
    }
}

int main(void)
{
    TcpServer *server = TcpServerNew();

    if (server) {
        int r = TcpServerStart(server, 8000, 10);
        printf("r = %d\n", r);

        if (r) {
            TcpServerSetListener(server, EventListener);
            TcpServerDoWork(server);
        }
    }
    return 0;
}