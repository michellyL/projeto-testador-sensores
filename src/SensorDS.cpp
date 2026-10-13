#include "SensorDS.h"
#include "Display.h"

void mostrarTemperatura() {
    sensor.requestTemperatures();
    float temp = sensor.getTempCByIndex(0);

    limparLinha(0);
    lcd.print("TEMP 1:");

    limparLinha(1);

    if (temp == DEVICE_DISCONNECTED_C) {
        lcd.print("Sensor ausente");
        Serial.println("DS18B20 ausente");
    } else {
        lcd.print(temp, 1);
        lcd.write((uint8_t)223);
        lcd.print("C");

        Serial.print("DS18B20: ");
        Serial.println(temp);
    }
}

void mostrarTag() {
    lcd.clear();
    sensor.begin();

    if (!sensor.getAddress(enderecoSensor, 0)) {
        lcd.print("Sensor ausente");
        return;
    }

    Serial.print("TAG: ");

    for (int i = 0; i < 8; i++) {
        if (i == 4) lcd.setCursor(0, 1);

        if (enderecoSensor[i] < 16) {
            lcd.print("0");
            Serial.print("0");
        }

        lcd.print(enderecoSensor[i], HEX);
        Serial.print(enderecoSensor[i], HEX);
    }

    Serial.println();
}
