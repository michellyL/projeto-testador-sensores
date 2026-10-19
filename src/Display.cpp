#include "Display.h"

/**
 * Limpa a linha informada do LCD.
 * @param numeroLinha Indice da linha, normalmente 0 ou 1.
 */
void limparLinha(uint8_t numeroLinha) {
    lcd.setCursor(0, numeroLinha);
    lcd.print("                "); // Sobrescreve os 16 caracteres da linha.
    lcd.setCursor(0, numeroLinha);
}
