#include "BluetoothBLE.h"
#include "Display.h"

void interpretarTeltonika(Dispositivo& d, const std::string& dados) {
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
            d.temperatura = temp;
            d.temTemp = true;
        }
    }
    if ((flags & 0x02) != 0) {
        if (pos >= n) return;
        if (p[pos] <= 100) {
            d.umidade = p[pos];
            d.temUmid = true;
        }
    }
}

// Recebe anuncios de qualquer dispositivo BLE, sem filtrar fabricantes.
class ReceptorBLE : public BLEAdvertisedDeviceCallbacks {
public:
    void onResult(BLEAdvertisedDevice dev) override {
        const String mac(dev.getAddress().toString().c_str());
        int indice = -1;
        for (int i = 0; i < totalBLE; ++i) {
            if (lista[i].mac == mac) {
                indice = i;
                break;
            }
        }
        if (indice < 0) {
            if (totalBLE >= MAX_BLE) return;
            indice = totalBLE;
            ++totalBLE;
            lista[indice].mac = mac;
            lista[indice].nome = "";
            lista[indice].rssi = dev.getRSSI();
            lista[indice].temTemp = false;
            lista[indice].temUmid = false;
            lista[indice].temperatura = 0.0f;
            lista[indice].umidade = 0;
        }
        Dispositivo& dispositivo = lista[indice];
        if (dev.getRSSI() > dispositivo.rssi) {
            dispositivo.rssi = dev.getRSSI();
        }
        if (dev.haveName()) {
            dispositivo.nome = String(dev.getName().c_str());
        }
        if (dev.haveManufacturerData()) {
            interpretarTeltonika(dispositivo, dev.getManufacturerData());
        }
    }
};

void mostrarBLE() {
    lcd.clear();
    if (totalBLE == 0) {
        lcd.print("NENHUM BLE");
        lcd.setCursor(0, 1);
        lcd.print("ENCONTRADO");
        return;
    }
    const Dispositivo& d = lista[selecionado];
    Serial.print("BLE "); Serial.print(selecionado + 1);
    Serial.print("/"); Serial.print(totalBLE);
    Serial.print(" | MAC: "); Serial.print(d.mac);
    Serial.print(" | Nome: "); Serial.print(d.nome);
    Serial.print(" | RSSI: "); Serial.println(d.rssi);

    if (mostrandoMAC) {
        lcd.print(d.mac.substring(0, 9));
        lcd.setCursor(0, 1);
        lcd.print(d.mac.substring(9));
        return;
    }
    if (d.temTemp || d.temUmid) {
        lcd.print("TEMP: ");
        if (d.temTemp) {
            lcd.print(d.temperatura, 1);
            lcd.write(static_cast<uint8_t>(223));
            lcd.print("C");
        } else {
            lcd.print("--");
        }
        lcd.setCursor(0, 1);
        lcd.print("UMID: ");
        if (d.temUmid) {
            lcd.print(d.umidade);
            lcd.print("%");
        } else {
            lcd.print("--");
        }
        return;
    }
    lcd.print("BLE "); lcd.print(selecionado + 1);
    lcd.print("/"); lcd.print(totalBLE);
    lcd.setCursor(0, 1);
    if (d.nome.length() > 0) lcd.print(d.nome.substring(0, 16));
    else lcd.print("SEM TEMP/UMID");
}

void buscarBluetooth() {
    lcd.clear();
    lcd.print("BUSCANDO BLE...");
    lcd.setCursor(0, 1);
    lcd.print("Aguarde 10 seg");
    totalBLE = 0;
    selecionado = 0;
    mostrandoMAC = false;
    scannerBLE->start(10, false);
    for (int i = 0; i < totalBLE - 1; ++i) {
        for (int j = i + 1; j < totalBLE; ++j) {
            if (lista[j].rssi > lista[i].rssi) {
                Dispositivo temp = lista[i];
                lista[i] = lista[j];
                lista[j] = temp;
            }
        }
    }
    scannerBLE->clearResults();
    Serial.print("TOTAL ENCONTRADO: ");
    Serial.println(totalBLE);
    for (int i = 0; i < totalBLE; ++i) {
        Serial.print(i + 1); Serial.print(" | ");
        Serial.print(lista[i].mac); Serial.print(" | ");
        Serial.print(lista[i].nome); Serial.print(" | RSSI ");
        Serial.println(lista[i].rssi);
    }
    mostrarBLE();
}

void iniciarBluetooth() {
    BLEDevice::init("ESP32_TESTADOR");
    scannerBLE = BLEDevice::getScan();
    scannerBLE->setAdvertisedDeviceCallbacks(new ReceptorBLE(), true);
    scannerBLE->setActiveScan(true);
    scannerBLE->setInterval(100);
    scannerBLE->setWindow(99);
}
