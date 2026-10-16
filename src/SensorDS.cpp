#include "SensorDS.h"
#include "Display.h"\n#include <cstring>

namespace {
    DeviceAddress enderecosSensoresDS[CAPACIDADE_MAXIMA_DS];
    int quantidadeSensoresDSInterna = 0;
    int indiceSensorDSSelecionado = 0;

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
        // Exibe os 16 caracteres hexadecimais na segunda linha do LCD.
        lcd.setCursor(0, 1);
        for (uint8_t i = 0; i < 8; ++i) {
            if (endereco[i] < 0x10) lcd.print('0');
            lcd.print(endereco[i], HEX);
        }
    }
}

// Inicializa a biblioteca DallasTemperature e procura sensores no barramento.
void iniciarSensoresDS() {
    sensoresTemperaturaDS.begin();
    sensoresTemperaturaDS.setWaitForConversion(true);
    procurarSensoresDS();
}

// Atualiza enderecos encontrados, evita duplicacoes
void procurarSensoresDS() {
    DeviceAddress enderecoAtual;
    DeviceAddress enderecoSelecionado;
    bool tinhaSelecionado = quantidadeSensoresDSInterna > 0 && indiceSensorDSSelecionado < quantidadeSensoresDSInterna;

    if (tinhaSelecionado) {
        memcpy(enderecoSelecionado, enderecosSensoresDS[indiceSensorDSSelecionado], sizeof(DeviceAddress));
    }

    sensoresTemperaturaDS.begin();
    int encontrados = sensoresTemperaturaDS.getDeviceCount();
    quantidadeSensoresDSInterna = 0;

    for (int i = 0; i < encontrados && quantidadeSensoresDSInterna < CAPACIDADE_MAXIMA_DS; ++i) {
        if (sensoresTemperaturaDS.getAddress(enderecoAtual, static_cast<uint8_t>(i))) {
            // Evita guardar endereços duplicados.
            bool duplicado = false;
            for (int j = 0; j < quantidadeSensoresDSInterna; ++j) {
                if (mesmoEndereco(enderecoAtual, enderecosSensoresDS[j])) {
                    duplicado = true;
                    break;
                }
            }
            if (!duplicado) {
                memcpy(enderecosSensoresDS[quantidadeSensoresDSInterna], enderecoAtual, sizeof(DeviceAddress));
                ++quantidadeSensoresDSInterna;
            }
        }
    }

    indiceSensorDSSelecionado = 0;
    if (tinhaSelecionado) {
        for (int i = 0; i < quantidadeSensoresDSInterna; ++i) {
            if (mesmoEndereco(enderecoSelecionado, enderecosSensoresDS[i])) {
                indiceSensorDSSelecionado = i;
                break;
            }
        }
    }
    if (quantidadeSensoresDSInterna == 0) indiceSensorDSSelecionado = 0;

    Serial.print("DS18B20 encontrados: ");
    Serial.println(quantidadeSensoresDSInterna);
    for (int i = 0; i < quantidadeSensoresDSInterna; ++i) {
        Serial.print("DS "); Serial.print(i + 1); Serial.print(" TAG: ");
        imprimirTagSerial(enderecosSensoresDS[i]);
        Serial.println();
    }
    instanteUltimaBuscaDS = millis();
}

// Retorna a quantidade de sensores DS18B20 encontrados.
int quantidadeSensoresDS() {
    return quantidadeSensoresDSInterna;
}

// Avanca para o proximo sensor
void proximoSensorDS() {
    if (quantidadeSensoresDSInterna == 0) {
        mostrarTemperatura();
        return;
    }
    indiceSensorDSSelecionado = (indiceSensorDSSelecionado + 1) % quantidadeSensoresDSInterna;
    mostrandoTag = false;
    mostrarTemperatura();
    instanteUltimaTemperatura = millis();
}

// Le o sensor selecionado e exibe temperatura e TAG no LCD.
void mostrarTemperatura() {
    lcd.clear();

    if (quantidadeSensoresDSInterna == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Nenhum sensoresTemperaturaDS DS");
        lcd.setCursor(0, 1);
        lcd.print("Verifique cabos");
        Serial.println("Nenhum DS18B20 encontrado no barramento.");
        return;
    }

    sensoresTemperaturaDS.requestTemperatures();
    float temp = sensoresTemperaturaDS.getTempC(enderecosSensoresDS[indiceSensorDSSelecionado]);

    // Linha 1: numero do sensoresTemperaturaDS e temperatura; linha 2: TAG completa (16 digitos).
    lcd.setCursor(0, 0);
    lcd.print(indiceSensorDSSelecionado + 1);
    lcd.print(" TEMP: ");

    if (temp == DEVICE_DISCONNECTED_C || temp < -55.0f || temp > 125.0f) {
        lcd.print("ERRO");
        Serial.print("Falha na leitura do DS ");
        Serial.println(indiceSensorDSSelecionado + 1);
    } else {
        lcd.print(temp, 1);
        lcd.write(static_cast<uint8_t>(223));
        lcd.print("C");
        Serial.print("DS "); Serial.print(indiceSensorDSSelecionado + 1);
        Serial.print("/"); Serial.print(quantidadeSensoresDSInterna);
        Serial.print(" | TAG: "); imprimirTagSerial(enderecosSensoresDS[indiceSensorDSSelecionado]);
        Serial.print(" | Temperatura: "); Serial.print(temp, 2); Serial.println(" C");
    }

    imprimirTagLCD(enderecosSensoresDS[indiceSensorDSSelecionado]);
}

void mostrarTag() {
    // A TAG agora permanece visivel junto da temperatura na tela principal.
    mostrarTemperatura();
}
