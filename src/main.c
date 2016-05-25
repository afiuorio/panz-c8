#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

#define RES_W 64
#define RES_H 32

void loadROM(c8_cpu *cpu, char *rom) {
    FILE *f = fopen(rom, "rb");
    if (!f) {
        fprintf(stderr, "Can't load ROM\n");
        exit(2);
    }
    fread(cpu->memory + 0x200, 1, MEM_SIZE - 0x200, f);
    fclose(f);
}

void renderVideoMemory(c8_cpu *cpu, SDL_Renderer *rend, int SCALE) {
    int i = 0, j = 0;
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
    SDL_RenderClear(rend);
    SDL_Rect rectangle;
    rectangle.w = SCALE;
    rectangle.h = SCALE;
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
    for (i = 0; i < 32; i += 1)
        for (j = 0; j < 64; j += 1)
            if (cpu->video_memory[i][j] != 0) {
                rectangle.x = j * SCALE;
                rectangle.y = i * SCALE;
                SDL_RenderFillRect(rend, &rectangle);
            }
    SDL_RenderPresent(rend);
}

void getInput(c8_cpu *cpu) {
    SDL_Event event;
    int value, key;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            exit(EXIT_SUCCESS);
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_x:
                key = 0;
                break;
            case SDLK_1:
                key = 1;
                break;
            case SDLK_2:
                key = 2;
                break;
            case SDLK_3:
                key = 3;
                break;
            case SDLK_q:
                key = 4;
                break;
            case SDLK_w:
                key = 5;
                break;
            case SDLK_e:
                key = 6;
                break;
            case SDLK_a:
                key = 7;
                break;
            case SDLK_s:
                key = 8;
                break;
            case SDLK_d:
                key = 9;
                break;
            case SDLK_z:
                key = 10;
                break;
            case SDLK_c:
                key = 11;
                break;
            case SDLK_4:
                key = 12;
                break;
            case SDLK_r:
                key = 13;
                break;
            case SDLK_f:
                key = 14;
                break;
            case SDLK_v:
                key = 15;
                break;
            default:
                key = -1;
                break;
            }
            if (event.type == SDL_KEYDOWN)
                value = 1;
            else
                value = 0;

            if (key != -1)
                cpu->keymap[key] = value;
        }
    }
}

void handleArguments(char *configFile, unsigned int *configOption,
                     c8_cpu *cpu) {
    FILE *config = fopen(configFile, "rt");
    if (!config) {
        fprintf(stderr, "Can't load config.cfg file\n");
        exit(EXIT_FAILURE);
    }

    char buffer[128];

    while (fscanf(config, "%s128\n", buffer) == 1) {
        char *variableName = strtok(buffer, "*");
        if (strcmp(variableName, "SCALE") == 0) {
            configOption[0] = atoi(strtok(NULL, "*"));
        } else if (strcmp(variableName, "OPCODES") == 0) {
            configOption[1] = atoi(strtok(NULL, "*"));
        }
    }
}

void initConfig(unsigned int *config) {
    config[0] = 1;
    config[1] = 400;
}

int main(int argc, char **argv) {
    c8_cpu c8;
    SDL_Renderer *renderer;
    SDL_Window *window;
    /*configOption:
     * 0 is the resolution scale
     * 1 is the number of opcode per second
     * 2 is a flag for the sound
     */
    unsigned int configOption[3];
    unsigned int *SCALE = configOption;
    unsigned int *opcodePerSec = configOption + 1;

    initConfig(configOption);
    initCPU(&c8);
    SDL_Init(SDL_INIT_EVERYTHING);

    handleArguments("config.cfg", configOption, &c8);
    loadROM(&c8, argv[1]);

    window = SDL_CreateWindow("panz-c8", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, RES_W * *SCALE,
                              RES_H * *SCALE, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    while (1) {
        unsigned int timeStartFrame = SDL_GetTicks();
        for (int i = 0; i < *opcodePerSec / 60; i++) {
            getInput(&c8);
            WORD opcode = fetch(&c8);
            decode(opcode, &c8);
            setRandomByte(&c8, SDL_GetTicks() & 0xFF);
        }
        updateTimer(&c8);
        renderVideoMemory(&c8, renderer, *SCALE);
        float deltaT =
            (float)1000 / 60 - (float)(SDL_GetTicks() - timeStartFrame);
        if (deltaT > 0)
            SDL_Delay((unsigned int)deltaT);
    }
}
