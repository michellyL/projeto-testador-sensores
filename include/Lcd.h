#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void lcd_init();
void lcd_write(const char* msg, uint8_t linha, uint8_t coluna);
