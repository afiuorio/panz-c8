#include "cpu.h"
#include <string.h>

void execute0NNN(WORD opcode, c8_cpu *cpu){

}

void execute00E0(WORD opcode, c8_cpu *cpu){
    memset(cpu->video_memory, 0, 32*64);
}

void execute00EE(WORD opcode, c8_cpu *cpu){
    WORD val;
    POP(cpu->stack, val);
    cpu->prog_counter = val;
}

void execute1NNN(WORD opcode, c8_cpu *cpu){
    WORD addr = opcode &0xFFF;
    cpu->prog_counter = addr;
}

void execute2NNN(WORD opcode, c8_cpu *cpu){
    WORD addr = opcode & 0xFFF;
    PUSH(cpu->stack, cpu->prog_counter);
    cpu->prog_counter = addr;
}

void execute3XNN(WORD opcode, c8_cpu *cpu){
    WORD register_num = SEC_HEX(opcode);
    WORD value = opcode & 0xFF;
    if(cpu->registers[register_num] == value)
        cpu->prog_counter+=2;
}

void execute4XNN(WORD opcode, c8_cpu *cpu){
    WORD register_num = SEC_HEX(opcode);
    WORD value = opcode & 0xFF;
    if(cpu->registers[register_num] != value)
        cpu->prog_counter+=2;
}

void execute5XY0(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    if(cpu->registers[fir] == cpu->registers[sec])
        cpu->prog_counter+=2;
}

void execute6XNN(WORD opcode, c8_cpu *cpu){
    WORD register_num = SEC_HEX(opcode);
    WORD value = opcode & 0xFF;
    cpu->registers[register_num] = value;
}

void execute7XNN(WORD opcode, c8_cpu *cpu){
    WORD register_num = SEC_HEX(opcode);
    WORD value = opcode & 0xFF;
    cpu->registers[register_num] += value;
}

void execute8XY0(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    cpu->registers[fir] = cpu->registers[sec];
}

void execute8XY1(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    cpu->registers[fir] |= cpu->registers[sec];
}

void execute8XY2(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    cpu->registers[fir] &= cpu->registers[sec];
}

void execute8XY3(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    cpu->registers[fir] ^= cpu->registers[sec];
}

void execute8XY4(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    cpu->registers[15] = 0;

    int value = cpu->registers[fir] + cpu->registers[sec];
    if(value > 255)
        cpu->registers[15] = 1;

    cpu->registers[fir] += cpu->registers[sec];
}

void execute8XY5(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    cpu->registers[15] = 1;

    if(cpu->registers[fir] < cpu->registers[sec])
        cpu->registers[15] = 0;

    cpu->registers[fir] -= cpu->registers[sec];
}

void execute8XY6(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    cpu->registers[15] = cpu->registers[fir] & 0x1;
    cpu->registers[fir] >>=1;
}

void execute8XY7(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    cpu->registers[15] = 1;

    if(cpu->registers[sec] < cpu->registers[fir])
        cpu->registers[15] = 0;

    cpu->registers[fir] = cpu->registers[sec] - cpu->registers[fir] ;
}

void execute8XYE(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    cpu->registers[15] = cpu->registers[fir] >>7;
    cpu->registers[fir] <<=1;
}

void execute9XY0(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    if(cpu->registers[fir] != cpu->registers[sec])
        cpu->prog_counter+=2;
}

void executeANNN(WORD opcode, c8_cpu *cpu){
    WORD value = opcode & 0xFFF;
    cpu->mem_register = value;
}

void executeBNNN(WORD opcode, c8_cpu *cpu){
    WORD value = opcode & 0xFFF;
    cpu->prog_counter = value + cpu->registers[0];
}

void executeCXNN(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD mask = opcode & 0xFF;

    cpu->registers[fir] = cpu->randomByte & mask; //TODO: stub, should be random
}

void executeDXYN(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    WORD sec = THIRD_HEX(opcode);
    
    int coordx = cpu->registers[fir];
    int coordy = cpu->registers[sec];
    int height = opcode & 0xF ;
    
    cpu->registers[15] = 0 ;
    
    for (int yline = 0; yline < height; yline++)
    {
        BYTE data = cpu->memory[cpu->mem_register + yline];

        int xpixel = 0 ;
        int xpixelinv = 7 ;
        for(xpixel = 0; xpixel < 8; xpixel++, xpixelinv--)
        {
            BYTE work = data >> xpixelinv;
            work &= 0x1;
            int x = xpixel + coordx ;
            int y = coordy + yline ;
            BYTE last = cpu->video_memory[y][x];
            cpu->video_memory[y][x] ^= work;
            if((last == 1) && (cpu->video_memory[y][x] ==0) )
                cpu->registers[15] = 1;
        }
    }
}

void executeEX9E(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    BYTE val = cpu->registers[fir];
    if(cpu->keymap[val] == 1)
        cpu->prog_counter+=2;
}

void executeEXA1(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    BYTE val = cpu->registers[fir];
    if(cpu->keymap[val] == 0)
        cpu->prog_counter+=2;
}

void executeFX07(WORD opcode, c8_cpu *cpu){
    WORD val = SEC_HEX(opcode);
    cpu->registers[val] = cpu->timer;
}

void executeFX0A(WORD opcode, c8_cpu *cpu){
    WORD val = SEC_HEX(opcode);
    int i;
    
    for (i = 0 ; i < 16; i++)
    {
        if (cpu->keymap[i] > 0)
            break;
    }
    
    if (i == 16)
        cpu->prog_counter -= 2 ;
    else
        cpu->registers[val] = i;
}

void executeFX15(WORD opcode, c8_cpu *cpu){
    WORD val = SEC_HEX(opcode);
    cpu->timer = cpu->registers[val];
}

void executeFX18(WORD opcode, c8_cpu *cpu){
    WORD val = SEC_HEX(opcode);
    cpu->soundTimer = cpu->registers[val];
}

void executeFX1E(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    cpu->mem_register += cpu->registers[fir];
}

void executeFX29(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    cpu->mem_register = cpu->registers[fir]*5;
}

void executeFX33(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);

    BYTE val = cpu->registers[fir];
    cpu->memory[cpu->mem_register] = val/100;
    cpu->memory[cpu->mem_register +1] = (val/10) % 10;
    cpu->memory[cpu->mem_register +2] = val % 10;
}

void executeFX55(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    int i;
    for(i=0; i <= fir; i++)
        cpu->memory[cpu->mem_register + i] = cpu->registers[i];
    cpu->mem_register += fir;
    cpu->mem_register += 1;
}

void executeFX65(WORD opcode, c8_cpu *cpu){
    WORD fir = SEC_HEX(opcode);
    int i;
    for(i=0; i <= fir; i++)
        cpu->registers[i] = cpu->memory[cpu->mem_register + i];
    cpu->mem_register += fir;
    cpu->mem_register += 1;
}
