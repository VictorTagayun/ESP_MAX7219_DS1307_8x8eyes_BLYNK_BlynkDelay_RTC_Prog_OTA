/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "b7ecd26a4a1040c7b0317e53e92b797a";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SingaporeMilitia";
char pass[] = "123Qweasd";

BlynkTimer timer;
WidgetLED led1(V0);
WidgetRTC blynk_rtc;
int ledState = LOW;

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 ds1307_rtc;

// MAX7219
#include "LedControl.h" //  need the library
#define PIN_EYES_DIN 13 // D7
#define PIN_EYES_CS 12  // D6
#define PIN_EYES_CLK 14 // D5
LedControl lc = LedControl(PIN_EYES_DIN, PIN_EYES_CLK, PIN_EYES_CS, 2);
int ledintensity = 1; // LED Intensity 0-15

#include "fontTiny.h"
int present_minute = 0;
int previous_minute = 0;
int present_hour = 0;
int previous_hour = 0;
char minute_ones[4];
int c_minute_ones = 100;
char minute_tens[4];
int c_minute_tens = 100;
char hour_ones[4];
int c_hour_ones = 100;
char hour_tens[4];
int c_hour_tens = 100;
char temp_num[8];
char temp_char;

// rotation
bool rotateMatrix0 = 1;  // rotate 0 matrix by 180 deg
bool rotateMatrix1 = 1;  // rotate 1 matrix by 180 deg

// define eye ball without pupil
byte eyeBall[8] = {
  B00111100,
  B01111110,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100
};

byte eyePupil = B11100111;

// stores current state of LEDs
byte eyeCurrent[8];
int currentX;
int currentY;
int cntLoop = 0;
int cntEffect = 0;

// min and max positions
#define MIN -2
#define MAX  2

// delays
#define DELAY_BLINK 40

// perform an effect every # of loop iterations, 0 to disable
#define EFFECT_ITERATION 4
int intensity = 1;

BLYNK_WRITE(V3) {

  ledState = param.asInt();
  digitalWrite(LED_BUILTIN, ledState);
  Serial.print("ledState : ");
  Serial.print(ledState);
  Serial.println();

  Serial.print("Hour : ");
  Serial.print(hour());
  Serial.print(" | Minute : ");
  Serial.print(minute());
  Serial.print(" | Second : ");
  Serial.print(second());
  Serial.println();

  Serial.print("Day : ");
  Serial.print(day());
  Serial.print(" | Month : ");
  Serial.print(month());
  Serial.print(" | Year : ");
  Serial.print(year());
  Serial.println();

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V2, currentDate);

}

BLYNK_WRITE(V4) {

  ledState = param.asInt();
  digitalWrite(LED_BUILTIN, ledState);

  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  ds1307_rtc.adjust(DateTime(year(), month(), day(), hour(), minute(), second()));

  DateTime now = ds1307_rtc.now();

  Serial.print("Year : ");
  Serial.print(now.year(), DEC);
  Serial.print(" Month : ");
  Serial.print(now.month(), DEC);
  Serial.print(" Day : ");
  Serial.print(now.day(), DEC);
  Serial.print(" Hour : ");
  Serial.print(now.hour(), DEC);
  Serial.print(" Minute : ");
  Serial.print(now.minute(), DEC);
  Serial.print(" Second : ");
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print("ledState : ");
  Serial.print(ledState);
  Serial.println();

  Serial.print("Hour : ");
  Serial.print(hour());
  Serial.print(" | Minute : ");
  Serial.print(minute());
  Serial.print(" | Second : ");
  Serial.print(second());
  Serial.println();

  Serial.print("Day : ");
  Serial.print(day());
  Serial.print(" | Month : ");
  Serial.print(month());
  Serial.print(" | Year : ");
  Serial.print(year());
  Serial.println();

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V2, currentDate);

}

BLYNK_CONNECTED() {
  // Synchronize time on connection

  blynk_rtc.begin();

  led1.off();
  delay(1000);
  led1.on();

  ds1307_rtc.adjust(DateTime(year(), month(), day(), hour(), minute(), second()));

  DateTime now = ds1307_rtc.now();

  Serial.print("Year : ");
  Serial.print(now.year(), DEC);
  Serial.print(" Month : ");
  Serial.print(now.month(), DEC);
  Serial.print(" Day : ");
  Serial.print(now.day(), DEC);
  Serial.print(" Hour : ");
  Serial.print(now.hour(), DEC);
  Serial.print(" Minute : ");
  Serial.print(now.minute(), DEC);
  Serial.print(" Second : ");
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.print("Hour : ");
  Serial.print(hour());
  Serial.print(" | Minute : ");
  Serial.print(minute());
  Serial.print(" | Second : ");
  Serial.print(second());
  Serial.println();

  Serial.print("Day : ");
  Serial.print(day());
  Serial.print(" | Month : ");
  Serial.print(month());
  Serial.print(" | Year : ");
  Serial.print(year());
  Serial.println();

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V2, currentDate);
}

