/***************************************************
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

// RTC
#include "RTClib.h"

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// EINK
#include "Adafruit_ThinkInk.h"

#define EPD_CS      15
#define EPD_DC      33
#define SRAM_CS     32
#define EPD_RESET   -1 // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY    -1 // can set to -1 to not use a pin (will wait a fixed delay)

// 2.13" Monochrome displays with 250x122 pixels and SSD1675 chipset
ThinkInk_213_Mono_B72 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  // The following line can be uncommented if the time needs to be reset.
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  rtc.start();

  //EINK
  display.begin(THINKINK_MONO);
}

void loop() {

  // Gets the current date and time, and writes it to the Eink display.
  String currentTime = getDateTimeAsString();


  // writes the current time on the bottom half of the display (y is height)
  drawText(currentTime, EPD_BLACK, 2, 0, 75);

  // Draws a line from the leftmost pixel, on line 50, to the rightmost pixel (250) on line 50.
  display.drawLine(0, 50, 250, 50, EPD_BLACK);
  display.display();


  // waits 180 seconds (3 minutes) as per guidelines from adafruit.
  delay(180000);
}

void drawText(String text, uint16_t color, int textSize, int x, int y) {
  display.clearBuffer();
  display.setCursor(x, y);
  display.setTextColor(color);
  display.setTextSize(textSize);
  display.setTextWrap(true);
  display.print(text);
  display.display(); // Needed to tell screen to update
}

String getDateTimeAsString() {
  DateTime now = rtc.now();

  //Prints the date and time to the Serial monitor for debugging.
  /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  */

  // Converts the date and time into a human-readable format.
  char humanReadableDate[20];
  sprintf(humanReadableDate, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());

  return humanReadableDate;
}
