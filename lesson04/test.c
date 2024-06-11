#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* 使用文件描述符操作文件读写 */
int main(int argc, char **argv)
{
    int iofd = 0;
    char s[] = "D.T.Software\n";
    int len = 0;

    write(0, s, sizeof(s));
    len = read(0, s, 5);

    s[len] = 0;
    printf("%s\n", s);

    exit(0);
}