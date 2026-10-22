#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <stdint.h>

extern LiquidCrystal_I2C lcd;

void lcd_init();
void lcd_print(const char* str, uint8_t col, uint8_t row);
