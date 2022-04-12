#define BUTTON_PIN 8
#define BELL_PIN 9
// #define DEBUG_MODE

#include <Arduino.h>
#include <RTClib.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

RTC_DS3231 rtc;
hd44780_I2Cexp lcd;

int status;
DateTime now;
long timeFormatted;
bool isLesson = false;
uint16_t tmr;
uint16_t tmr2;
byte letter_P[8] = {
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B00000,
};
byte letter_Y[8] = {
    B10001,
    B10001,
    B10001,
    B01010,
    B00100,
    B01000,
    B10000,
    B00000,
};

const long schedule[16] = {8100, 8500, 900, 9400, 9550, 10350, 10550, 11350, 11550, 12350, 12500, 13300, 13400, 14200, 14300, 15100};
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

void setup()
{
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
  lcd.print("AUTO BELL");
  lcd.setCursor(5, 1);
  lcd.print("BY POZITP");
  delay(2000);
  lcd.noBacklight();
  lcd.setCursor(0, 0);
  lcd.clear();
}

void loop()
{
  now = rtc.now();
  timeFormatted = (String(now.hour()) + String(now.minute()) + String(now.second())).toInt();
  Serial.begin(9600);
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
    lcd.print("            ");
    lcd.setCursor(0, 0);
    if (now.hour() < 10)
      lcd.print("0");
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10)
      lcd.print("0");
    lcd.print(now.minute());
    lcd.print(":");
    if (now.second() < 10)
      lcd.print("0");
    lcd.print(now.second());
#ifdef DEBUG_MODE
    lcd.setCursor(0, 1);
    lcd.print("DEBUG: " + String(timeFormatted));
    lcd.print("               ");
#else
    lcd.setCursor(0, 1);
    if (isLesson == true)
    {
      lcd.print("\2");
      lcd.print("POK");
      lcd.print("                ");
    }
    else
    {
      lcd.print("\1");
      lcd.print("EPEMEHA");
      lcd.print("                ");
    }
#endif
  }
}

void yield()
{
  now = rtc.now();
  timeFormatted = (String(now.hour()) + String(now.minute()) + String(now.second())).toInt();
}