#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "msg_parser.h"

typedef struct msg_parser {
    message_t cache; // 缓存已解析的消息头
    int header;      // 标识消息头是否解析成功
    int need;        // 标识还需要多少字节才能完成解析
    message_t *msg;  // 解析中的协议消息(半成品)
} MsgParser;

static void InitState(MsgParser *p)
{
    p->header = 0;
    p->need = sizeof(p->cache);

    if (p->msg)
        free(p->msg);

    p->msg = NULL;
}

static int ToMidState(MsgParser *p)
{
    p->header = 1;
    p->need = p->cache.length;

    p->msg = malloc(sizeof(p->cache) + p->need);
    if (p->msg) {
        *p->msg = p->cache;
    }

    return !!p->msg;
}

static message_t *ToLastState(MsgParser *p)
{
    message_t *ret = NULL;

    if (p->header && !p->need) {
        ret = p->msg;
        p->msg = NULL;
    }

    return ret;
}

static void ntoh(message_t *m)
{
    m->type = ntohs(m->type);
    m->cmd = ntohs(m->cmd);
    m->index = ntohs(m->index);
    m->total = ntohs(m->total);
    m->length = ntohl(m->length);
}

static int ToRecv(int fd, char *buf, int size)
{
    int retry = 0;
    int i = 0;

    while (i < size) {
        int len = read(fd, buf + i, size - i);
        if (len > 0) {
            i += len;
        } else if (len < 0) {
            break;
        } else {
            if (retry++ > 5) {
                break;
            }
            usleep(200 * 1000);
        }
    }
}

MParser *MParserNew()
{
    MParser *ret = calloc(1, sizeof(MsgParser)); //calloc会将新申请的内存块清零

    if (ret) {
        InitState(ret);
    }

    return ret;
}
message_t *MParserReadMem(MParser *parser, unsigned char *mem, unsigned int length)
{
    message_t *ret = NULL;
    MsgParser *p = (MsgParser *)parser;

    if (p && mem && length) {
        if (!p->header) {
            // 解析消息头
            int len = (p->need < length) ? p->need : length;
            int offset = sizeof(p->cache) - p->need;

            memcpy((char *)&p->cache + offset, mem, len);

            if (p->need == len) {
                message_n2h(&p->cache);

                mem += p->need;
                length -= p->need;

                if (ToMidState(p)) {
                    ret = MParserReadMem(p, mem, length);
                } else {
                    InitState(p);
                }
            } else {
                p->need -= len;
            }
        } else {
            if (p->msg) {
                unsigned int len = (p->need < length) ? p->need : length;
                unsigned int offset = p->msg->length - p->need;

                memcpy(p->msg->payload, mem, len);
                p->need -= len;
            }

            if (ret = ToLastState(p)) {
                InitState(p);
            }
        }
    }

    return ret;
}

message_t *MParserReadFd(MParser *parser, int fd)
{
    message_t *ret = NULL;
    MsgParser *p = (MsgParser *)parser;

    if ((fd != -1) && p) {
        if (!p->header) {
            /* 先解析消息头 */
            int offset = sizeof(p->cache) - p->need;
            int len = ToRecv(fd, (char *)&p->cache + offset, p->need);

            if (len == p->need) {
                /* 消息头接收完成 */
                message_n2h(&p->cache);
                if (ToMidState(p)) {
                    ret = MParserReadFd(p, fd);
                } else {
                    InitState(p);
                }
            } else {
                /* 继续收数据 */
                p->need -= len;
            }
        } else {
            /* 解析payload */
            if (p->msg) {
                int offset = p->msg->length - p->need;
                int len = ToRecv(fd, p->msg->payload + offset, p->need);
                p->need -= len;
            }

            if (ret = ToLastState(p)) {
                InitState(p);
            }
        }
    }

    return ret;
}
void MParserReset(MParser *parser)
{
    MsgParser *p = (MsgParser *)parser;

    if (p) {
        InitState(p);
    }
}
void MParserDel(MParser *parser)
{
    MsgParser *p = (MsgParser *)parser;

    if (p) {
        if (p->msg) {
            free(p->msg);
            free(p);
        }
    }
}