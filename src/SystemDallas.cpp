#include "SystemDallas.h"
#include "Botoes.h"
#include "Config.h"
#include "Lcd.h"
#include "Serial.h"
#include "SensorDS.h"

extern unsigned long instanteUltimaTemperatura;
unsigned long instanteUltimaBuscaDS = 0;

void SystemDallas_mostrarQuantidadeSensores() {
    lcd_print ("Sensores", 0, 0);
    lcd_print ("Encontrados", 0, 1);
    lcd_print(String(quantidadeSensoresDS()).c_str(), 13, 1);
}

void SystemDallas_mostrarInformacoes() {
    const int quantidade = quantidadeSensoresDS();
    if (quantidade == 0) {
        serial_mostrarSemSensores();
        return;
    }

    const int indice = indiceSensorDSSelecionado();
    const float temperatura = lerTemperaturaSensorDS();
    DeviceAddress endereco;
    if (!obterEnderecoSensorDSSelecionado(endereco)) {
        return;
    }

    lcd.clear();
    lcd_print(String(indice + 1).c_str(), 0, 0);
    lcd_print(" TEMP: ", 1, 0);

    if (temperatura < -55.0f || temperatura > 125.0f) {
        lcd.print("ERRO");
    } else {
        lcd.print(temperatura, 1);
        lcd.write(static_cast<uint8_t>(223));
        lcd.print("C");
    }

    lcd.setCursor(0, 1);
    for (uint8_t i = 0; i < 8; ++i) {
        if (endereco[i] < 0x10) {
            lcd.print('0');
        }
        lcd.print(endereco[i], HEX);
    }
    serial_mostrarSensorDS(indice, quantidade, temperatura, endereco);
}

bool SystemDallas() {
    static bool botaoInicializado = false;
    static bool leituraAnterior = false;
    static bool estadoEstavel = false;
    static unsigned long instanteUltimaMudanca = 0;
    static bool primeiraAtualizacao = true;
    static bool conversaoPendente = false;
    bool solicitarAtualizacao = false;

    const unsigned long last_update_ms = millis();
    const bool botaoPressionado = button_is_pressed(botaoDS);

    if (!botaoInicializado) {
        leituraAnterior = botaoPressionado;
        estadoEstavel = botaoPressionado;
        instanteUltimaMudanca = last_update_ms;
        botaoInicializado = true;
    }

    if (primeiraAtualizacao) {
        instanteUltimaTemperatura = last_update_ms;
        instanteUltimaBuscaDS = last_update_ms;
        primeiraAtualizacao = false;
        solicitarAtualizacao = true;
    }

    if (botaoPressionado != leituraAnterior) {
        leituraAnterior = botaoPressionado;
        instanteUltimaMudanca = last_update_ms;
    }

    if (last_update_ms - instanteUltimaMudanca >= TEMPO_DEBOUNCE_MS &&
        botaoPressionado != estadoEstavel) {
        estadoEstavel = botaoPressionado;
        if (estadoEstavel) {
            proximoSensorDS();
            solicitarAtualizacao = true;
        }
    }

    if (last_update_ms - instanteUltimaBuscaDS >= INTERVALO_REBUSCA_DS_MS) {
        procurarSensoresDS();
        instanteUltimaBuscaDS = millis();
        solicitarAtualizacao = true;
    } else if (last_update_ms - instanteUltimaTemperatura >= INTERVALO_ATUALIZACAO_TEMPERATURA_MS) {
        solicitarAtualizacao = true;
    }

    if (solicitarAtualizacao && !conversaoPendente) {
        instanteUltimaTemperatura = millis();
        if (quantidadeSensoresDS() == 0) {
            delay(10);
            return true;
        }
        sensoresTemperaturaDS.requestTemperatures();
        conversaoPendente = true;
    }

    if (conversaoPendente && sensoresTemperaturaDS.isConversionComplete()) {
        conversaoPendente = false;
        delay(10);
        return true;
    }

    delay(10);
    return false;
}