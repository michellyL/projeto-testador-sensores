#include <Lcd.h>

void lcd_init() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
}
