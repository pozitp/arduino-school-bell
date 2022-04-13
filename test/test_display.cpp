#include <Arduino.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#define LCD_COLS 16
#define LCD_ROWS 2

int status;
hd44780_I2Cexp lcd;

int main() {
    init();
    status = lcd.begin(LCD_COLS, LCD_ROWS);
    for (int i = 0; i < 16; i++) {
        lcd.setCursor(0, 0);
        lcd.write(255);
        lcd.setCursor(0, 1);
        lcd.write(255);
        delay(500);
    }
    lcd.clear();
    lcd.print("TEST SUCCESS");
    delay(5000);
    lcd.clear();
    return 0;
}