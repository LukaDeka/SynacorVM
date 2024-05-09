#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RED "\033[0;31m"
#define RESET "\033[0m"

extern FILE* asm_live_file; 
extern uint16_t buffer[32768];
extern uint16_t registers[8];
extern size_t PC;

void dump_asm();

void dump_live_asm();

#endif
