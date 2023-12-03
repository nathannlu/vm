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

#define OP_CALL 0x11

#define OP_GET_LOCAL 0x12
#define OP_SET_LOCAL 0x13

#define OP_SCOPE_EXIT 0x14

#define OP_RETURN 0x15

#define OP_POP 0x16

#endif 
