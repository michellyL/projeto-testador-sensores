#pragma once
#include <Arduino.h>
#include "Config.h"

// Tipo de acao a cada botao
enum class TipoBotao { SensorDS18B20, BluetoothBLE };

// Estado do botao para debounce e pressionamento longo.
struct Botao {
    uint8_t pinoGPIO;                     
    bool leituraAnterior;                 
    bool estadoEstavel;                   //Leitura validada pelo debounce
    unsigned long instanteUltimaMudanca;  
    unsigned long instantePressionamento; 
    bool cliqueLongoDisparado;             //Impede repetir acao de clique longo
};

// Funcoes de clique e clique longo para cada botao.
void cliqueDS();
void longoDS();
void cliqueBLE();
void longoBLE();

// Processa leitura, debounce e evento de clique do botao informado.
void processarBotao(Botao& botao, TipoBotao tipo);
