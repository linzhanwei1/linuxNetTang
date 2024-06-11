#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    unsigned int addr = inet_addr("1.2.3.4");
    struct in_addr addr1 = {0x09080706};
    struct in_addr addr2 = {0x05040302};
    char *s1 = inet_ntoa(addr1);
    char *s2 = inet_ntoa(addr2);
    char buf1[INET6_ADDRSTRLEN] = {0};
    char buf2[INET_ADDRSTRLEN] = {0};

    printf("addr = %x\n", addr);
    printf("s1 = %s\n", s1);
    printf("s2 = %s\n", s2);

    if (inet_aton("4.3.2.1", &addr1)) { // 判断IP地址是否合法
        printf("addr1 = %x\n", addr1.s_addr);
    }

    exit(0);
}