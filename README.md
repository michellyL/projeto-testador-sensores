# Testador de Sensores DS18B20 e BLE

Projeto desenvolvido com ESP32 para testar múltiplos sensores de temperatura Dallas DS18B20 e identificar dispositivos Bluetooth Low Energy (BLE). O sistema utiliza um display LCD 16x2 I2C para apresentar as informações e botões para navegar entre os sensores e dispositivos encontrados.

## Funcionalidades

- Reconhecimento de múltiplos sensores DS18B20 conectados ao mesmo barramento OneWire.
- Leitura individual da temperatura de cada sensor encontrado.
- Identificação dos sensores DS18B20 por meio de seus endereços únicos (TAG).
- Exibição simultânea do número do sensor, temperatura e TAG no LCD.
- Exibição da quantidade de sensores encontrados durante a inicialização.
- Navegação entre os sensores de temperatura utilizando o botão DS.
- Busca de dispositivos Bluetooth Low Energy (BLE) próximos.
- Navegação entre os dispositivos BLE encontrados.
- Visualização do nome, endereço MAC e informações disponíveis dos dispositivos BLE.
- Leitura de temperatura e umidade de dispositivos BLE compatíveis, quando essas informações estiverem disponíveis nos anúncios Bluetooth.

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

### Responsabilidades dos módulos

- **main.cpp:** inicialização dos componentes e execução do fluxo principal do sistema.
- **Config.h:** centralização das configurações, dos pinos e das variáveis compartilhadas.
- **SensorDS.cpp / SensorDS.h:** descoberta dos sensores DS18B20, identificação por TAG e leitura individual das temperaturas.
- **BluetoothBLE.cpp / BluetoothBLE.h:** busca, identificação e apresentação das informações dos dispositivos BLE.
- **Display.cpp / Display.h:** funções auxiliares para controlar e atualizar o LCD.
- **Botoes.cpp / Botoes.h:** tratamento dos botões e controle da navegação entre os sensores e dispositivos.

## Funcionamento

### 1. Inicialização

Ao ligar o equipamento, o sistema apresenta a mensagem de busca pelos sensores de temperatura DS18B20 conectados ao barramento.

Após a busca, a quantidade de sensores encontrados é exibida no LCD por aproximadamente 2 segundos. Em seguida, o equipamento entra na tela principal.

### 2. Exibição dos sensores DS18B20

A tela principal apresenta simultaneamente o número do sensor selecionado, sua temperatura e sua TAG.

Exemplo de exibição no LCD:

```text
1 TEMP: 24.9°C
28A1B2C3D4E5F607
```

A TAG é exibida integralmente na segunda linha do display, utilizando os 16 caracteres disponíveis.

O botão DS permite avançar para o próximo sensor encontrado. Ao chegar ao último, a navegação retorna ao primeiro sensor.

### 3. Busca e navegação BLE

O sistema mantém a funcionalidade de busca por dispositivos Bluetooth Low Energy próximos. Os dispositivos encontrados podem ser percorridos utilizando os controles previstos no projeto.

Conforme as informações disponibilizadas por cada dispositivo, o equipamento pode apresentar nome, endereço MAC, temperatura e umidade.

A disponibilidade dessas informações depende dos dados transmitidos pelo dispositivo BLE.

## Barramento dos sensores DS18B20

Os sensores DS18B20 utilizam o protocolo OneWire, que permite conectar vários sensores ao mesmo barramento de dados.

Nesta versão, o barramento está configurado para utilizar a GPIO 4 do ESP32, identificada como D4 no projeto.

Cada sensor possui um endereço único, utilizado para diferenciá-lo dos demais conectados ao barramento.

A quantidade de sensores reconhecidos depende da montagem elétrica, da qualidade das conexões, do comprimento dos cabos e dos limites de memória e configuração do sistema.

**Atenção:** confirme a pinagem do conector e do chicote antes de conectar os sensores. A alimentação, o GND, o sinal de dados e o resistor de pull-up devem estar corretamente configurados.

## Objetivo

Desenvolver um equipamento portátil para auxiliar na identificação, verificação e leitura de múltiplos sensores de temperatura DS18B20, mantendo também a capacidade de detectar e consultar informações de dispositivos Bluetooth Low Energy.

A organização modular facilita a manutenção do código e a implementação de futuras funcionalidades.
