#pragma once
#include <Arduino.h>

//manter apenas os seguitntes campos
// uint8_t pinoGPIO; uint8_t value;
struct Botao {
    uint8_t pinoGPIO;
    uint8_t value;
};

extern Botao botaoDS;

void button_configure(Botao& botao, uint8_t modo);
bool button_is_pressed(Botao& botao);
