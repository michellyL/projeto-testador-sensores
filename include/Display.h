#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// LCD utilizado pelo modulo de apresentacao.
extern LiquidCrystal_I2C lcd;

// Apaga o conteudo de uma linha do LCD e reposiciona o cursor.
void limparLinha(uint8_t numeroLinha);
