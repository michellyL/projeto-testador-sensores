#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Config.h"

//A classe OneWire implementa a comunicação com dispositivos 1-Wire, permitindo operações como reset, 
//leitura e escrita de bytes e bits, além de suporte para busca de dispositivos e verificação de CRC
extern OneWire barramentoOneWireDS;
extern DallasTemperature sensoresTemperaturaDS;

// Inicializa a biblioteca e realiza a primeira busca de sensores
void iniciarSensoresDS();
// Atualiza a lista de enderecos dos sensores
void procurarSensoresDS();
// Seleciona proximo sensor
void proximoSensorDS();
// Le e mostra temperatura e TAG do sensor 
void mostrarTemperatura();

void mostrarTag();
// Informa quantos sensores DS18B20 foram encontrados.
int quantidadeSensoresDS();