void setup()
{
  Serial.begin(9600);

  // wifi Blynk
  Blynk.begin(auth, ssid, pass);

  // OTA
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.shutdown(1, false);
  lc.setIntensity(0, ledintensity); // sets brightness (0~15 possible values)
  lc.setIntensity(1, ledintensity);
  lc.clearDisplay(0);// clear screen
  lc.clearDisplay(1);
  Serial.println("");
  Serial.println("");

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  if (! ds1307_rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }

  if (! ds1307_rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  DateTime now = ds1307_rtc.now();

  // LED test
  // vertical line
  byte b = B10000000;
  for (int c = 0; c <= 7; c++)
  {
    for (int r = 0; r <= 7; r++)
    {
      setColumn(0, r, b);
      setColumn(1, r, b);
    }
    b = b >> 1;
    Blynk_Delay(50);
  }
  // full module
  b = B11111111;
  for (int r = 0; r <= 7; r++)
  {
    setColumn(0, r, b);
    setColumn(1, r, b);
  }
  Blynk_Delay(500);

  // clear both modules
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  Blynk_Delay(500);

  // random seed
  randomSeed(analogRead(0));

  // center eyes, crazy blink
  displayEyes(0, 0);
  Blynk_Delay(2000);
  blinkEyes(true, false);
  blinkEyes(false, true);
  Blynk_Delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
}

void display_minute_ones()
{
  for (int cntr = 0; cntr < 7; cntr++)
  {
    temp_num[cntr] = tinyFont[c_minute_ones][7 - cntr]; // copy to temp variable
  }

  for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
  {
    for (int row_cntr = 0; row_cntr < 8; row_cntr++)
    {
      minute_ones[col_cntr] = minute_ones[col_cntr] << 1; // initial shift
      temp_char = temp_num[row_cntr] & 0x01; // check the bit if 0 or 1
      minute_ones[col_cntr] = minute_ones[col_cntr] | temp_char; // AND the bit to
      temp_num[row_cntr] = temp_num[row_cntr] >> 1; // shift the array right and
    }
  }

  //  for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
  //  {
  //    lc.setRow(0, 7 - col_cntr, minute_ones[col_cntr]); // 180deg mirror
  //  }

  char c_bit;
  for (int row_cntr = 0; row_cntr < 8; row_cntr++)
  {
    for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
    {
      lc.setLed(0, 7 - col_cntr, 7 - row_cntr, true );
    }
    Blynk_Delay(150);
    for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
    {
      // lc.setLed(0, 0, 0, true); // lower left
      // lc.setLed(0, 7, 0, true); // lower right
      c_bit = minute_ones[col_cntr] >> row_cntr;
      c_bit = c_bit & 0x01;
      lc.setLed(0, 7 - col_cntr, 7 - row_cntr, c_bit ); // lower left
    }
  }
}

void display_minute_tens()
{
  for (int cntr = 0; cntr < 7; cntr++)
  {
    temp_num[cntr] = tinyFont[c_minute_tens][7 - cntr]; // copy to temp variable
  }

  for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
  {
    for (int row_cntr = 0; row_cntr < 8; row_cntr++)
    {
      minute_tens[col_cntr] = minute_tens[col_cntr] << 1; // initial shift
      temp_char = temp_num[row_cntr] & 0x01; // check the bit if 0 or 1
      minute_tens[col_cntr] = minute_tens[col_cntr] | temp_char; // AND the bit to
      temp_num[row_cntr] = temp_num[row_cntr] >> 1; // shift the array right and
    }
  }

  //  for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
  //  {
  //    lc.setRow(0, 3 - col_cntr, minute_tens[col_cntr]); // 180deg mirror
  //  }

  char c_bit;
  for (int row_cntr = 0; row_cntr < 8; row_cntr++)
  {
    for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
    {
      lc.setLed(0, 3 - col_cntr, 7 - row_cntr, true );
    }
    Blynk_Delay(150);
    for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
    {
      // lc.setLed(0, 0, 0, true); // lower left
      // lc.setLed(0, 7, 0, true); // lower right
      c_bit = minute_tens[col_cntr] >> row_cntr;
      c_bit = c_bit & 0x01;
      lc.setLed(0, 3 - col_cntr, 7 - row_cntr, c_bit ); // lower left
    }
  }

}

void display_hour_ones()
{
  for (int cntr = 0; cntr < 7; cntr++)
  {
    temp_num[cntr] = tinyFont[c_hour_ones][7 - cntr]; // copy to temp variable
  }

  for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
  {
    for (int row_cntr = 0; row_cntr < 8; row_cntr++)
    {
      hour_ones[col_cntr] = hour_ones[col_cntr] << 1; // initial shift
      temp_char = temp_num[row_cntr] & 0x01; // check the bit if 0 or 1
      hour_ones[col_cntr] = hour_ones[col_cntr] | temp_char; // AND the bit to
      temp_num[row_cntr] = temp_num[row_cntr] >> 1; // shift the array right and
    }
  }

  //  for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
  //  {
  //    lc.setRow(1, 7 - col_cntr, hour_ones[col_cntr]); // 180deg mirror
  //  }

  char c_bit;
  for (int row_cntr = 0; row_cntr < 8; row_cntr++)
  {
    for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
    {
      lc.setLed(1, 7 - col_cntr, 7 - row_cntr, true );
    }
    Blynk_Delay(150);
    for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
    {
      // lc.setLed(0, 0, 0, true); // lower left
      // lc.setLed(0, 7, 0, true); // lower right
      c_bit = hour_ones[col_cntr] >> row_cntr;
      c_bit = c_bit & 0x01;
      lc.setLed(1, 7 - col_cntr, 7 - row_cntr, c_bit ); // lower left
    }
  }
}

void display_hour_tens()
{
  for (int cntr = 0; cntr < 7; cntr++)
  {
    temp_num[cntr] = tinyFont[c_hour_tens][7 - cntr]; // copy to temp variable
  }

  for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
  {
    for (int row_cntr = 0; row_cntr < 8; row_cntr++)
    {
      hour_tens[col_cntr] = hour_tens[col_cntr] << 1; // initial shift
      temp_char = temp_num[row_cntr] & 0x01; // check the bit if 0 or 1
      hour_tens[col_cntr] = hour_tens[col_cntr] | temp_char; // AND the bit to
      temp_num[row_cntr] = temp_num[row_cntr] >> 1; // shift the array right and
    }
  }

  //  for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
  //  {
  //    lc.setRow(1, 3 - col_cntr, hour_tens[col_cntr]); // 180deg mirror
  //  }

  char c_bit;
  for (int row_cntr = 0; row_cntr < 8; row_cntr++)
  {
    for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
    {
      lc.setLed(1, 3 - col_cntr, 7 - row_cntr, true );
    }
    Blynk_Delay(150);
    for (int col_cntr = 0; col_cntr < 4 ; col_cntr++)
    {
      // lc.setLed(0, 0, 0, true); // lower left
      // lc.setLed(0, 7, 0, true); // lower right
      c_bit = hour_tens[col_cntr] >> row_cntr;
      c_bit = c_bit & 0x01;
      lc.setLed(1, 3 - col_cntr, 7 - row_cntr, c_bit ); // lower left
    }
  }
}

void display_RTC()
{
  if (! ds1307_rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  } else {
    DateTime now = ds1307_rtc.now();
    present_minute = now.minute();
    present_hour = now.hour();

    if (!((present_minute > 60) && (present_hour > 24))) {
      c_minute_ones = present_minute % 10;
      display_minute_ones();
      c_minute_tens = present_minute / 10;
      display_minute_tens();
      c_hour_ones = present_hour % 10;
      display_hour_ones();
      c_hour_tens = present_hour / 10;
      display_hour_tens();

      Serial.print("Hour : ");
      Serial.print(present_hour);
      Serial.print(" / Minutes : ");
      Serial.println(present_minute);
    }
  }
  Blynk_Delay(9000);
}

void loop()
{
  // Blynk
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
    ArduinoOTA.handle();
  }

  // move to random position, wait random time
  moveEyes(random(MIN, MAX + 1), random(MIN, MAX + 1), 50);
  Blynk_Delay(random(5, 7) * 500);

  // blink time?
  if (random(0, 5) == 0)
  {
    Blynk_Delay(500);
    blinkEyes();
    Blynk_Delay(500);
  }

  // effect time?
  if (EFFECT_ITERATION > 0)
  {
    cntLoop++;
    if (cntLoop == EFFECT_ITERATION)
    {
      cntLoop = 0;
      if (cntEffect > 13) cntEffect = 0;
      switch (cntEffect)
      {
        case 0: // cross eyes
          crossEyes();
          Blynk_Delay(1000);
          break;

        case 1: // display Time
          display_RTC();
          Blynk_Delay(20000);
          break;

        case 2: // round spin
          roundSpin(2);
          Blynk_Delay(1000);
          break;

        case 3: // display Time
          display_RTC();
          Blynk_Delay(20000);
          break;

        case 4: // crazy spin
          crazySpin(2);
          Blynk_Delay(1000);
          break;

        case 5: // display Time
          display_RTC();
          Blynk_Delay(20000);
          break;

        case 6: // meth eyes
          methEyes();
          Blynk_Delay(1000);
          break;

        case 7: // display Time
          display_RTC();
          Blynk_Delay(20000);
          break;

        case 8: // lazy eye
          lazyEye();
          Blynk_Delay(1000);
          break;

        case 9: // display Time
          display_RTC();
          Blynk_Delay(20000);
          break;

        case 10: // crazy blink
          blinkEyes(true, false);
          blinkEyes(false, true);
          Blynk_Delay(1000);
          break;

        case 11: // display Time
          display_RTC();
          Blynk_Delay(20000);
          break;

        case 12: // glow
          glowEyes(3);
          Blynk_Delay(1000);
          break;

        case 13: // display Time
          display_RTC();
          Blynk_Delay(20000);
          break;

        default:
          break;
      }
      cntEffect++;
    }
  }
}

