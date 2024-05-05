#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define RED "\033[0;31m"
#define RESET "\033[0m"

typedef enum {
    OP_HALT,
    OP_SET,
    OP_PUSH,
    OP_POP,
    OP_EQ,
    OP_GT,
    OP_JMP,
    OP_JT,
    OP_JF,
    OP_ADD,
    OP_MULT,
    OP_MOD,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_RMEM,
    OP_WMEM,
    OP_CALL,
    OP_RET,
    OP_OUT,
    OP_IN,
    OP_NOOP
} Opcodes;

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

typedef struct _StackNode {
    uint16_t value;
    struct _StackNode *next;
} StackNode;

extern bool user_controlling;
extern uint16_t buffer[32768];
extern uint16_t registers[8];
extern size_t PC;
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

// get argument in string format
static inline char* get_arg(uint16_t num) {
    char* ret = malloc(6 * sizeof(char));
    if (num >= REG0 && num <= REG7) {
        sprintf(ret, "REG%d", num - REG0);
    } else {
        sprintf(ret, "%d", num);
    }
    return ret;
}

static inline void print_ist(uint16_t PC) {
    char *arg1, *arg2, *arg3;
    uint16_t opcode = buffer[PC - 1];
    switch(opcode) {
        case OP_HALT:
            printf(RED "HALT " RESET);
            break;
        case OP_SET:
            printf(RED "SET " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            printf("%s %s", arg1, arg2);
            free(arg1); free(arg2);
            break;
        case OP_PUSH:
            printf(RED "PUSH " RESET);
            arg1 = get_arg(buffer[PC]);
            printf("%s", arg1);
            free(arg1);
            break;
        case OP_POP:
            printf(RED "POP " RESET);
            arg1 = get_arg(buffer[PC]);
            printf("%s", arg1);
            free(arg1);
            break;
        case OP_EQ:
            printf(RED "EQ " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            arg3 = get_arg(buffer[PC + 2]);
            printf("%s %s %s", arg1, arg2, arg3);
            free(arg1); free(arg2); free(arg3);
            break;
        case OP_GT:
            printf(RED "GT " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            arg3 = get_arg(buffer[PC + 2]);
            printf("%s %s %s", arg1, arg2, arg3);
            free(arg1); free(arg2); free(arg3);
            break;
        case OP_JMP:
            printf(RED "JMP " RESET);
            arg1 = get_arg(buffer[PC]);
            printf("%s", arg1);
            free(arg1);
            break;
        case OP_JT:
            printf(RED "JT " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            printf("%s %s", arg1, arg2);
            free(arg1); free(arg2);
            break;
        case OP_JF:
            printf(RED "JF " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            printf("%s %s", arg1, arg2);
            free(arg1); free(arg2);
            break;
        case OP_ADD:
            printf(RED "ADD " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            arg3 = get_arg(buffer[PC + 2]);
            printf("%s %s %s", arg1, arg2, arg3);
            free(arg1); free(arg2); free(arg3);
            break;
        case OP_MULT:
            printf(RED "MULT " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            arg3 = get_arg(buffer[PC + 2]);
            printf("%s %s %s", arg1, arg2, arg3);
            free(arg1); free(arg2); free(arg3);
            break;
        case OP_MOD:
            printf(RED "MOD " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            arg3 = get_arg(buffer[PC + 2]);
            printf("%s %s %s", arg1, arg2, arg3);
            free(arg1); free(arg2); free(arg3);
            break;
        case OP_AND:
            printf(RED "AND " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            arg3 = get_arg(buffer[PC + 2]);
            printf("%s %s %s", arg1, arg2, arg3);
            free(arg1); free(arg2); free(arg3);
            break;
        case OP_OR:
            printf(RED "OR " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            arg3 = get_arg(buffer[PC + 2]);
            printf("%s %s %s", arg1, arg2, arg3);
            free(arg1); free(arg2); free(arg3);
            break;
        case OP_NOT:
            printf(RED "NOT " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            printf("%s %s", arg1, arg2);
            free(arg1); free(arg2);
            break;
        case OP_RMEM:
            printf(RED "RMEM " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            printf("%s %s", arg1, arg2);
            free(arg1); free(arg2);
            break;
        case OP_WMEM:
            printf(RED "WMEM " RESET);
            arg1 = get_arg(buffer[PC]);
            arg2 = get_arg(buffer[PC + 1]);
            printf("%s %s", arg1, arg2);
            free(arg1); free(arg2);
            break;
        case OP_CALL:
            printf(RED "CALL " RESET);
            arg1 = get_arg(buffer[PC]);
            printf("%s", arg1);
            free(arg1);
            break;
        case OP_RET:
            printf(RED "RET " RESET);
            break;
        case OP_OUT:
            printf(RED "OUT " RESET);
            arg1 = get_arg(buffer[PC]);
            printf("%s", arg1);
            free(arg1);
            break;
        case OP_IN:
            printf(RED "IN " RESET);
            arg1 = get_arg(buffer[PC]);
            printf("%s", arg1);
            free(arg1);
            break;
        case OP_NOOP:
            printf(RED "NOOP " RESET);
            break;
        default:
            printf("Semantic error calling print_ist!\n");
            break;
    }
    printf("\n");
    return;
}

static inline void print_curr_state() {
    // printf("\n\nNext instruction:\n");
    print_ist(PC);

    // print registers
    printf("\n");
    for (size_t i = 0; i < 8; i++) {
        printf(RED "REG%zu=%d  " RESET, i, registers[i]);
    }
    printf("\n");
}

static inline void fprint_arg(uint16_t num, FILE* asm_file) {
    if (num >= REG0 && num <= REG7) {
        fprintf(asm_file, "REG%d,\t", num - REG0);
    } else {
        fprintf(asm_file, "%d,\t", num);
    }
}

static inline void fprint_asm() {
    FILE* asm_file = fopen("asm.txt", "w");

    while(1) {
        fprintf(asm_file, "%05zu: ", PC);
        uint16_t opcode = buffer[PC++];

        switch(opcode) {
        case OP_HALT:
            fprintf(asm_file, "HALT\t\t");
            fprintf(asm_file, "\n");
            break;
        case OP_SET:
            fprintf(asm_file, "SET\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_PUSH:
            fprintf(asm_file, "PUSH\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_POP:
            fprintf(asm_file, "POP\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_EQ:
            fprintf(asm_file, "EQ\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_GT:
            fprintf(asm_file, "GT\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_JMP:
            fprintf(asm_file, "JMP\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_JT:
            fprintf(asm_file, "JT\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_JF:
            fprintf(asm_file, "JF\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_ADD:
            fprintf(asm_file, "ADD\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_MULT:
            fprintf(asm_file, "MULT\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_MOD:
            fprintf(asm_file, "MOD\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_AND:
            fprintf(asm_file, "AND\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_OR:
            fprintf(asm_file, "OR\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_NOT:
            fprintf(asm_file, "NOT\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_RMEM:
            fprintf(asm_file, "RMEM\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_WMEM:
            fprintf(asm_file, "WMEM\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_CALL:
            fprintf(asm_file, "CALL\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_RET:
            fprintf(asm_file, "RET\t\t");
            fprintf(asm_file, "\n");
            break;
        case OP_OUT:
            fprintf(asm_file, "OUT\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_IN:
            fprintf(asm_file, "IN\t\t");
            fprint_arg(buffer[PC++], asm_file);
            fprintf(asm_file, "\n");
            break;
        case OP_NOOP:
            fprintf(asm_file, "NOOP\t\t");
            fprintf(asm_file, "\n");
            break;
        default:
            fprintf(asm_file, "NUM: %d", opcode);
            fprintf(asm_file, "\n");
        }
        if (PC >= 29957) {
            free_stack(top);
            fclose(asm_file);
            exit(0);
        }
    }
}

// returns either arg, or the value in a corresponding register
uint16_t get_literal(uint16_t arg);

// writes "moves.txt" char-wise to stdint
char move_out();
