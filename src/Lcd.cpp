#include <Lcd.h>

void lcd_init() {
    lcd.init();
    lcd.backlight();
    lcd.clear();

}

void lcd_print(const char* str, uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
    lcd.print(str);
}