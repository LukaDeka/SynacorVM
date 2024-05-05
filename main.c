#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

bool user_controlling = false;
uint16_t buffer[32768] = {0};
uint16_t registers[8] =  {0};
char moves_buffer[512] = {0};
size_t PC = 0, move_i = 0;
int ch = 0;
StackNode* top = NULL;

extern bool user_controlling;
extern uint16_t buffer[32768];
extern uint16_t registers[8];
extern size_t PC;

// fetches the value from the register, if necessary
uint16_t get_literal(uint16_t arg) {
    if (arg >= REG0 && arg <= REG7) {
        return registers[arg - REG0];
    } else if (arg >= REG7) {
        printf("ERROR: Invalid instruction.\n");
        exit(1);
    } else { return arg; }
}

// used for solving text-based game
char move_out() {
    if (user_controlling) {
        registers[7] = 1; // TODO: find this number
        return getchar();
    } else {
        ch = moves_buffer[move_i++];
        if (ch == '\0') {
            user_controlling = true;
            return getchar();
        }
        return ch;
    }
}

int main() {
    int retval = 0;

    // read files into buffer
    FILE* moves_file = fopen("moves.txt", "r");
    FILE* bin_file = fopen("challenge.bin", "r");    
    while (true) {
        int byte1 = fgetc(bin_file);
        int byte2 = fgetc(bin_file);
        if (byte1 == EOF) { break; }
        buffer[PC++] = (byte2 << 8) | byte1; // 16-bit little endian
    }
    PC = 0;

    while ((ch = fgetc(moves_file)) != EOF) {
        moves_buffer[move_i++] = ch; 
    }
    move_i = 0;
    fclose(bin_file); fclose(moves_file);

    // fprint_asm(); // update the asm.txt file
    // goto exit;

    uint16_t opcode = 0, arg1 = 0, arg2 = 0, arg3 = 0;
    while (true) {
        opcode = buffer[PC++];
        // print_ist(opcode);
        // if (user_controlling) { print_curr_state(); }
        print_ist(PC); 

        switch (opcode) {
            case OP_HALT:
                printf("HALT\n");
                goto exit;
            case OP_SET:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = arg2;
                break;
            case OP_PUSH:
                arg1 = get_literal(buffer[PC++]);
                push(arg1);
                break;
            case OP_POP:
                arg1 = buffer[PC++];
                registers[arg1 - REG0] = pop();
                break;
            case OP_EQ:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                arg3 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = arg2 == arg3;
                break;
            case OP_GT:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                arg3 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = arg2 > arg3; 
                break;
            case OP_JMP:
                arg1 = get_literal(buffer[PC++]);
                PC = arg1;
                break;
            case OP_JT:
                arg1 = get_literal(buffer[PC++]);
                arg2 = get_literal(buffer[PC++]);
                PC = arg1 != 0 ? arg2 : PC;
                break;
            case OP_JF:
                arg1 = get_literal(buffer[PC++]);
                arg2 = get_literal(buffer[PC++]);
                PC = arg1 == 0 ? arg2 : PC;
                break;
            case OP_ADD:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                arg3 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = (arg2 + arg3) % 32768;
                break;
            case OP_MULT:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                arg3 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = (arg2 * arg3) % 32768;
                break;
            case OP_MOD:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                arg3 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = arg2 % arg3;
                break;
            case OP_AND:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                arg3 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = arg2 & arg3;
                break;
            case OP_OR:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                arg3 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = arg2 | arg3;
                break;
            case OP_NOT:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = ~arg2 & 0x7FFF; // w/o MSB
                break;
            case OP_RMEM:
                arg1 = buffer[PC++];
                arg2 = get_literal(buffer[PC++]);
                registers[arg1 - REG0] = buffer[arg2];
                break;
            case OP_WMEM:
                arg1 = get_literal(buffer[PC++]);
                arg2 = get_literal(buffer[PC++]);
                buffer[arg1] = arg2;
                break;
            case OP_CALL:
                arg1 = get_literal(buffer[PC++]);
                push(PC);
                PC = arg1;
                break;
            case OP_RET:
                PC = pop();
                break;
            case OP_OUT:
                arg1 = get_literal(buffer[PC++]);
                printf("%c", arg1);
                break;
            case OP_IN:
                arg1 = buffer[PC++];
                registers[arg1 - REG0] = move_out();
                break;
            case OP_NOOP:
                break;
            default:
                printf("ERROR: Invalid opcode.\n");
                retval = 1; goto exit;
        }
    }

    exit:
    free_stack(top);
    return retval;
}
