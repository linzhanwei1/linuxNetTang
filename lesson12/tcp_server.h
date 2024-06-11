#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "tcp_client.h"

typedef void TcpServer;
typedef void (*Listener)(TcpClient *, int);

enum { EVT_CONN, EVT_DATA, EVT_CLOSE };

TcpServer *TcpServerNew(void);
int TcpServerStart(TcpServer *server, int port, int max);
void TcpServerStop(TcpServer *server);
void TcpServerSetListener(TcpServer *server, Listener listener);
int TcpServerIsValid(TcpServer *server);
void TcpServerDoWork(TcpServer *server);
void TcpServerDel(TcpServer *server);
#endif /* __TCP_SERVER_H__ */