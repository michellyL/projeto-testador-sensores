#include "BluetoothBLE.h"
#include "Display.h"

// Decodifica temperatura/umidade quando os bytes correspondem ao cabecalho esperado.
void interpretarDadosTeltonika(Dispositivo& d, const std::string& dados) {
    const uint8_t* p = reinterpret_cast<const uint8_t*>(dados.data());
    const size_t n = dados.size();
    if (n < 4) return;
    const bool cabecalho =
        ((p[0] == 0x9A && p[1] == 0x08) ||
         (p[0] == 0x08 && p[1] == 0x9A)) && p[2] == 0x01;
    if (!cabecalho) return;
    const uint8_t flags = p[3];
    size_t pos = 4;
    if ((flags & 0x01) != 0) {
        if (pos + 2 > n) return;
        int16_t bruto = static_cast<int16_t>((static_cast<uint16_t>(p[pos]) << 8) | p[pos + 1]);
        float temp = bruto / 100.0f;
        pos += 2;
        if (temp >= -40.0f && temp <= 125.0f) {
            d.temperaturaCelsius = temp;
            d.possuiTemperatura = true;
        }
    }
    if ((flags & 0x02) != 0) {
        if (pos >= n) return;
        if (p[pos] <= 100) {
            d.umidadePercentual = p[pos];
            d.possuiUmidade = true;
        }
    }
}

// Recebe anuncios de qualquer dispositivo BLE, sem filtrar fabricantes.
class ReceptorBLE : public BLEAdvertisedDeviceCallbacks {
public:
    void onResult(BLEAdvertisedDevice dev) override {
        const String mac(dev.getAddress().toString().c_str());
        int indice = -1;
        for (int i = 0; i < quantidadeDispositivosBLE; ++i) {
            if (dispositivosBLE[i].enderecoMAC == mac) {
                indice = i;
                break;
            }
        }
        if (indice < 0) {
            if (quantidadeDispositivosBLE >= 20) return;
            indice = quantidadeDispositivosBLE;
            ++quantidadeDispositivosBLE;
            dispositivosBLE[indice].enderecoMAC = mac;
            dispositivosBLE[indice].nomeDispositivo = "";
            dispositivosBLE[indice].intensidadeSinalRSSI = dev.getRSSI();
            dispositivosBLE[indice].possuiTemperatura = false;
            dispositivosBLE[indice].possuiUmidade = false;
            dispositivosBLE[indice].temperaturaCelsius = 0.0f;
            dispositivosBLE[indice].umidadePercentual = 0;
        }
        Dispositivo& dispositivo = dispositivosBLE[indice];
        if (dev.getRSSI() > dispositivo.intensidadeSinalRSSI) {
            dispositivo.intensidadeSinalRSSI = dev.getRSSI();
        }
        if (dev.haveName()) {
            dispositivo.nomeDispositivo = String(dev.getName().c_str());
        }
        if (dev.haveManufacturerData()) {
            interpretarDadosTeltonika(dispositivo, dev.getManufacturerData());
        }
    }
};

// Apresenta os dados do dispositivo BLE selecionado no LCD e no Serial.
void mostrarDispositivoBLE() {
    lcd.clear();
    if (quantidadeDispositivosBLE == 0) {
        lcd.print("NENHUM BLE");
        lcd.setCursor(0, 1);
        lcd.print("ENCONTRADO");
        return;
    }
    const Dispositivo& d = dispositivosBLE[indiceDispositivoBLESelecionado];
    Serial.print("BLE "); Serial.print(indiceDispositivoBLESelecionado + 1);
    Serial.print("/"); Serial.print(quantidadeDispositivosBLE);
    Serial.print(" | MAC: "); Serial.print(d.enderecoMAC);
    Serial.print(" | Nome: "); Serial.print(d.nomeDispositivo);
    Serial.print(" | RSSI: "); Serial.println(d.intensidadeSinalRSSI);

    if (mostrandoMAC) {
        lcd.print(d.enderecoMAC.substring(0, 9));
        lcd.setCursor(0, 1);
        lcd.print(d.enderecoMAC.substring(9));
        return;
    }
    if (d.possuiTemperatura || d.possuiUmidade) {
        lcd.print("TEMP: ");
        if (d.possuiTemperatura) {
            lcd.print(d.temperaturaCelsius, 1);
            lcd.write(static_cast<uint8_t>(223));
            lcd.print("C");
        } else {
            lcd.print("--");
        }
        lcd.setCursor(0, 1);
        lcd.print("UMID: ");
        if (d.possuiUmidade) {
            lcd.print(d.umidadePercentual);
            lcd.print("%");
        } else {
            lcd.print("--");
        }
        return;
    }
    lcd.print("BLE "); lcd.print(indiceDispositivoBLESelecionado + 1);
    lcd.print("/"); lcd.print(quantidadeDispositivosBLE);
    lcd.setCursor(0, 1);
    if (d.nomeDispositivo.length() > 0) lcd.print(d.nomeDispositivo.substring(0, 16));
    else lcd.print("SEM TEMP/UMID");
}

// Executa a busca BLE, ordena os resultados e apresenta o primeiro dispositivo.
void buscarDispositivosBluetooth() {
    lcd.clear();
    lcd.print("BUSCANDO BLE...");
    lcd.setCursor(0, 1);
    lcd.print("Aguarde 10 seg");
    quantidadeDispositivosBLE = 0;
    indiceDispositivoBLESelecionado = 0;
    mostrandoMAC = false;
    scannerBluetooth->start(10, false);
    for (int i = 0; i < quantidadeDispositivosBLE - 1; ++i) {
        for (int j = i + 1; j < quantidadeDispositivosBLE; ++j) {
            if (dispositivosBLE[j].intensidadeSinalRSSI > dispositivosBLE[i].intensidadeSinalRSSI) {
                Dispositivo temp = dispositivosBLE[i];
                dispositivosBLE[i] = dispositivosBLE[j];
                dispositivosBLE[j] = temp;
            }
        }
    }
    scannerBluetooth->clearResults();
    Serial.print("TOTAL ENCONTRADO: ");
    Serial.println(quantidadeDispositivosBLE);
    for (int i = 0; i < quantidadeDispositivosBLE; ++i) {
        Serial.print(i + 1); Serial.print(" | ");
        Serial.print(dispositivosBLE[i].enderecoMAC); Serial.print(" | ");
        Serial.print(dispositivosBLE[i].nomeDispositivo); Serial.print(" | RSSI ");
        Serial.println(dispositivosBLE[i].intensidadeSinalRSSI);
    }
    mostrarDispositivoBLE();
}

// Inicializa o BLE e configura os parametros do scanner.
void iniciarBluetooth() {
    BLEDevice::init("ESP32_TESTADOR");
    scannerBluetooth = BLEDevice::getScan();
    scannerBluetooth->setAdvertisedDeviceCallbacks(new ReceptorBLE(), true);
    scannerBluetooth->setActiveScan(true);
    scannerBluetooth->setInterval(100);
    scannerBluetooth->setWindow(99);
}
