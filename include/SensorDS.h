#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Config.h"

extern OneWire barramentoOneWireDS;
extern DallasTemperature sensoresTemperaturaDS;

void iniciarSensoresDS();
void procurarSensoresDS();
void proximoSensorDS();
int quantidadeSensoresDS();
int indiceSensorDSSelecionado();
float lerTemperaturaSensorDS();
bool obterEnderecoSensorDSSelecionado(DeviceAddress endereco);
