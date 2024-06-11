#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include "proto.h"
#include "tcp_client.h"

int main(void)
{
    int i = 0;
    char *test = "D.T.Software";
    message_t *pm = NULL;
    TcpClient *client = TcpClientNew();

    if (client && TcpClientConnect(client, "127.0.0.1", 8000)) {
        printf("connect success\n");
        for (i = 0; i < strlen(test); i++) {
            char buf[2] = {0};
            buf[0] = test[i];
            pm = message_new(128, 129, i, strlen(test), 2, buf);
            TcpClientSendMsg(client, pm);

            free(pm);
        }
    }

    getchar();
    TcpClientDel(client);

    return 0;
}