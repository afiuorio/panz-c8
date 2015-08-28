#include "cpu.h"
#include <string.h>
#include <stdio.h>

void initCPU(c8_cpu *cpu){
    unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    
    memset(cpu->memory, 0, MEM_SIZE);
    memcpy(cpu->memory,chip8_fontset, 80);
    memset(cpu->registers, 0, 16);
    cpu->mem_register = 0;
    cpu->prog_counter = 0x200;
    cpu->stack = (WORD *)(cpu->memory + 0xEFF);
    memset(cpu->keymap, 0, 16);
    memset(cpu->video_memory, 0, 64*32);
    
    cpu->timer = 0;
    cpu->soundTimer = 0;
    cpu->randomByte = 0;
}

void setRandomByte(c8_cpu *cpu, BYTE value){
    cpu->randomByte = value;
}

WORD fetch(c8_cpu *cpu){
    WORD opcode = 0;
    opcode = cpu->memory[cpu->prog_counter];
    opcode <<= 8;
    opcode |= cpu->memory[cpu->prog_counter+1];
    cpu->prog_counter+=2;

    return opcode;
}

void decode(WORD opcode, c8_cpu *cpu){
    WORD first = FIRST_HEX(opcode);
    switch(first){
        case 0x0: execute0Opc(opcode, cpu); break;
        case 0x1: execute1NNN(opcode, cpu); break;
        case 0x2: execute2NNN(opcode, cpu); break;
        case 0x3: execute3XNN(opcode, cpu); break;
        case 0x4: execute4XNN(opcode, cpu); break;
        case 0x5: execute5XY0(opcode, cpu); break;
        case 0x6: execute6XNN(opcode, cpu); break;
        case 0x7: execute7XNN(opcode, cpu); break;
        case 0x8: execute8Opc(opcode, cpu); break;
        case 0x9: execute9XY0(opcode, cpu); break;
        case 0xA: executeANNN(opcode, cpu); break;
        case 0xB: executeBNNN(opcode, cpu); break;
        case 0xC: executeCXNN(opcode, cpu); break;
        case 0xD: executeDXYN(opcode, cpu); break;
        case 0xE: executeEOpc(opcode, cpu); break;
        case 0xF: executeFOpc(opcode, cpu); break;
        default: break;
    };
}

void execute0Opc(WORD opcode, c8_cpu *cpu){
    WORD second = SEC_HEX(opcode);

    if(second == 0){
        WORD four = FOUR_HEX(opcode);
        if(four == 0)
            execute00E0(opcode, cpu);
        else
            execute00EE(opcode, cpu);
        }
    else
        execute0NNN(opcode, cpu);
}

void execute8Opc(WORD opcode, c8_cpu *cpu){
    WORD four = FOUR_HEX(opcode);
    switch(four){
        case 0x0: execute8XY0(opcode,cpu); break;
        case 0x1: execute8XY1(opcode,cpu); break;
        case 0x2: execute8XY2(opcode,cpu); break;
        case 0x3: execute8XY3(opcode,cpu); break;
        case 0x4: execute8XY4(opcode,cpu); break;
        case 0x5: execute8XY5(opcode,cpu); break;
        case 0x6: execute8XY6(opcode,cpu); break;
        case 0x7: execute8XY7(opcode,cpu); break;
        case 0xE: execute8XYE(opcode,cpu); break;
        default: break;
    };
}

void executeEOpc(WORD opcode, c8_cpu *cpu){
    WORD third = THIRD_HEX(opcode);
    switch(third){
        case 0x9: executeEX9E(opcode,cpu); break;
        case 0xA: executeEXA1(opcode,cpu); break;
        default: break;
    };
}

void executeFOpc(WORD opcode, c8_cpu *cpu){
    WORD value = opcode & 0xFF;
    switch(value){
        case 0x07: executeFX07(opcode,cpu); break;
        case 0x0A: executeFX0A(opcode,cpu); break;
        case 0x15: executeFX15(opcode,cpu); break;
        case 0x18: executeFX18(opcode,cpu); break;
        case 0x1E: executeFX1E(opcode,cpu); break;
        case 0x29: executeFX29(opcode,cpu); break;
        case 0x33: executeFX33(opcode,cpu); break;
        case 0x55: executeFX55(opcode,cpu); break;
        case 0x65: executeFX65(opcode,cpu); break;
        default: break;
    }
}

void updateTimer(c8_cpu *cpu){
    if (cpu->timer > 0)
        cpu->timer-- ;
    if (cpu->soundTimer > 0)
        cpu->soundTimer--;
}
