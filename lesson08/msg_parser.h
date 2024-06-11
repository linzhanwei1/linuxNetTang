#ifndef __MSG_PARSER_H__
#define __MSG_PARSER_H__

#include "proto.h"

typedef void MParser;

MParser *MParserNew();
message_t *MParserReadMem(MParser *parser, unsigned char *mem, unsigned int length);
message_t *MParserReadFd(MParser *parser, int fd);
void MParserReset(MParser *parser);
void MParserDel(MParser *parser);

#endif /* __MSG_PARSER_H__ */