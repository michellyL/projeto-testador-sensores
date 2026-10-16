#include "Config.h"
#include "Display.h"
#include "SensorDS.h"
#include "BluetoothBLE.h"
#include "Botoes.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(PINO_SENSOR);
DallasTemperature sensor(&oneWire);
BLEScan* scannerBLE = nullptr;
Dispositivo lista[MAX_BLE];
int totalBLE = 0;
int selecionado = 0;
bool modoBLE = false;
bool mostrandoTag = false;
bool mostrandoMAC = false;
unsigned long ultimaTemperatura = 0;
unsigned long ultimaBuscaDS = 0;
Botao botaoDS = {BOTAO_DS, HIGH, HIGH, 0, 0, false};
Botao botaoBLE = {BOTAO_BLE, HIGH, HIGH, 0, 0, false};

void setup() {
    Serial.begin(115200);
    pinMode(BOTAO_DS, INPUT_PULLUP);
    pinMode(BOTAO_BLE, INPUT_PULLUP);

    Wire.begin(21, 22);
    lcd.init();
    lcd.backlight();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Buscando");
    lcd.setCursor(0, 1);
    lcd.print("sensores...");

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
    ultimaTemperatura = millis();
    Serial.println("Testador iniciado!");
}

void loop() {
    processarBotao(botaoDS, 0);
    processarBotao(botaoBLE, 1);

    // Atualiza a lista periodicamente para perceber sensores conectados/removidos.
    if (!modoBLE && millis() - ultimaBuscaDS >= INTERVALO_BUSCA_DS) {
        procurarSensoresDS();
        if (mostrandoTag) mostrarTag();
        else mostrarTemperatura();
        ultimaTemperatura = millis();
    }

    if (!modoBLE && !mostrandoTag &&
        millis() - ultimaTemperatura >= INTERVALO_TEMPERATURA) {
        ultimaTemperatura = millis();
        mostrarTemperatura();
    }

    delay(10);
}
