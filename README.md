# Testador de Sensores DS18B20 e BLE

Projeto desenvolvido com ESP32 para testar múltiplos sensores de temperatura Dallas DS18B20 e identificar dispositivos Bluetooth Low Energy (BLE). O sistema utiliza um display LCD 16x2 I2C para apresentar as informações e botões para percorrer entre os sensores e dispositivos encontrados.

## Funcionalidades

- Reconhecimento de múltiplos sensores DS18B20 conectados ao mesmo barramento OneWire.
- Leitura individual da temperatura de cada sensor encontrado.
- Identificação dos sensores DS18B20 por meio de seus endereços únicos (TAG).
- Exibição do número do sensor, temperatura e TAG no LCD.
- Exibição da quantidade de sensores encontrados durante a inicialização.
- Navegação entre os sensores de temperatura utilizando o botão DS.
- Busca de dispositivos Bluetooth Low Energy (BLE) próximos.
- Navegação entre os dispositivos BLE encontrados.
- Visualização do nome, MAC e informações disponíveis dos dispositivos BLE.
- Leitura de temperatura e umidade de dispositivos BLE 

## Tecnologias utilizadas

- ESP32 DOIT DevKit V1
- C++ com framework Arduino
- PlatformIO e Visual Studio Code
- Display LCD 16x2 com interface I2C
- Protocolo OneWire
- Bibliotecas OneWire e DallasTemperature
- Bluetooth Low Energy (BLE)

## Arquitetura do projeto

```text
proj_testador_ds_ble/
├── include/
│   ├── Config.h
│   ├── SensorDS.h
│   ├── BluetoothBLE.h
│   ├── Display.h
│   └── Botoes.h
├── src/
│   ├── main.cpp
│   ├── SensorDS.cpp
│   ├── BluetoothBLE.cpp
│   ├── Display.cpp
│   └── Botoes.cpp
└── platformio.ini
```
## Funcionamento

### 1. Inicialização
O sistema apresenta a mensagem de busca pelos sensores DS18B20.Após a busca, é exibido no LCD o total de sensores encontrados.

### 2. Exibição dos sensores DS18B20
A tela principal mostra o número do sensor selecionado, sua temperatura e TAG.
Exemplo :
```text
1 TEMP: 24.9°C
28A1B2C3D4E5F607
```

## Objetivo
Desenvolvimento de um equipamento portátil para auxiliar no dia a dia, detectando possíveis problemas e testando o funcionamento dos sensores de uma forma mais rápida.

