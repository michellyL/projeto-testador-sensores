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

#define PINO_SENSOR 4
#define PINO_SENSOR_ADD 32
#define BOTAO_DS 25
#define BOTAO_BLE 26
constexpr int MAX_BLE = 50;
constexpr unsigned long DEBOUNCE = 50;
constexpr unsigned long TEMPO_LONGO = 2000;

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
extern DeviceAddress enderecoSensor;
extern BLEScan* scannerBLE;
extern Dispositivo lista[MAX_BLE];
extern int totalBLE;
extern int selecionado;
extern bool modoBLE;
extern bool mostrandoTag;
extern uint8_t telaDS;
extern bool mostrandoMAC;
extern unsigned long ultimaTemperatura;
extern Botao botaoDS;
extern Botao botaoBLE;