void blinkEyes()
{
  blinkEyes(true, true);
}

/*
  This method blinks eyes as per provided params
*/
void blinkEyes(boolean blinkLeft, boolean blinkRight)
{
  // blink?
  if (!blinkLeft && !blinkRight)
    return;

  // close eyelids
  for (int i = 0; i <= 3; i++)
  {
    if (blinkLeft)
    {
      setColumn(0, i, 0);
      setColumn(0, 7 - i, 0);
    }
    if (blinkRight)
    {
      setColumn(1, i, 0);
      setColumn(1, 7 - i, 0);
    }
    Blynk_Delay(DELAY_BLINK);
  }

  // open eyelids
  for (int i = 3; i >= 0; i--)
  {
    if (blinkLeft)
    {
      setColumn(0, i, eyeCurrent[i]);
      setColumn(0, 7 - i, eyeCurrent[7 - i]);
    }
    if (blinkRight)
    {
      setColumn(1, i, eyeCurrent[i]);
      setColumn(1, 7 - i, eyeCurrent[7 - i]);
    }
    Blynk_Delay(DELAY_BLINK);
  }
}

/*
  This methods moves eyes to center position,
  then moves horizontally with wrapping around edges.
*/
void crazySpin(int times)
{
  if (times == 0)
    return;

  moveEyes(0, 0, 50);
  Blynk_Delay(500);

  byte row = eyePupil;
  for (int t = 0; t < times; t++)
  {
    // spin from center to L
    for (int i = 0; i < 5; i++)
    {
      row = row >> 1;
      row = row | B10000000;
      setColumn(0, 3, row);  setColumn(1, 3, row);
      setColumn(0, 4, row);  setColumn(1, 4, row);
      Blynk_Delay(50);
      if (t == 0)
        Blynk_Delay((5 - i) * 10); // increase delay on 1st scroll (speed up effect)
    }
    // spin from R to center
    for (int i = 0; i < 5; i++)
    {
      row = row >> 1;
      if (i >= 2)
        row = row | B10000000;
      setColumn(0, 3, row);  setColumn(1, 3, row);
      setColumn(0, 4, row);  setColumn(1, 4, row);
      Blynk_Delay(50);
      if (t == (times - 1))
        Blynk_Delay((i + 1) * 10); // increase delay on last scroll (slow down effect)
    }
  }
}

