#include "Botoes.h"
#include "SensorDS.h"
#include "BluetoothBLE.h"

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

/*A função processarBotao é responsável por gerenciar o estado de um botão, 
*realizando leituras do seu pino GPIO e detectando mudanças de estado, 
*como cliques curtos e longos, com base em um tempo de debounce definido.
*/

void processarBotao(Botao& botao, TipoBotao tipo) {
    bool leitura = digitalRead(botao.pinoGPIO);
    unsigned long agora = millis();

    if (leitura != botao.leituraAnterior) botao.instanteUltimaMudanca = agora;

    //calcula o temp desde a última mudança de estado do botão e verifica se passou o temp de debounce definido.
    if (agora - botao.instanteUltimaMudanca > TEMPO_DEBOUNCE_MS) {
        if (leitura != botao.estadoEstavel) {
            botao.estadoEstavel = leitura; //Registra a leitura atual como o novo estado estável do botão
            if (leitura == LOW) {
                botao.instantePressionamento = agora; //Esse horário será usado para saber se o botão ficou pressionado tempo suficiente para contar como clique longo.
                botao.cliqueLongoDisparado = false;
            } else if (!botao.cliqueLongoDisparado) { //Se a leitura não é LOW, botão foi solto, e verifica 
                                                      //se o clique longo não foi disparado, então é um clique curto.
                if (tipo == TipoBotao::SensorDS18B20) cliqueDS(); //Escolhe a ação de clique curto conforme o tipo de botão
                else cliqueBLE();
            }
        }
    }

    //o botão continua pressionado && a ação de clique longo ainda não foi executada neste pressionamento 
    //&& passou o tempo mínimo definido para reconhecer um clique longo.
    if (botao.estadoEstavel == LOW && !botao.cliqueLongoDisparado && agora - botao.instantePressionamento >= TEMPO_CLIQUE_LONGO_MS) {
        botao.cliqueLongoDisparado = true; //Registra que o clique longo foi reconhecido
        if (tipo == TipoBotao::SensorDS18B20) longoDS();
        else longoBLE();
    }

    botao.leituraAnterior = leitura; //guarda a leitura atual como a leitura anterior
}
