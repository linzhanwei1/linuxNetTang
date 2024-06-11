#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr = {0};

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket()");
        exit(1);
    }

    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port = htons(8000);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("connect()");
        exit(1);
    }

    printf("connect success\n");
    close(sock);

    exit(0);
}
