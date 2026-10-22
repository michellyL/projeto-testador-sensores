#include <LiquidCrystal_I2C.h>
#include "Config.h"
#include "Botoes.h"
#include "SensorDS.h"
#include "SystemDallas.h"
#include "SystemSetup.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire barramentoOneWireDS(PINO_SENSOR_DS);
DallasTemperature sensoresTemperaturaDS(&barramentoOneWireDS);
unsigned long instanteUltimaTemperatura = 0;
Botao botaoDS = {PINO_BOTAO_DS, HIGH};

void setup() {
    SystemSetup();
}

void loop() {
    if (SystemDallas()) {
        SystemDallas_mostrarInformacoes();
    }
}
