#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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

    asize = sizeof(caddr);

    if ((client = accept(server, (struct sockaddr *)&caddr, &asize)) < 0) {
        perror("accept()");
        close(server);
        exit(1);
    }

    printf("client: %d\n", client);
    len = 0;

    do {
        int i = 0;

        r = recv(client, buf, sizeof(buf), 0);
        if (r > 0) {
            len += r;
        }

        for (i = 0; i < r; i++) {
            printf("%c", buf[i]);
        }
        fflush(NULL);
    } while (len < 64);

    printf("\n");

    send(client, "Hello world!", 12, 0);
    sleep(1);

    close(client);
    close(server);

    exit(1);
}