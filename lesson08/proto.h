#ifndef __PROTO_H__
#define __PROTO_H__

typedef struct message {
    unsigned short type;
    unsigned short cmd;
    unsigned short index;
    unsigned short total;
    unsigned int length;
    unsigned char payload[];
} message_t;

message_t *message_new(unsigned short type, unsigned short cmd, unsigned short index, unsigned short total, unsigned int length, const char *payload);

#endif /* __PROTO_H__ */