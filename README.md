# Testador de Sensores DS18B20

Projeto desenvolvido com ESP32 para testar múltiplos sensores de temperatura Dallas DS18B20. O sistema utiliza um display LCD 16x2 I2C para apresentar as informações e um botão para percorrer os sensores encontrados.

## Funcionalidades

- Reconhecimento de múltiplos sensores DS18B20 conectados ao mesmo barramento OneWire.
- Identificação dos sensores DS18B20 por meio de seus endereços (TAG).
- Exibição do número do sensor selecionado, temperatura e TAG no LCD.
- Exibição da quantidade de sensores encontrados pela serial durante a inicialização.
- Navegação entre os sensores de temperatura utilizando o botão DS.
- Atualização periódica da temperatura e nova busca por sensores DS18B20.

## Tecnologias utilizadas

- ESP32 DOIT DevKit V1
- C++ com framework Arduino
- PlatformIO e Visual Studio Code
- Display LCD 16x2 com interface I2C
- Protocolo OneWire
- Bibliotecas OneWire e DallasTemperature

## Arquitetura do projeto

```text
proj_testador_ds_ble/
├── include/
│   ├── Botoes.h
│   ├── Config.h
│   ├── Gpio.h
│   ├── Lcd.h
│   ├── Serial.h
│   ├── SensorDS.h
│   ├── SystemDallas.h
│   └── SystemSetup.h
├── src/
│   ├── Botoes.cpp
│   ├── Gpio.cpp
│   ├── Lcd.cpp
│   ├── main.cpp
│   ├── Serial.cpp
│   ├── SensorDS.cpp
│   ├── SystemDallas.cpp
│   └── SystemSetup.cpp
└── platformio.ini
```

## Funcionamento

### 1. Inicialização

O sistema configura a serial, o botão e o barramento I2C, inicializa o LCD e procura sensores DS18B20. A quantidade encontrada é informada pela serial. No loop principal, o sistema verifica eventos e atualizações, e encaminha os dados do sensor para os módulos de LCD e Serial.

### 2. Exibição dos sensores DS18B20

A tela principal mostra o número do sensor selecionado, sua temperatura e TAG. O botão DS avança para o próximo sensor. A temperatura é atualizada periodicamente e o barramento é pesquisado novamente para detectar sensores conectados ou removidos.

Exemplo:

```text
1 TEMP: 24.9°C
28A1B2C3D4E5F607
```

## Objetivo

Desenvolver um equipamento portátil para auxiliar no dia a dia, detectando possíveis problemas e testando o funcionamento dos sensores de forma rápida.
