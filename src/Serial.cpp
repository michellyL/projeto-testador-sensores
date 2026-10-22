#include "Serial.h"

static void imprimirEnderecoSerial(const uint8_t endereco[8]) {
    for (uint8_t i = 0; i < 8; ++i) {
        if (endereco[i] < 0x10) {
            Serial.print('0');
        }
        Serial.print(endereco[i], HEX);
    }
}

void serial_init(unsigned long velocidade) {
    Serial.begin(velocidade);
}

void serial_mostrarInicializacao(int quantidadeSensores) {
    Serial.print("DS18B20 encontrados: ");
    Serial.println(quantidadeSensores);
    Serial.println("Testador iniciado!");
}

void serial_mostrarSemSensores() {
    Serial.println("Nenhum DS18B20 encontrado no barramento.");
}

void serial_mostrarSensorDS(
    int indice,
    int quantidade,
    float temperatura,
    const uint8_t endereco[8]) {
    Serial.print("DS ");
    Serial.print(indice + 1);
    Serial.print("/");
    Serial.print(quantidade);

    if (temperatura < -55.0f || temperatura > 125.0f) {
        Serial.println(" | Falha na leitura.");
        return;
    }

    Serial.print(" | Temperatura: ");
    Serial.print(temperatura, 2);
    Serial.print(" C | TAG: ");
    imprimirEnderecoSerial(endereco);
    Serial.println();
}