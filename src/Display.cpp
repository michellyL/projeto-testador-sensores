#include "Display.h"

void limparLinha(int linha) {
    lcd.setCursor(0, linha);
    lcd.print("                ");
    lcd.setCursor(0, linha);
}
