
#include "Botoes.h"
#include "SensorDS.h"
#include "BluetoothBLE.h"
#include "SensorUmidade.h"

void cliqueDS()
{
    modoBLE = false;
    telaDS = (telaDS + 1) % 4;
    mostrandoTag = (telaDS == 1 || telaDS == 3);
    switch (telaDS) {
        case 0: mostrarTemperatura(); break;
        case 1: mostrarTag(); break;
        case 2: mostrarTemperaturaUmidade(); break;
        case 3: mostrarTagUmidade(); break;
    }
    ultimaTemperatura = millis();
}

void longoDS()
{
    modoBLE = false;
    telaDS = 0;
    mostrandoTag = false;
    mostrarTemperatura();
    ultimaTemperatura = millis();
}

void cliqueBLE()
{
    if (!modoBLE)
    {
        modoBLE = true;
        buscarBluetooth();
        return;
    }

    if (totalBLE == 0)
    {
        buscarBluetooth();
        return;
    }

    mostrandoMAC = false;
    selecionado = (selecionado + 1) % totalBLE;
    mostrarBLE();
}

void longoBLE()
{
    if (!modoBLE)
    {
        modoBLE = true;
        buscarBluetooth();
        return;
    }

    mostrandoMAC = !mostrandoMAC;
    mostrarBLE();
}

void processarBotao(Botao &botao, int tipo)
{
    bool leitura = digitalRead(botao.pino);
    unsigned long agora = millis();

    if (leitura != botao.anterior)
    {
        botao.mudanca = agora;
    }

    if (agora - botao.mudanca >= DEBOUNCE)
    {
        if (leitura != botao.estavel)
        {
            botao.estavel = leitura;

            if (leitura == LOW)
            {
                botao.inicio = agora;
                botao.longo = false;
            }
            else
            {
                if (!botao.longo)
                {
                    if (tipo == 0)
                    {
                        cliqueDS();
                    }
                    else
                    {
                        cliqueBLE();
                    }
                }
            }
        }

        if (botao.estavel == LOW &&
            !botao.longo &&
            agora - botao.inicio >= TEMPO_LONGO)
        {

            botao.longo = true;

            if (tipo == 0)
            {
                longoDS();
            }
            else
            {
                longoBLE();
            }
        }
    }

    botao.anterior = leitura;
}
