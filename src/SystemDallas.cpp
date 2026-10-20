#include "SystemDallas.h"
#include "Botoes.h"
#include "Config.h"
#include "SensorDS.h"

extern Botao botaoDS;

void SystemDallas() {
    const bool leitura = button_read(botaoDS);
    const unsigned long agora = millis();
    if (leitura != botaoDS.leituraAnterior) {
        botaoDS.leituraAnterior = leitura;
        botaoDS.instanteUltimaMudanca = agora;
    }

    if (agora - botaoDS.instanteUltimaMudanca >= TEMPO_DEBOUNCE_MS &&
        leitura != botaoDS.estadoEstavel) {
        botaoDS.estadoEstavel = leitura;
        if (leitura == HIGH) {
            proximoSensorDS();
        }
    }

    if (agora - instanteUltimaBuscaDS >= INTERVALO_REBUSCA_DS_MS) {
        procurarSensoresDS();
        mostrarTemperatura();
        instanteUltimaTemperatura = millis();
    }

    if (millis() - instanteUltimaTemperatura >= INTERVALO_ATUALIZACAO_TEMPERATURA_MS) {
        instanteUltimaTemperatura = millis();
        mostrarTemperatura();
    }

    delay(10);
}