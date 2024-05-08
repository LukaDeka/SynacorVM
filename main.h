#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    REG0 = 32768,
    REG1,
    REG2,
    REG3,
    REG4,
    REG5,
    REG6,
    REG7
} Registers;

typedef enum {
    OP_HALT, OP_SET,  OP_PUSH, OP_POP,  OP_EQ,  OP_GT,  OP_JMP,
    OP_JT,   OP_JF,   OP_ADD,  OP_MULT, OP_MOD, OP_AND, OP_OR,
    OP_NOT,  OP_RMEM, OP_WMEM, OP_CALL, OP_RET, OP_OUT, OP_IN, OP_NOOP
} Opcodes;

static char* op_arr[] = {
    "HALT", "SET",  "PUSH", "POP",  "EQ",  "GT",  "JMP",
    "JT",   "JF",   "ADD",  "MULT", "MOD", "AND", "OR",
    "NOT",  "RMEM", "WMEM", "CALL", "RET", "OUT", "IN", "NOOP"
};

typedef struct _StackNode {
    uint16_t value;
    struct _StackNode *next;
} StackNode;

extern StackNode* top;

static inline void push(uint16_t value) {
    StackNode* new = malloc(sizeof(StackNode));
    new->value = value;
    new->next = top;
    top = new;
}

static inline uint16_t pop() {
    if (top == NULL) {
        printf("ERROR: Stack is empty!\n");
        exit(1);
    }
    StackNode* temp = top;
    uint16_t ret = top->value;
    top = top->next;
    free(temp);
    return ret;
}

static inline void free_stack(StackNode* node) {
    if (node == NULL) {
        return;
    }
    StackNode* next = node->next;
    free(node);
    free_stack(next);
}

// returns either arg, or the value in a corresponding register
uint16_t get_literal(uint16_t arg);

// writes "moves.txt" char-wise to stdint
char move_out();

#endif
