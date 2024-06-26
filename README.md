# Synacor Virtual Machine Challenge
## Overview
This challenge involves creating a *virtual machine* capable of executing the provided assembly in the `challenge.bin` file on a specific architecture, specified in `arch-spec`.

The challenge encompasses more than that, including a text-based adventure game, reverse-engineering the bin file, disassembling and tracing the assembly instructions, as well as graph problems.

## Architecture
The virtual machine's architecture includes three memory regions:
> * **Memory with a 15-bit address space storing 16-bit values (little endian).**<br>
> * **Eight registers.**<br>
> * **An unbounded stack holding individual 16-bit values.**<br>

As well as 21 instructions:
```
HALT, SET, PUSH, POP, EQ, GT, JMP, JT, JF, ADD, MULT,
MOD, AND, OR, NOT, RMEM, WMEM, CALL, RET, OUT, IN, NOOP
```

For detailed specifications and opcode listing, refer to the `arch-spec` file.
