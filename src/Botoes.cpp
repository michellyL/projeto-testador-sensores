#include "Botoes.h"
#include "SensorDS.h"
#include "BluetoothBLE.h"

void cliqueDS() {
    modoBLE = false;
    // Clique curto percorre os sensores encontrados.
    proximoSensorDS();
}

void longoDS() {
    // A tela principal sempre mostra temperatura e TAG juntas.
    modoBLE = false;
    mostrandoTag = false;
    mostrarTemperatura();
    ultimaTemperatura = millis();
}

void cliqueBLE() {
    if (!modoBLE) {
        modoBLE = true;
        buscarBluetooth();
        return;
    }

    if (totalBLE == 0) {
        buscarBluetooth();
        return;
    }

    selecionado++;
    if (selecionado >= totalBLE) selecionado = 0;
    mostrarBLE();
}

void longoBLE() {
    if (!modoBLE) {
        modoBLE = true;
        buscarBluetooth();
        return;
    }

    mostrandoMAC = !mostrandoMAC;
    mostrarBLE();
}

void processarBotao(Botao& botao, int tipo) {
    bool leitura = digitalRead(botao.pino);
    unsigned long agora = millis();

    if (leitura != botao.anterior) botao.mudanca = agora;

    if (agora - botao.mudanca > DEBOUNCE) {
        if (leitura != botao.estavel) {
            botao.estavel = leitura;
            if (leitura == LOW) {
                botao.inicio = agora;
                botao.longo = false;
            } else if (!botao.longo) {
                if (tipo == 0) cliqueDS();
                else cliqueBLE();
            }
        }
    }

    if (botao.estavel == LOW && !botao.longo &&
        agora - botao.inicio >= TEMPO_LONGO) {
        botao.longo = true;
        if (tipo == 0) longoDS();
        else longoBLE();
    }

    botao.anterior = leitura;
}
