#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    int iofd = 0;
    char s[] = "D.T.Software\n";
    int len = 0;
    fd_set reads = {0};
    fd_set temps = {0};
    struct timeval timeout = {0};

    FD_ZERO(&reads);
    FD_SET(iofd, &reads);

    while (1) {
        int r = -1;

        temps = reads; // 重要
        timeout.tv_sec = 0;
        timeout.tv_usec = 50000;

        r = select(1, &temps, 0, 0, &timeout);
        if (r > 0) {
            len = read(iofd, s, sizeof(s) - 1);

            s[len] = 0;
            printf("Input: %s\n", s);
        } else if (r == 0) {
            static int count;
            usleep(10000);
            count++;
            if (count > 100) {
                printf("do something else\n");
                count = 0;
            }
        } else {
            break;
        }
    }

    exit(0);
}