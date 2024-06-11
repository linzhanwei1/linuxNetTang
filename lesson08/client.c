#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "proto.h"
#include "msg_parser.h"

#define HOST "127.0.0.1"
#define PORT 8000

static void hton(message_t *m)
{
    m->type = htons(m->type);
    m->cmd = htons(m->cmd);
    m->index = htons(m->index);
    m->total = htons(m->total);
    m->length = htonl(m->length);
}

int main(int argc, char *argv[])
{
    int sock = 0;
    struct sockaddr_in addr = {0};
    char buf[128] = {0};
    char input[32] = {0};
    int r = 0, len;
    int i = 0;
    char *test = "D.T.Software";
    message_t *pm = NULL;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }

    addr.sin_family = AF_INET;
    inet_pton(AF_INET, HOST, &addr.sin_addr.s_addr);
    addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect()");
        exit(1);
    }

    printf("connect success\n");

    for (i = 0; i < strlen(test); i++) {
        char buf[2] = {0};
        buf[0] = test[i];

        pm = message_new(128, 129, i, strlen(test), 2, buf);
        // printf("%s\n", buf);
        hton(pm);
        send(sock, pm, sizeof(message_t) + 2, 0);
        free(pm);
    }

    getchar();

    close(sock);
    exit(0);
}