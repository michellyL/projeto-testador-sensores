#include "Gpio.h"

void gpio_init(uint8_t pino, uint8_t modo) {
    pinMode(pino, modo);
}

void gpio_write(uint8_t pino, uint8_t valor) {
    digitalWrite(pino, valor);
}

void gpio_read(uint8_t pino, uint8_t& valor) {
    valor = digitalRead(pino);
}