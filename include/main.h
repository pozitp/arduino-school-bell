#pragma once
#include <RTClib.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

/* #define RIGHT_BTN_PIN 2
#define CENTRAL_BTN_PIN 3
#define LEFT_BTN_PIN 4 */
#define BELL_PIN 9
#define TIME_DISPLAY_ZERO '0'
#define TIME_DISPLAY_COLON ':'
#define LCD_COLS 16
#define LCD_ROWS 2
// #define DEBUG_MODE

RTC_DS3231 rtc;
hd44780_I2Cexp lcd;

int status;
DateTime now;
long timeFormatted;
bool isLesson = false;
unsigned long tmr, tmr2, ms3;
int bellState = LOW;
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

const PROGMEM long schedule[16] = {8100, 8500, 900, 9400, 9550, 10350, 10550, 11350, 11550, 12350, 12500, 13300, 13400, 14200, 14300, 15100};