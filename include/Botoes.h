#pragma once
#include <Arduino.h>
#include "Config.h"

// Tipo de acao associado a cada botao, evitando numeros magicos.
enum class TipoBotao { SensorDS18B20, BluetoothBLE };

// Estado do botao para debounce e deteccao de pressionamento longo.
struct Botao {
    uint8_t pinoGPIO;                     // GPIO conectado ao botao.
    bool leituraAnterior;                 // Ultima leitura bruta.
    bool estadoEstavel;                   // Leitura validada pelo debounce.
    unsigned long instanteUltimaMudanca;  // Momento da ultima mudanca.
    unsigned long instantePressionamento; // Inicio do pressionamento.
    bool cliqueLongoDisparado;             // Impede repetir a acao de clique longo.
};

// Acoes atuais preservadas para nao remover funcionalidades nesta etapa.
void cliqueDS();
void longoDS();
void cliqueBLE();
void longoBLE();

// Processa leitura, debounce e evento de clique do botao informado.
void processarBotao(Botao& botao, TipoBotao tipo);
