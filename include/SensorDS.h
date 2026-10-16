#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Config.h"

// Objetos de hardware gerenciados pelo modulo DS18B20.
extern OneWire barramentoOneWireDS;
extern DallasTemperature sensoresTemperaturaDS;

// Inicializa a biblioteca e realiza a primeira busca de sensores.
void iniciarSensoresDS();
// Atualiza a lista de enderecos dos sensores conectados.
void procurarSensoresDS();
// Seleciona o proximo sensor, voltando ao primeiro ao final da lista.
void proximoSensorDS();
// Le e mostra temperatura e TAG do sensor selecionado.
void mostrarTemperatura();
// Funcao legada mantida temporariamente, conforme solicitado.
void mostrarTag();
// Informa quantos sensores DS18B20 foram encontrados.
int quantidadeSensoresDS();
