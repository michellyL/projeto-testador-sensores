#include <Lcd.h>

void lcd_init() {
    lcd.init();
    lcd.setCursor(0, 1);
    lcd.backlight();
    lcd.clear();
}

void lcd_write(const char* msg, uint8_t linha, uint8_t coluna) {
    lcd.setCursor(coluna, linha);
    lcd.print(msg);
}
