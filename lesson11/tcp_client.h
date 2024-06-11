#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "proto.h"

typedef void TcpClient;

TcpClient *TcpClientNew(void);
TcpClient *TcpClientFrom(int fd);
int TcpClientSendMsg(TcpClient *client, message_t *msg);
int TcpClientSendRaw(TcpClient *client, char *buf, int length);
message_t *TcpClientRecvMsg(TcpClient *client);
int TcpClientRecvRaw(TcpClient *client, char *buf, int length);
int TcpClientConnect(TcpClient *client, char *ip, int port);
int TcpClientIsValid(TcpClient *client);
void TcpClientClose(TcpClient *client);
void TcpClientDel(TcpClient *client);
void TcpClientSetData(TcpClient *client, void *data);
void *TcpClientGetData(TcpClient *client);
#endif /* __TCP_CLIENT_H__ */