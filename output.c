#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "output.h"
#include "main.h"

void fprint_arg(uint16_t num, FILE* asm_file) {
    if (num >= REG0 && num <= REG7) {
        fprintf(asm_file, "REG%d\t", num - REG0);
    } else {
        fprintf(asm_file, "%05d\t", num);
    }
}

void fprint_asm_line(FILE* fp) {
    fprintf(fp, "%05zu: ", PC);
    uint16_t opcode = buffer[PC++];

    switch(opcode) {
    case OP_HALT:
        fprintf(fp, "%s\t\t-----------------------", op_arr[opcode]);
        fprintf(fp, "\n");
        break;
    case OP_SET:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_PUSH:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_POP:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_EQ:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_GT:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_JMP:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_JT:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_JF:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_ADD:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_MULT:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_MOD:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_AND:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_OR:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_NOT:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_RMEM:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_WMEM:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_CALL:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_RET:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprintf(fp, "\n");
        break;
    case OP_OUT:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        char ch = buffer[PC-1];
        fprintf(fp, "\t\t\t(%c)\n", isprint(ch) ? ch : '.');
        break;
    case OP_IN:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprint_arg(buffer[PC++], fp);
        fprintf(fp, "\n");
        break;
    case OP_NOOP:
        fprintf(fp, "%s\t\t", op_arr[opcode]);
        fprintf(fp, "\n");
        break;
    default:
        fprintf(fp, "NUM: %05d", opcode);
        fprintf(fp, "\n");
    }
}

void dump_asm() {
    FILE* asm_file = fopen("./etc/asm.txt", "w");
    while (PC < 29957) {
        fprint_asm_line(asm_file);
    }
    free_stack(top);
    fclose(asm_file);
    exit(0);
}

void dump_live_asm() {
    uint16_t temp_PC = PC;
    fprint_asm_line(asm_live_file);
    PC = temp_PC;
}
