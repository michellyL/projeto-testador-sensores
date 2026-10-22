#pragma once
#include <Arduino.h>
#include <stdint.h>

void serial_init(unsigned long velocidade);
void serial_mostrarInicializacao(int quantidadeSensores);
void serial_mostrarSemSensores();
void serial_mostrarSensorDS(int indice, int quantidade, float temperatura, const uint8_t endereco[8]);
