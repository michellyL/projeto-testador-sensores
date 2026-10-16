#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BLEDevice.h>
#include "Config.h"
#include "Display.h"
#include "SensorDS.h"
#include "BluetoothBLE.h"
#include "Botoes.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire barramentoOneWireDS(PINO_SENSOR_DS);
DallasTemperature sensoresTemperaturaDS(&barramentoOneWireDS);
BLEScan* scannerBluetooth = nullptr;
Dispositivo dispositivosBLE[CAPACIDADE_MAXIMA_BLE];
int quantidadeDispositivosBLE = 0;
int indiceDispositivoBLESelecionado = 0;
bool modoBluetoothAtivo = false;
bool mostrandoTag = false;
bool mostrandoMAC = false;
unsigned long instanteUltimaTemperatura = 0;
unsigned long instanteUltimaBuscaDS = 0;
Botao botaoDS = {PINO_BOTAO_DS, HIGH, HIGH, 0, 0, false};
Botao botaoBLE = {PINO_BOTAO_BLE, HIGH, HIGH, 0, 0, false};

// Configura serial, GPIOs, LCD e sensores antes de iniciar 
void setup() {
    Serial.begin(115200);
    pinMode(PINO_BOTAO_DS, INPUT_PULLUP);
    pinMode(PINO_BOTAO_BLE, INPUT_PULLUP);

    Wire.begin(21, 22);
    lcd.init();
    lcd.backlight();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Buscando");
    lcd.setCursor(0, 1);
    lcd.print("sensores......");

    iniciarSensoresDS();
    iniciarBluetooth();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensores: ");
    lcd.setCursor(0, 1);
    lcd.print(quantidadeSensoresDS());
    delay(4000);

    mostrandoTag = false;
    mostrarTemperatura();
    instanteUltimaTemperatura = millis();
    Serial.println("Testador iniciado!");
}

// Processa botoes e atualiza periodicamente sensores
void loop() {
    processarBotao(botaoDS, TipoBotao::SensorDS18B20);
    processarBotao(botaoBLE, TipoBotao::BluetoothBLE);

    // Atualiza periodicamente a lista de sensores para detectar conexoes e remocoes.
    if (!modoBluetoothAtivo && millis() - instanteUltimaBuscaDS >= INTERVALO_REBUSCA_DS_MS) {
        procurarSensoresDS();
        if (mostrandoTag) mostrarTag();
        else mostrarTemperatura();
        instanteUltimaTemperatura = millis();
    }

    if (!modoBluetoothAtivo && !mostrandoTag &&
        millis() - instanteUltimaTemperatura >= INTERVALO_ATUALIZACAO_TEMPERATURA_MS) {
        instanteUltimaTemperatura = millis();
        mostrarTemperatura();
    }

    delay(10);
}
