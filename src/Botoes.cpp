#include "Botoes.h"

bool button_is_pressed(Botao& botao) {
    botao.value = digitalRead(botao.pinoGPIO);
    return botao.value == LOW;
}

void button_configure(Botao& botao, uint8_t modo) {
    pinMode(botao.pinoGPIO, modo);
    botao.value = digitalRead(botao.pinoGPIO);
}
