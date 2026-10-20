#include <LiquidCrystal_I2C.h>
#include "Config.h"
#include "Lcd.h"
#include "SensorDS.h"
#include "Botoes.h"
#include "SystemDallas.h"
#include "SystemSetup.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire barramentoOneWireDS(PINO_SENSOR_DS);
DallasTemperature sensoresTemperaturaDS(&barramentoOneWireDS);
bool mostrandoTag = false;
unsigned long instanteUltimaTemperatura = 0;
unsigned long instanteUltimaBuscaDS = 0;
Botao botaoDS = {PINO_BOTAO_DS, HIGH, HIGH, 0};

void setup() {
    SystemSetup();
}

void loop() {
    SystemDallas();
}
