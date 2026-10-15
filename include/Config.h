#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <string>

// Barramento OneWire compartilhado por todos os DS18B20.
// GPIO4 do ESP32 (também identificado como D4 em algumas placas).
#define PINO_SENSOR 4
#define BOTAO_DS 25
#define BOTAO_BLE 26

constexpr int MAX_BLE = 50;
constexpr int MAX_DS = 16;
constexpr unsigned long DEBOUNCE = 50;
constexpr unsigned long TEMPO_LONGO = 2000;
constexpr unsigned long INTERVALO_TEMPERATURA = 2000;
constexpr unsigned long INTERVALO_BUSCA_DS = 10000;

struct Dispositivo {
    String mac;
    String nome;
    int rssi;
    bool temTemp;
    bool temUmid;
    float temperatura;
    int umidade;
};

struct Botao {
    int pino;
    bool anterior;
    bool estavel;
    unsigned long mudanca;
    unsigned long inicio;
    bool longo;
};

extern LiquidCrystal_I2C lcd;
extern OneWire oneWire;
extern DallasTemperature sensor;
extern BLEScan* scannerBLE;
extern Dispositivo lista[MAX_BLE];
extern int totalBLE;
extern int selecionado;
extern bool modoBLE;
extern bool mostrandoTag;
extern bool mostrandoMAC;
extern unsigned long ultimaTemperatura;
extern unsigned long ultimaBuscaDS;
extern Botao botaoDS;
extern Botao botaoBLE;
