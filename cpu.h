#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef uint8_t BYTE;
typedef uint16_t WORD;

#define MEM_SIZE 0xFFF

#define FIRST_HEX(a) ((a & 0xF000) >> 12)
#define SEC_HEX(a) ((a & 0xF00) >> 8)
#define THIRD_HEX(a) ((a & 0xF0) >> 4)
#define FOUR_HEX(a) ((a & 0xF))

#define PUSH(base, l) *(base--) = l;
#define POP(base, l) l = (*++base);

// 76543210
#define GET_BIT(byte, numBit) (byte & (0x1 << numBit)) >> numBit

typedef struct c8_cpu {
    BYTE memory[MEM_SIZE];
    BYTE registers[16];
    WORD mem_register;
    WORD prog_counter;

    WORD timer;
    WORD soundTimer;

    WORD *stack;

    /*All these variables should me mapped on memory*/

    BYTE keymap[16];
    BYTE video_memory[32][64];
    BYTE randomByte;
} c8_cpu;

void initCPU(c8_cpu *cpu);
void updateTimer(c8_cpu *cpu);
void setRandomByte(c8_cpu *cpu, BYTE value);

WORD fetch(c8_cpu *cpu);
void decode(WORD opcode, c8_cpu *cpu);

void execute8Opc(WORD opcode, c8_cpu *cpu);
void execute0Opc(WORD opcode, c8_cpu *cpu);
void executeEOpc(WORD opcode, c8_cpu *cpu);
void executeFOpc(WORD opcode, c8_cpu *cpu);

void execute0NNN(WORD opcode, c8_cpu *cpu);
void execute00E0(WORD opcode, c8_cpu *cpu);
void execute00EE(WORD opcode, c8_cpu *cpu);
void execute1NNN(WORD opcode, c8_cpu *cpu);
void execute2NNN(WORD opcode, c8_cpu *cpu);
void execute3XNN(WORD opcode, c8_cpu *cpu);
void execute4XNN(WORD opcode, c8_cpu *cpu);
void execute5XY0(WORD opcode, c8_cpu *cpu);
void execute6XNN(WORD opcode, c8_cpu *cpu);
void execute7XNN(WORD opcode, c8_cpu *cpu);
void execute8XY0(WORD opcode, c8_cpu *cpu);
void execute8XY1(WORD opcode, c8_cpu *cpu);
void execute8XY2(WORD opcode, c8_cpu *cpu);
void execute8XY3(WORD opcode, c8_cpu *cpu);
void execute8XY4(WORD opcode, c8_cpu *cpu);
void execute8XY5(WORD opcode, c8_cpu *cpu);
void execute8XY6(WORD opcode, c8_cpu *cpu);
void execute8XY7(WORD opcode, c8_cpu *cpu);
void execute8XYE(WORD opcode, c8_cpu *cpu);
void execute9XY0(WORD opcode, c8_cpu *cpu);
void executeANNN(WORD opcode, c8_cpu *cpu);
void executeBNNN(WORD opcode, c8_cpu *cpu);
void executeCXNN(WORD opcode, c8_cpu *cpu);
void executeDXYN(WORD opcode, c8_cpu *cpu);
void executeEX9E(WORD opcode, c8_cpu *cpu);
void executeEXA1(WORD opcode, c8_cpu *cpu);
void executeFX07(WORD opcode, c8_cpu *cpu);
void executeFX0A(WORD opcode, c8_cpu *cpu);
void executeFX15(WORD opcode, c8_cpu *cpu);
void executeFX18(WORD opcode, c8_cpu *cpu);
void executeFX1E(WORD opcode, c8_cpu *cpu);
void executeFX29(WORD opcode, c8_cpu *cpu);
void executeFX33(WORD opcode, c8_cpu *cpu);
void executeFX55(WORD opcode, c8_cpu *cpu);
void executeFX65(WORD opcode, c8_cpu *cpu);

#endif
