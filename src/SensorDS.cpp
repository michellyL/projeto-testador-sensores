#include "SensorDS.h"

static DeviceAddress enderecosSensoresDS[CAPACIDADE_MAXIMA_DS];
static int quantidadeSensoresEncontrados = 0;  
static int sensorSelecionado = 0;  

static bool enderecosSaoIguais(const DeviceAddress enderecoA, const DeviceAddress enderecoB) {
    for (uint8_t i = 0; i < 8; ++i) {
        if (enderecoA[i] != enderecoB[i]) {
            return false;
        }
    }
    return true;
}

static void copiarEndereco(DeviceAddress destino, const DeviceAddress origem) {
    for (uint8_t i = 0; i < 8; ++i) {
        destino[i] = origem[i];
    }
}

void iniciarSensoresDS() {
    sensoresTemperaturaDS.setWaitForConversion(false);
    procurarSensoresDS();
}

void procurarSensoresDS() {
    DeviceAddress enderecoAtual;
    DeviceAddress enderecoSelecionado;
    const bool tinhaSelecionado = quantidadeSensoresEncontrados > 0 &&
        sensorSelecionado < quantidadeSensoresEncontrados;

    if (tinhaSelecionado) {
        copiarEndereco(enderecoSelecionado, enderecosSensoresDS[sensorSelecionado]);
    }

    sensoresTemperaturaDS.begin();
    int encontrados = sensoresTemperaturaDS.getDeviceCount();
    quantidadeSensoresEncontrados = 0;

    for (int i = 0; i < encontrados && quantidadeSensoresEncontrados < CAPACIDADE_MAXIMA_DS; ++i) {
        if (sensoresTemperaturaDS.getAddress(enderecoAtual, static_cast<uint8_t>(i))) {
            bool duplicado = false;
            for (int j = 0; j < quantidadeSensoresEncontrados; ++j) {
                if (enderecosSaoIguais(enderecoAtual, enderecosSensoresDS[j])) {
                    duplicado = true;
                    break;
                }
            }
            if (!duplicado) {
                copiarEndereco(enderecosSensoresDS[quantidadeSensoresEncontrados], enderecoAtual);
                ++quantidadeSensoresEncontrados;
            }
        }
    }

    sensorSelecionado = 0;
    if (tinhaSelecionado) {
        for (int i = 0; i < quantidadeSensoresEncontrados; ++i) {
            if (enderecosSaoIguais(enderecoSelecionado, enderecosSensoresDS[i])) {
                sensorSelecionado = i;
                break;
            }
        }
    }
}

int quantidadeSensoresDS() {
    return quantidadeSensoresEncontrados;
}

void proximoSensorDS() {
    if (quantidadeSensoresEncontrados > 0) {
        sensorSelecionado =
            (sensorSelecionado + 1) % quantidadeSensoresEncontrados;
    }
}

int indiceSensorDSSelecionado() {
    return sensorSelecionado;
}

float lerTemperaturaSensorDS() {
    if (quantidadeSensoresEncontrados == 0) {
        return DEVICE_DISCONNECTED_C;
    }
    return sensoresTemperaturaDS.getTempC(
        enderecosSensoresDS[indiceSensorDSSelecionado()]);
}

bool obterEnderecoSensorDSSelecionado(DeviceAddress endereco) {
    if (quantidadeSensoresEncontrados == 0) {
        return false;
    }

    copiarEndereco(endereco, enderecosSensoresDS[indiceSensorDSSelecionado()]);
    return true;
}