/*
  This method crosses eyes
*/
void crossEyes()
{
  moveEyes(0, 0, 50);
  Blynk_Delay(500);

  byte pupilR = eyePupil;
  byte pupilL = eyePupil;

  // move pupils together
  for (int i = 0; i < 2; i++)
  {
    pupilR = pupilR >> 1;
    pupilR = pupilR | B10000000;
    pupilL = pupilL << 1;
    pupilL = pupilL | B1;

    setColumn(0, 3, pupilR); setColumn(1, 3, pupilL);
    setColumn(0, 4, pupilR); setColumn(1, 4, pupilL);

    Blynk_Delay(100);
  }

  Blynk_Delay(2000);

  // move pupils back to center
  for (int i = 0; i < 2; i++)
  {
    pupilR = pupilR << 1;
    pupilR = pupilR | B1;
    pupilL = pupilL >> 1;
    pupilL = pupilL | B10000000;

    setColumn(0, 3, pupilR); setColumn(1, 3, pupilL);
    setColumn(0, 4, pupilR); setColumn(1, 4, pupilL);

    Blynk_Delay(100);
  }
}

/*
  This method displays eyeball with pupil offset by X, Y values from center position.
  Valid X and Y range is [MIN,MAX]
  Both LED modules will show identical eyes
*/
void displayEyes(int offsetX, int offsetY)
{
  // ensure offsets are  in valid ranges
  offsetX = getValidValue(offsetX);
  offsetY = getValidValue(offsetY);

  // calculate indexes for pupil rows (perform offset Y)
  int row1 = 3 - offsetY;
  int row2 = 4 - offsetY;

  // define pupil row
  byte pupilRow = eyePupil;

  // perform offset X
  // bit shift and fill in new bit with 1
  if (offsetX > 0) {
    for (int i = 1; i <= offsetX; i++)
    {
      pupilRow = pupilRow >> 1;
      pupilRow = pupilRow | B10000000;
    }
  }
  else if (offsetX < 0) {
    for (int i = -1; i >= offsetX; i--)
    {
      pupilRow = pupilRow << 1;
      pupilRow = pupilRow | B1;
    }
  }

  // pupil row cannot have 1s where eyeBall has 0s
  byte pupilRow1 = pupilRow & eyeBall[row1];
  byte pupilRow2 = pupilRow & eyeBall[row2];

  // display on LCD matrix, update to eyeCurrent
  for (int r = 0; r < 8; r++)
  {
    if (r == row1)
    {
      setColumn(0, r, pupilRow1);
      setColumn(1, r, pupilRow1);
      eyeCurrent[r] = pupilRow1;
    }
    else if (r == row2)
    {
      setColumn(0, r, pupilRow2);
      setColumn(1, r, pupilRow2);
      eyeCurrent[r] = pupilRow2;
    }
    else
    {
      setColumn(0, r, eyeBall[r]);
      setColumn(1, r, eyeBall[r]);
      eyeCurrent[r] = eyeBall[r];
    }
  }

  // update current X and Y
  currentX = offsetX;
  currentY = offsetY;
}

