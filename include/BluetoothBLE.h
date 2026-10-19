#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <string>

// Informacoes dispositivo Bluetooth 
struct Dispositivo {
    String enderecoMAC;           
    String nomeDispositivo;       
    int intensidadeSinalRSSI;     
    bool possuiTemperatura;       
    bool possuiUmidade;           
    float temperaturaCelsius;     
    int umidadePercentual;       
};

extern BLEScan* scannerBluetooth;
extern Dispositivo dispositivosBLE[];
extern int quantidadeDispositivosBLE;
extern int indiceDispositivoBLESelecionado;
extern bool mostrandoMAC;

// Interpreta dados do fabricante no formato da Teltonika 
void interpretarDadosTeltonika(Dispositivo& dispositivo, const std::string& dados);
// Exibe no LCD os dados do dispositivo selecionado.
void mostrarDispositivoBLE();
// Busca dispositivos BLE e ordena os resultados pelo sinal recebido.
void buscarDispositivosBluetooth();
// Inicializa o Bluetooth e configura o scanner.
void iniciarBluetooth();
