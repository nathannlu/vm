#ifndef OP_CODE_H
#define OP_CODE_H

#define OP_HALT 0x00
#define OP_CONST 0x01
#define OP_ADD 0x02
#define OP_SUB 0x03
#define OP_MUL 0x04
#define OP_DIV 0x05

#define OP_COMPARE 0x06

#define OP_JMP_IF_FALSE 0x07
#define OP_JMP 0x08

#define OP_GET_GLOBAL 0x09
#define OP_SET_GLOBAL 0x10

#endif 