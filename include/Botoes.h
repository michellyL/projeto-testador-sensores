#pragma once
#include <Arduino.h>
#include "Config.h"

struct Botao {
    uint8_t pinoGPIO;
    bool leituraAnterior;
    bool estadoEstavel;
    unsigned long instanteUltimaMudanca;
};

void button_configure(Botao& botao, uint8_t modo);
bool button_read(const Botao& botao);
