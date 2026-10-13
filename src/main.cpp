#include "Config.h"
#include "Display.h"
#include "SensorDS.h"
#include "BluetoothBLE.h"
#include "Botoes.h"
#include "SensorUmidade.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(PINO_SENSOR);
DallasTemperature sensor(&oneWire);
DeviceAddress enderecoSensor;
BLEScan *scannerBLE = nullptr;
Dispositivo lista[MAX_BLE];
int totalBLE = 0;
int selecionado = 0;
bool modoBLE = false;
bool mostrandoTag = false;
uint8_t telaDS = 0;
bool mostrandoMAC = false;
unsigned long ultimaTemperatura = 0;
Botao botaoDS = {BOTAO_DS, HIGH, HIGH, 0, 0, false};
Botao botaoBLE = {BOTAO_BLE, HIGH, HIGH, 0, 0, false};

void setup()
{
    Serial.begin(115200);

    pinMode(BOTAO_DS, INPUT_PULLUP);
    pinMode(BOTAO_BLE, INPUT_PULLUP);

    Wire.begin(21, 22);
    lcd.init();
    lcd.backlight();

    sensor.begin();
    iniciarSensorUmidade();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TESTADOR SENSOR");
    lcd.setCursor(0, 1);
    lcd.print("Iniciando...");

    iniciarBluetooth();

    delay(1500);

    lcd.clear();
    mostrarTemperatura();
    ultimaTemperatura = millis();

    Serial.println("Testador iniciado!");
}

void loop()
{
    processarBotao(botaoDS, 0);
    processarBotao(botaoBLE, 1);

    if (!modoBLE &&
        !mostrandoTag &&
        millis() - ultimaTemperatura >= 2000)
    {

        ultimaTemperatura = millis();
        if (telaDS == 2) mostrarTemperaturaUmidade();
        else mostrarTemperatura();
    }

    delay(10);
}
