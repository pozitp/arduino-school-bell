#include <Arduino.h>
#include "main.h"

int main()
{
  init();
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BELL_PIN, OUTPUT);

  if (!rtc.begin())
  {
    while (1)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(10);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  if (rtc.lostPower())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if (lcd.begin(LCD_COLS, LCD_ROWS))
  {
    hd44780::fatalError(status);
  }
  lcd.createChar(1, letter_P);
  lcd.createChar(2, letter_Y);
  lcd.setCursor(2, 0);
  lcd.print(F("AUTO BELL"));
  lcd.setCursor(5, 1);
  lcd.print(F("BY POZITP"));
  delay(2000);
  lcd.noBacklight();
  lcd.setCursor(0, 0);
  lcd.clear();

  for (;;)
  {
    now = rtc.now();
    timeFormatted = now.hour() * 10000L + now.minute() * 100 + now.second();
    if (now.dayOfTheWeek() != 0 && now.dayOfTheWeek() != 6)
    {
      for (int x = 0; x < 16; x = x + 1)
      {
        if (schedule[x] == timeFormatted)
        {
          uint16_t ms = millis() & 0xFFFF;
          if (ms - tmr > 3000)
          {
            tmr = ms;
            isLesson = !isLesson;
            tone(BELL_PIN, 900, 3000);
          }
        }
      }
    }
    uint16_t ms2 = millis() & 0xFFFF;
    if (ms2 - tmr2 > 1000)
    {
      tmr2 = ms2;
      lcd.print(F("            "));
      lcd.setCursor(0, 0);
      if (now.hour() < 10)
        lcd.print(TIME_DISPLAY_ZERO);
      lcd.print(now.hour());
      lcd.print(TIME_DISPLAY_COLON);
      if (now.minute() < 10)
        lcd.print(TIME_DISPLAY_ZERO);
      lcd.print(now.minute());
      lcd.print(TIME_DISPLAY_COLON);
      if (now.second() < 10)
        lcd.print(TIME_DISPLAY_ZERO);
      lcd.print(now.second());
#ifdef DEBUG_MODE
      lcd.setCursor(0, 1);
      lcd.print(F("DEBUG: "));
      lcd.print(timeFormatted);
      lcd.print(F("               "));
#else
      lcd.setCursor(0, 1);
      if (isLesson == true)
      {
        lcd.print(F("\2"));
        lcd.print(F("POK"));
        lcd.print(F("                "));
      }
      else
      {
        lcd.print(F("\1"));
        lcd.print(F("EPEMEHA"));
        lcd.print(F("                "));
      }
#endif
    }
  }
  return 0;
}