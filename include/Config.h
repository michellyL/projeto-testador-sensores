#pragma once
#include <Arduino.h>

// Configuracoes e tempo
constexpr uint8_t PINO_SENSOR_DS = 4;
constexpr uint8_t PINO_BOTAO_DS = 25;
constexpr uint8_t PINO_BOTAO_BLE = 26;
constexpr int CAPACIDADE_MAXIMA_BLE = 50;
constexpr int CAPACIDADE_MAXIMA_DS = 16;
constexpr unsigned long TEMPO_DEBOUNCE_MS = 50;
constexpr unsigned long TEMPO_CLIQUE_LONGO_MS = 2000;
constexpr unsigned long INTERVALO_ATUALIZACAO_TEMPERATURA_MS = 8000;
constexpr unsigned long INTERVALO_REBUSCA_DS_MS = 10000;


extern bool modoBluetoothAtivo;
extern bool mostrandoTag;
extern bool mostrandoMAC;
extern unsigned long instanteUltimaTemperatura;
extern unsigned long instanteUltimaBuscaDS;
