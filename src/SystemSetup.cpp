#include "SystemSetup.h"
#include <Wire.h>
#include "Config.h"
#include "Botoes.h"
#include "Lcd.h"
#include "Serial.h"
#include "SensorDS.h"
#include "SystemDallas.h"

void SystemSetup() {
    serial_init(115200);

    button_configure(botaoDS, INPUT_PULLUP);

    Wire.begin(21, 22);
    lcd_init();

    iniciarSensoresDS();
    SystemDallas_mostrarQuantidadeSensores();
    delay(4000);

    serial_mostrarInicializacao(quantidadeSensoresDS());
}