#include "Botoes.h"
#include "SensorDS.h"
#include "BluetoothBLE.h"

// Clique curto, avança entre os sensores
void cliqueDS() {
    modoBluetoothAtivo = false;
    proximoSensorDS();
}

// Clique longo, exibe a tela de temperatura/TAG.
void longoDS() {
    modoBluetoothAtivo = false;
    mostrandoTag = false;
    mostrarTemperatura();
    instanteUltimaTemperatura = millis();
}

// Clique curto avança entre os BLE
void cliqueBLE() {
    if (!modoBluetoothAtivo) {
        modoBluetoothAtivo = true;
        buscarDispositivosBluetooth();
        return;
    }

    if (quantidadeDispositivosBLE == 0) {
        buscarDispositivosBluetooth();
        return;
    }

    indiceDispositivoBLESelecionado++;
    if (indiceDispositivoBLESelecionado >= quantidadeDispositivosBLE) indiceDispositivoBLESelecionado = 0;
    mostrarDispositivoBLE();
}

// Clique longo, exibe o MAC
void longoBLE() {
    if (!modoBluetoothAtivo) {
        modoBluetoothAtivo = true;
        buscarDispositivosBluetooth();
        return;
    }

    mostrandoMAC = !mostrandoMAC;
    mostrarDispositivoBLE();
}

void processarBotao(Botao& botao, TipoBotao tipo) {
    bool leitura = digitalRead(botao.pinoGPIO);
    unsigned long agora = millis();

    if (leitura != botao.leituraAnterior) botao.instanteUltimaMudanca = agora;

    if (agora - botao.instanteUltimaMudanca > TEMPO_DEBOUNCE_MS) {
        if (leitura != botao.estadoEstavel) {
            botao.estadoEstavel = leitura;
            if (leitura == LOW) {
                botao.instantePressionamento = agora;
                botao.cliqueLongoDisparado = false;
            } else if (!botao.cliqueLongoDisparado) {
                if (tipo == TipoBotao::SensorDS18B20) cliqueDS();
                else cliqueBLE();
            }
        }
    }

    if (botao.estadoEstavel == LOW && !botao.cliqueLongoDisparado &&
        agora - botao.instantePressionamento >= TEMPO_CLIQUE_LONGO_MS) {
        botao.cliqueLongoDisparado = true;
        if (tipo == TipoBotao::SensorDS18B20) longoDS();
        else longoBLE();
    }

    botao.leituraAnterior = leitura;
}
