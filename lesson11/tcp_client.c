#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include "tcp_client.h"
#include "msg_parser.h"

typedef struct tcp_client {
    int fd;
    MParser *parser;
    void *data;
} Client;

TcpClient *TcpClientNew(void)
{
    return TcpClientFrom(-1);
}

TcpClient *TcpClientFrom(int fd)
{
    Client *ret = malloc(sizeof(Client));

    if (ret) {
        ret->fd = fd;
        ret->parser = MParserNew();
        ret->data = NULL;
    }

    return (ret && ret->parser) ? ret : (free(ret), NULL);
}

int TcpClientSendMsg(TcpClient *client, message_t *msg)
{
    int ret = 0;
    Client *c = (Client *)client;

    if (c && msg) {
        int len = message_size(msg);
        char *data = (char *)message_h2n(msg);

        ret = (send(c->fd, data, len, 0) != -1);
        message_n2h(msg);
    }

    return ret;
}

int TcpClientSendRaw(TcpClient *client, char *buf, int length)
{
    int ret = 0;
    Client *c = (Client *)client;

    if (c && buf) {
        ret = send(c->fd, buf, length, 0);
    }

    return ret;
}

message_t *TcpClientRecvMsg(TcpClient *client)
{
    message_t *ret = NULL;
    Client *c = (Client *)client;

    if (c) {
        ret = MParserReadFd(c->parser, c->fd);
    }

    return ret;
}

int TcpClientRecvRaw(TcpClient *client, char *buf, int length)
{
    int ret = 0;
    Client *c = (Client *)client;

    if (c && buf) {
        ret = recv(c->fd, buf, length, 0);
    }

    return ret;
}

int TcpClientConnect(TcpClient *client, char *ip, int port)
{
    int ret = TcpClientIsValid(client);
    Client *c = (Client *)client;

    if (!ret && ip && ((c->fd = socket(PF_INET, SOCK_STREAM, 0)) != -1)) {
        struct sockaddr_in addr = {0};

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);

        ret = (connect(c->fd, (struct sockaddr *)&addr, sizeof(addr)) != -1);
    }

    return ret;
}

int TcpClientIsValid(TcpClient *client)
{
    int ret = 0;
    Client *c = (Client *)client;

    if (c) {
        struct tcp_info info = {0};
        int len = sizeof(info);

        getsockopt(c->fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
        ret = (info.tcpi_state == TCP_ESTABLISHED);
    }

    return ret;
}

void TcpClientClose(TcpClient *client)
{
    Client *c = (Client *)client;

    if (c) {
        close(c->fd);
        c->fd = -1;
        MParserReset(c->parser);
    }
}

void TcpClientDel(TcpClient *client)
{
    Client *c = (Client *)client;

    if (c) {
        TcpClientClose(c);
        MParserDel(c->parser);
        free(c);
    }
}

void TcpClientSetData(TcpClient *client, void *data)
{
    Client *c = (Client *)client;

    if (c) {
        c->data = data;
    }
}

void *TcpClientGetData(TcpClient *client)
{
    void *ret = NULL;
    Client *c = (Client *)client;

    if (c) {
        ret = c->data;
    }

    return ret;
}