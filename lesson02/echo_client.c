#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define HOST "182.92.148.82"
#define PORT 8000

int main(int argc, char *argv[])
{
    int sock = 0;
    struct sockaddr_in addr = {0};
    char buf[128] = {0};
    char input[32] = {0};
    int r = 0, len;
    int i = 0;

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

    while (1) {
        printf("Input: ");
        scanf("%s", input);

        len = send(sock, input, strlen(input) + 1, 0);
        r = recv(sock, buf, sizeof(buf), 0);
        if (r > 0) {
            printf("Receive: %s\n", buf);
        } else {
            /* 服务器断开连接 */
            break;
        }
    }
}