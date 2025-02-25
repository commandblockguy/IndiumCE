#ifndef AST_H
#define AST_H

#include "types.h"

enum etype {
    ET_NUMBER,
    ET_COMPLEX,
    ET_VARIABLE,

    ET_STRING,
    ET_TEMP_STRING,
    ET_EQU,

    ET_LIST,
    ET_TEMP_LIST,
    ET_TEMP_LIST_COMPLEX,
    ET_CUSTOM_LIST,

    ET_MATRIX,
    ET_TEMP_MATRIX,

    ET_OPERATOR,
    ET_FUNCTION_CALL,   // The difference between a function call and a command is that a function call can be used in
    ET_COMMAND          // an expression, whereas a command should be the first token on a line.
};

union operand_t {
    // OS variables
    uint8_t variableNr;
    uint8_t stringNr;
    uint8_t equationNr;
    uint8_t listNr;
    uint8_t customListNr;
    uint8_t matrixNr;

    // Internal
    uint8_t op;
    unsigned int func;
    unsigned int command;

    // These are the main types, everything will resolve to one of these types
    Number *num;
    Complex *cplx;
    List *list;
    ComplexList *complexList;
    Matrix *matrix;
    String *string;
};

struct element_t {
    enum etype type;
    union operand_t operand;
};

struct NODE {
    struct NODE *next;
    struct NODE *child;
    struct element_t data;
};

void nodeFree(struct NODE *node);

#endif
