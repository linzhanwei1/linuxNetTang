#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netinet/tcp.h>

#include "proto.h"
#include "msg_parser.h"

#define PORT 8000

int main(int argc, char **argv)
{
    int server = 0;
    struct sockaddr_in saddr = {0};
    int client = 0;
    struct sockaddr_in caddr = {0};
    socklen_t asize = 0;
    int len = 0;
    char buf[32] = {0};
    int r = 0;
    MParser *parser = MParserNew();

    if ((server = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }

    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "0.0.0.0", &saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    if (bind(server, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("bind()");
        exit(1);
    }

    if (listen(server, 1) < 0) {
        perror("listen()");
        exit(1);
    }

    printf("server start success\n");

    while (1) {
        struct tcp_info info = {0};
        int l = sizeof(info);
        asize = sizeof(caddr);

        if ((client = accept(server, (struct sockaddr *)&caddr, &asize)) < 0) {
            perror("accept()");
            close(server);
            exit(1);
        }

        printf("client: %d\n", client);

        do {
            getsockopt(client, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&l);
            message_t *m = MParserReadFd(parser, client);
            if (m) {
                // printf("type = %d\n", m->type);
                // printf("cmd = %d\n", m->cmd);
                // printf("index = %d\n", m->index);
                // printf("total = %d\n", m->total);
                // printf("length = %d\n", m->length);
                // printf("paload = %s\n", m->payload);
                printf("%s", m->payload);

                printf("\n");
                free(m);
            }
        } while (info.tcpi_state == TCP_ESTABLISHED);

        printf("client socket is closed.\n");
        close(client);
    }

    close(server);

    exit(1);
}