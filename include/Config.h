#pragma once
#include <Arduino.h>

//tirei as variáveis de aplicação extern bool mostrandoTag; e 
//extern unsigned long instanteUltimaTemperatura;

constexpr uint8_t PINO_SENSOR_DS = 4; 
constexpr uint8_t PINO_BOTAO_DS = 25;
constexpr int CAPACIDADE_MAXIMA_DS = 16;
constexpr unsigned long TEMPO_DEBOUNCE_MS = 50;
constexpr unsigned long INTERVALO_ATUALIZACAO_TEMPERATURA_MS = 8000;
constexpr unsigned long INTERVALO_REBUSCA_DS_MS = 10000;

