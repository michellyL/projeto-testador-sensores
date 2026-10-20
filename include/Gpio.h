#pragma once
#include <Arduino.h>

void gpio_init(uint8_t pino, uint8_t modo);
void gpio_write(uint8_t pino, uint8_t valor);
void gpio_read(uint8_t pino, uint8_t& valor);
