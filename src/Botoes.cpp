#include "Botoes.h"

bool button_read(const Botao& botao) {
    return digitalRead(botao.pinoGPIO) == HIGH;
}

void button_configure(Botao& botao, uint8_t modo) {
    pinMode(botao.pinoGPIO, modo);
    botao.leituraAnterior = button_read(botao);
    botao.estadoEstavel = botao.leituraAnterior;
    botao.instanteUltimaMudanca = millis();
}
