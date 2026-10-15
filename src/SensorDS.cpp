#include "SensorDS.h"
#include "Display.h"

namespace {
    DeviceAddress enderecosDS[MAX_DS];
    int totalDS = 0;
    int selecionadoDS = 0;

    bool mesmoEndereco(const DeviceAddress a, const DeviceAddress b) {
        for (uint8_t i = 0; i < 8; ++i) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }

    void imprimirTagSerial(const DeviceAddress endereco) {
        for (uint8_t i = 0; i < 8; ++i) {
            if (endereco[i] < 0x10) Serial.print('0');
            Serial.print(endereco[i], HEX);
        }
    }

    void imprimirTagLCD(const DeviceAddress endereco) {
        // A TAG possui 16 caracteres hexadecimais; divide entre as duas linhas.
        for (uint8_t i = 0; i < 8; ++i) {
            if (i == 4) lcd.setCursor(0, 1);
            if (endereco[i] < 0x10) lcd.print('0');
            lcd.print(endereco[i], HEX);
        }
    }
}

void iniciarSensoresDS() {
    sensor.begin();
    sensor.setWaitForConversion(true);
    procurarSensoresDS();
}

void procurarSensoresDS() {
    DeviceAddress enderecoAtual;
    DeviceAddress enderecoSelecionado;
    bool tinhaSelecionado = totalDS > 0 && selecionadoDS < totalDS;

    if (tinhaSelecionado) {
        memcpy(enderecoSelecionado, enderecosDS[selecionadoDS], sizeof(DeviceAddress));
    }

    sensor.begin();
    int encontrados = sensor.getDeviceCount();
    totalDS = 0;

    for (int i = 0; i < encontrados && totalDS < MAX_DS; ++i) {
        if (sensor.getAddress(enderecoAtual, static_cast<uint8_t>(i))) {
            // Evita guardar endereços duplicados.
            bool duplicado = false;
            for (int j = 0; j < totalDS; ++j) {
                if (mesmoEndereco(enderecoAtual, enderecosDS[j])) {
                    duplicado = true;
                    break;
                }
            }
            if (!duplicado) {
                memcpy(enderecosDS[totalDS], enderecoAtual, sizeof(DeviceAddress));
                ++totalDS;
            }
        }
    }

    selecionadoDS = 0;
    if (tinhaSelecionado) {
        for (int i = 0; i < totalDS; ++i) {
            if (mesmoEndereco(enderecoSelecionado, enderecosDS[i])) {
                selecionadoDS = i;
                break;
            }
        }
    }
    if (totalDS == 0) selecionadoDS = 0;

    Serial.print("DS18B20 encontrados: ");
    Serial.println(totalDS);
    for (int i = 0; i < totalDS; ++i) {
        Serial.print("DS "); Serial.print(i + 1); Serial.print(" TAG: ");
        imprimirTagSerial(enderecosDS[i]);
        Serial.println();
    }
    ultimaBuscaDS = millis();
}

int quantidadeSensoresDS() {
    return totalDS;
}

void proximoSensorDS() {
    if (totalDS == 0) {
        mostrarTemperatura();
        return;
    }
    selecionadoDS = (selecionadoDS + 1) % totalDS;
    mostrandoTag = false;
    mostrarTemperatura();
    ultimaTemperatura = millis();
}

void mostrarTemperatura() {
    if (totalDS == 0) {
        lcd.clear();
        lcd.print("NENHUM DS18B20");
        lcd.setCursor(0, 1);
        lcd.print("Verifique cabos");
        Serial.println("Nenhum DS18B20 encontrado no barramento.");
        return;
    }

    sensor.requestTemperatures();
    float temp = sensor.getTempC(enderecosDS[selecionadoDS]);

    lcd.clear();
    lcd.print("DS "); lcd.print(selecionadoDS + 1); lcd.print("/"); lcd.print(totalDS);
    lcd.print(" TEMP:");
    lcd.setCursor(0, 1);

    if (temp == DEVICE_DISCONNECTED_C || temp < -55.0f || temp > 125.0f) {
        lcd.print("Leitura invalida");
        Serial.print("Falha na leitura do DS ");
        Serial.println(selecionadoDS + 1);
    } else {
        lcd.print(temp, 1);
        lcd.write(static_cast<uint8_t>(223));
        lcd.print("C");
        Serial.print("DS "); Serial.print(selecionadoDS + 1);
        Serial.print("/"); Serial.print(totalDS);
        Serial.print(" | TAG: "); imprimirTagSerial(enderecosDS[selecionadoDS]);
        Serial.print(" | Temperatura: "); Serial.print(temp, 2); Serial.println(" C");
    }
}

void mostrarTag() {
    if (totalDS == 0) {
        mostrarTemperatura();
        return;
    }

    lcd.clear();
    lcd.print("TAG DS "); lcd.print(selecionadoDS + 1); lcd.print("/"); lcd.print(totalDS);
    lcd.setCursor(0, 1);
    // Mostra os primeiros 8 dígitos na primeira linha e os últimos 8 na segunda.
    lcd.clear();
    imprimirTagLCD(enderecosDS[selecionadoDS]);

    Serial.print("TAG DS "); Serial.print(selecionadoDS + 1); Serial.print(": ");
    imprimirTagSerial(enderecosDS[selecionadoDS]);
    Serial.println();
}