/*
  This method corrects provided coordinate value
*/
int getValidValue(int value)
{
  if (value > MAX)
    return MAX;
  else if (value < MIN)
    return MIN;
  else
    return value;
}

/*
  This method pulsates eye (changes LED brightness)
*/
void glowEyes(int times)
{
  for (int t = 0; t < times; t++)
  {
    for (int i = 2; i <= 8; i++)
    {
      lc.setIntensity(0, i);
      lc.setIntensity(1, i);
      Blynk_Delay(50);
    }

    Blynk_Delay(250);

    for (int i = 7; i >= 1; i--)
    {
      lc.setIntensity(0, i);
      lc.setIntensity(1, i);
      Blynk_Delay(25);
    }

    Blynk_Delay(150);
  }
  lc.setIntensity(0, intensity);
  lc.setIntensity(1, intensity);
}

/*
  This method moves eyes to center, out and then back to center
*/
void methEyes()
{
  moveEyes(0, 0, 50);
  Blynk_Delay(500);

  byte pupilR = eyePupil;
  byte pupilL = eyePupil;

  // move pupils out
  for (int i = 0; i < 2; i++)
  {
    pupilR = pupilR << 1;
    pupilR = pupilR | B1;
    pupilL = pupilL >> 1;
    pupilL = pupilL | B10000000;

    setColumn(0, 3, pupilR); setColumn(1, 3, pupilL);
    setColumn(0, 4, pupilR); setColumn(1, 4, pupilL);

    Blynk_Delay(100);
  }

  Blynk_Delay(2000);

  // move pupils back to center
  for (int i = 0; i < 2; i++)
  {
    pupilR = pupilR >> 1;
    pupilR = pupilR | B10000000;
    pupilL = pupilL << 1;
    pupilL = pupilL | B1;

    setColumn(0, 3, pupilR); setColumn(1, 3, pupilL);
    setColumn(0, 4, pupilR); setColumn(1, 4, pupilL);

    Blynk_Delay(100);
  }
}

