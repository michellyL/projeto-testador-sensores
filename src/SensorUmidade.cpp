#include "SensorUmidade.h"
#include "Display.h"

namespace {
    constexpr uint8_t PINO_ADD_THERM = 32;

    OneWire barramentoAdd(PINO_ADD_THERM);
    uint8_t romAdd[8];
    bool encontrado = false;

    bool selecionarDS2438() {
        if (!barramentoAdd.reset()) {
            return false;
        }

        barramentoAdd.select(romAdd);
        return true;
    }

    bool lerPagina(uint8_t pagina, uint8_t dados[9]) {
        // Recupera os dados da memória para o scratchpad
        if (!selecionarDS2438()) return false;

        barramentoAdd.write(0xB8);
        barramentoAdd.write(pagina);
        delay(2);

        // Lê os 8 bytes e o byte de CRC
        if (!selecionarDS2438()) return false;

        barramentoAdd.write(0xBE);
        barramentoAdd.write(pagina);

        for (int i = 0; i < 9; i++) {
            dados[i] = barramentoAdd.read();
        }

        return OneWire::crc8(dados, 8) == dados[8];
    }

    bool configurarVAD() {
        uint8_t dados[9];

        if (!lerPagina(0, dados)) return false;

        // AD = 0 seleciona a entrada VAD.
        // O bit AD fica no bit 3 do registrador de configuração.
        if ((dados[0] & 0x08) == 0) {
            return true;
        }

        dados[0] &= ~0x08;

        if (!selecionarDS2438()) return false;

        // Escreve os 8 bytes da página 0
        barramentoAdd.write(0x4E);
        barramentoAdd.write(0);

        for (int i = 0; i < 8; i++) {
            barramentoAdd.write(dados[i]);
        }

        // Copia a configuração para a memória do DS2438
        if (!selecionarDS2438()) return false;

        barramentoAdd.write(0x48);
        barramentoAdd.write(0);
        delay(15);

        return true;
    }

    bool converterTemperatura(float &temperatura) {
        if (!selecionarDS2438()) return false;

        barramentoAdd.write(0x44);
        delay(20);

        uint8_t dados[9];

        if (!lerPagina(0, dados)) return false;

        // Temperatura: byte 1 = parte baixa, byte 2 = parte alta.
        uint16_t valor = ((uint16_t)dados[2] << 8) | dados[1];
        int16_t bruto = (int16_t)valor;

        temperatura = bruto / 256.0f;

        return temperatura >= -40.0f && temperatura <= 85.0f;
    }

    bool converterVAD(float &tensao) {
        // Garante que o conversor esteja selecionando VAD
        if (!configurarVAD()) return false;

        if (!selecionarDS2438()) return false;

        // Convert Voltage: não enviar byte adicional
        barramentoAdd.write(0xB4);
        delay(12);

        uint8_t dados[9];

        if (!lerPagina(0, dados)) return false;

        // Tensão: byte 3 = parte baixa, byte 4 = parte alta
        uint16_t bruto =
            ((uint16_t)dados[4] << 8) | dados[3];

        bruto &= 0x03FF;
        tensao = bruto * 0.01f;

        return tensao >= 0.0f && tensao <= 10.23f;
    }
}

void iniciarSensorUmidade() {
    encontrado = false;
    barramentoAdd.reset_search();

    while (barramentoAdd.search(romAdd)) {
        if (OneWire::crc8(romAdd, 7) == romAdd[7]
            && romAdd[0] == 0x26) {
            encontrado = true;
            break;
        }
    }

    barramentoAdd.reset_search();

    Serial.println(
        encontrado
            ? "ADD-THERM DS2438 encontrado"
            : "ADD-THERM DS2438 nao encontrado"
    );
}

bool lerSensorUmidade(
    float &temperatura,
    float &vdd,
    float &vad
) {
    if (!encontrado) {
        iniciarSensorUmidade();
    }

    if (!encontrado) return false;

    if (!converterTemperatura(temperatura)) return false;
    if (!converterVAD(vad)) return false;

    // A placa está alimentada em 3,3 V.
    // VDD é informado como referência aproximada,
    // pois o DS2438 está configurado para medir VAD.
    vdd = 3.30f;

    return true;
}

void mostrarTemperaturaUmidade() {
    float temperatura = 0;
    float vdd = 0;
    float vad = 0;

    if (!lerSensorUmidade(temperatura, vdd, vad)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Falha ADD-THERM");
        lcd.setCursor(0, 1);
        lcd.print("Verifique sensor");

        Serial.println("Falha na leitura do ADD-THERM");
        return;
    }

    // Fórmula de umidade adotada: HIH-5030
    float umidade = ((vad / vdd) - 0.1515f) / 0.00636f;
    umidade = umidade / (1.0546f - 0.00216f * temperatura);
    umidade = constrain(umidade, 0.0f, 100.0f);

    // Temperatura na primeira linha
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp 2: ");
    lcd.print(temperatura, 1);
    lcd.write((uint8_t)223);
    lcd.print("C");

    // Umidade na segunda linha
    lcd.setCursor(0, 1);
    lcd.print("Umidade: ");
    lcd.print(umidade, 1);
    lcd.print("%");

    Serial.println("===== ADD-THERM =====");
    Serial.printf("Temperatura: %.2f C\n", temperatura);
    Serial.printf("VAD: %.2f V\n", vad);
    Serial.printf("Umidade estimada: %.1f%%\n", umidade);
}


void mostrarTagUmidade() {
    if (!encontrado) {
        iniciarSensorUmidade();
    }

    lcd.clear();

    if (!encontrado) {
        lcd.print("ADD ausente");
        return;
    }

    // LCD 16x2: 8 caracteres hexadecimais por linha.
    lcd.setCursor(0, 0);

    for (int i = 0; i < 4; i++) {
        if (romAdd[i] < 0x10) lcd.print("0");
        lcd.print(romAdd[i], HEX);
    }

    lcd.setCursor(0, 1);

    for (int i = 4; i < 8; i++) {
        if (romAdd[i] < 0x10) lcd.print("0");
        lcd.print(romAdd[i], HEX);
    }

    Serial.printf(
        "TAG ADD-THERM: %02X %02X %02X %02X %02X %02X %02X %02X\n",
        romAdd[0], romAdd[1], romAdd[2], romAdd[3],
        romAdd[4], romAdd[5], romAdd[6], romAdd[7]
    );
}
