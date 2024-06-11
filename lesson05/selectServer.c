#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 8000

int server_handler(int server)
{
    struct sockaddr_in addr = {0};
    socklen_t asize = sizeof(addr);

    return accept(server, (struct sockaddr *)&addr, &asize);
}

int client_handler(int client)
{
    char buf[32] = {0};
    int ret = read(client, buf, sizeof(buf) - 1);

    if (ret > 0) {
        buf[ret] = 0;
        printf("Receive: %s\n", buf);

        if (strcmp(buf, "quit") != 0) {
            ret = write(client, buf, ret);
        } else {
            ret = -1;
        }
    }

    return ret;
}

int main(int argc, char **argv)
{
    int server = 0;
    struct sockaddr_in saddr = {0};
    int max = 0;
    int num = 0;
    fd_set reads = {0};
    fd_set temps = {0};
    struct timeval timeout = {0};

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

    FD_ZERO(&reads);
    FD_SET(server, &reads);

    max = server;

    while (1) {
        temps = reads;

        timeout.tv_sec = 0;
        timeout.tv_usec = 10000;

        num = select(max + 1, &temps, 0, 0, &timeout);
        if (num > 0) {
            int i = 0;
            for (i = 1; i <= max; i++) {
                if (FD_ISSET(i, &temps)) {
                    if (i == server) {
                        /* 新连接到达 */
                        int client = server_handler(server);
                        if (client > -1) {
                            FD_SET(client, &reads);
                            max = (client > max) ? client : max;
                            printf("accept client: %d\n", client);
                        }
                    } else {
                        /* 数据到达 */
                        int r = client_handler(i);
                        if (r == -1) {
                            FD_CLR(i, &reads);
                            close(i);
                        }
                    }
                }
            }
        }
    }

    close(server);

    exit(1);
}