/*
  This method moves both eyes from current position to new position
*/
void moveEyes(int newX, int newY, int stepDelay)
{
  // set current position as start position
  int startX = currentX;
  int startY = currentY;

  // fix invalid new X Y values
  newX = getValidValue(newX);
  newY = getValidValue(newY);

  // eval steps
  int stepsX = abs(currentX - newX);
  int stepsY = abs(currentY - newY);

  // need to change at least one position
  if ((stepsX == 0) && (stepsY == 0))
    return;

  // eval direction of movement, # of steps, change per X Y step, perform move
  int dirX = (newX >= currentX) ? 1 : -1;
  int dirY = (newY >= currentY) ? 1 : -1;
  int steps = (stepsX > stepsY) ? stepsX : stepsY;
  int intX, intY;
  float changeX = (float)stepsX / (float)steps;
  float changeY = (float)stepsY / (float)steps;
  for (int i = 1; i <= steps; i++)
  {
    intX = startX + round(changeX * i * dirX);
    intY = startY + round(changeY * i * dirY);
    displayEyes(intX, intY);
    Blynk_Delay(stepDelay);
  }
}

/*
  This method lowers and raises right pupil only
*/
void lazyEye()
{
  moveEyes(0, 1, 50);
  Blynk_Delay(500);

  // lower left pupil slowly
  for (int i = 0; i < 3; i++)
  {
    setColumn(1, i + 2, eyeBall[i + 2]);
    setColumn(1, i + 3, eyeBall[i + 3] & eyePupil);
    setColumn(1, i + 4, eyeBall[i + 4] & eyePupil);
    Blynk_Delay(150);
  }

  Blynk_Delay(1000);

  // raise left pupil quickly
  for (int i = 0; i < 3; i++)
  {
    setColumn(1, 4 - i, eyeBall[4 - i] & eyePupil);
    setColumn(1, 5 - i, eyeBall[5 - i] & eyePupil);
    setColumn(1, 6 - i, eyeBall[6 - i]);
    Blynk_Delay(25);
  }
}

/*
  This method spins pupils clockwise
*/
void roundSpin(int times)
{
  if (times == 0)
    return;

  moveEyes(2, 0, 50);
  Blynk_Delay(500);

  for (int i = 0; i < times; i++)
  {
    displayEyes(2, -1); Blynk_Delay(40); if (i == 0) Blynk_Delay(40);
    displayEyes(1, -2); Blynk_Delay(40); if (i == 0) Blynk_Delay(30);
    displayEyes(0, -2); Blynk_Delay(40); if (i == 0) Blynk_Delay(20);
    displayEyes(-1, -2); Blynk_Delay(40); if (i == 0) Blynk_Delay(10);
    displayEyes(-2, -1); Blynk_Delay(40);
    displayEyes(-2, 0); Blynk_Delay(40);
    displayEyes(-2, 1); Blynk_Delay(40); if (i == (times - 1)) Blynk_Delay(10);
    displayEyes(-1, 2); Blynk_Delay(40); if (i == (times - 1)) Blynk_Delay(20);
    displayEyes(0, 2); Blynk_Delay(40); if (i == (times - 1)) Blynk_Delay(30);
    displayEyes(1, 2); Blynk_Delay(40); if (i == (times - 1)) Blynk_Delay(40);
    displayEyes(2, 1); Blynk_Delay(40); if (i == (times - 1)) Blynk_Delay(50);
    displayEyes(2, 0); Blynk_Delay(40);
  }
}

/*
  This method sets values to matrix row
  Performs 180 rotation if needed
*/
void setColumn(int addr, int row, byte rowValue)
{
  if (((addr == 0) && (rotateMatrix0)) || (addr == 1 && rotateMatrix1))
  {
    row = abs(row - 7);
    //rowValue = bitswap(rowValue);
  }
  lc.setColumn(addr, row, rowValue);
}

// Blynk Delay
void Blynk_Delay(int milli)
{
  int end_time = millis() + milli;
  while (millis() < end_time)
  {
    if (WiFi.status() == WL_CONNECTED) {
      Blynk.run();
    }
    yield();
  }
}

