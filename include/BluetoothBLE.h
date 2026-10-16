#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <string>

// Informacoes armazenadas para cada dispositivo Bluetooth encontrado.
struct Dispositivo {
    String enderecoMAC;           // Endereco MAC anunciado.
    String nomeDispositivo;       // Nome anunciado, se disponivel.
    int intensidadeSinalRSSI;     // Intensidade do sinal recebido.
    bool possuiTemperatura;       // Indica se a temperatura e valida.
    bool possuiUmidade;            // Indica se a umidade e valida.
    float temperaturaCelsius;     // Temperatura decodificada.
    int umidadePercentual;        // Umidade relativa decodificada.
};

extern BLEScan* scannerBluetooth;
extern Dispositivo dispositivosBLE[];
extern int quantidadeDispositivosBLE;
extern int indiceDispositivoBLESelecionado;
extern bool mostrandoMAC;

// Interpreta dados de fabricante no formato Teltonika conhecido pelo projeto.
void interpretarDadosTeltonika(Dispositivo& dispositivo, const std::string& dados);
// Exibe no LCD os dados do dispositivo selecionado.
void mostrarDispositivoBLE();
// Busca dispositivos BLE e ordena os resultados pelo sinal recebido.
void buscarDispositivosBluetooth();
// Inicializa o Bluetooth e configura o scanner.
void iniciarBluetooth();
