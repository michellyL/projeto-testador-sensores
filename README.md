# Testador de Sensores DS18B20 e BLE
Projeto desenvolvido com ESP32 para testar sensores de temperatura Dallas DS18B20 e identificar dispositivos Bluetooth Low Energy (BLE).
O sistema utiliza um display LCD 16x2 para apresentar as informações e dois botões para passar entre as funções.

## Funcionalidades
- Leitura de temperatura do sensor DS18B20.
- Exibição do endereço (TAG) do sensor Dallas.
- Busca de dispositivos BLE próximos.
- Navegação entre os dispositivos encontrados.
- Visualização do nome, MAC e informações disponíveis dos dispositivos BLE.
- Leitura de temperatura e umidade dos dispositivos BLE

## Tecnologias utilizadas
- ESP32 DOIT DevKit V1
- C++ com framework Arduino
- PlatformIO e VS Code
- Display LCD 16x2 I2C
- OneWire e DallasTemperature
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
### Responsabilidades
- **main.cpp:** inicialização dos componentes e execução principal.
- **Config.h:** configurações, estruturas e variáveis compartilhadas.
- **SensorDS:** leitura de temperatura e identificação do sensor Dallas.
- **BluetoothBLE:** busca e exibição dos dispositivos Bluetooth.
- **Display:** funções auxiliares de exibição no LCD.
- **Botoes:** tratamento dos cliques e navegação entre as telas.

## Funcionamento
O equipamento inicia exibindo a temperatura do sensor Dallas. Pelos botões, é possível visualizar sua TAG ou iniciar uma busca por dispositivos BLE próximos.
Os dispositivos encontrados podem ser percorridos pelo usuário, com a possibilidade de visualizar o endereço MAC.

## Objetivo
Desenvolver um equipamento portátil para auxiliar na identificação e nos testes de sensores.
