#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Config.h"

//A classe OneWire implementa a comunicação com dispositivos 1-Wire, permitindo operações como reset, 
//leitura e escrita de bytes e bits, além de suporte para busca de dispositivos e verificação de CRC
extern OneWire barramentoOneWireDS;
extern DallasTemperature sensoresTemperaturaDS;


void iniciarSensoresDS();

void procurarSensoresDS();

void proximoSensorDS();

void mostrarTemperatura();

void mostrarTag();

int quantidadeSensoresDS();
