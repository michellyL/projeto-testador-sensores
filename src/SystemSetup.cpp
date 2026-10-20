#include "SystemSetup.h"
#include <Wire.h>
#include <stdio.h>
#include "Config.h"
#include "Botoes.h"
#include "Lcd.h"
#include "SensorDS.h"

extern Botao botaoDS;

void SystemSetup() {
    Serial.begin(115200);

    button_configure(botaoDS, INPUT_PULLUP);

    Wire.begin(21, 22);
    lcd_init();
    lcd_write("Buscando Sensor", 0, 0);
    lcd_write("Aguarde...", 1, 0);
    delay(2000);

    iniciarSensoresDS();

    char mensagem[17];
    snprintf(mensagem, sizeof(mensagem), "Sensores DS: %d", quantidadeSensoresDS());
    lcd.clear();
    lcd_write(mensagem, 0, 0);
    delay(4000);

    mostrandoTag = false;
    mostrarTemperatura();
    instanteUltimaTemperatura = millis();
    Serial.println("Testador iniciado!");
}