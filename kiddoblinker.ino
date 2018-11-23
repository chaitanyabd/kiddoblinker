/*

Built for the ESP-01.

To know more about this project, have a peek at my writeup at https://my.infinitybeam.io/forum/my-latest-project-with-infinitybeam
Github Repo at https://github.com/chaitanyabd/kiddoblinker

-Chaitanya Dhareshwar

*/


#include <ESP8266WiFi.h>
#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

enum BLINK_STYLE : byte { OFF = 0, ONE = 1, TWO = 2, ONETWO = 3, FASTBLINK = 4, PURPLEONE = 5, BROWNTWO = 6, NAVYONETWO = 7, ORANGEFASTBLINK = 8, BREATHE };
BLINK_STYLE blinky = OFF;
BLINK_STYLE nextstate = ONE;
uint32 timmy = 0;

void toggle() {
  Serial.println(".");
  if (millis() - timmy < 500) return; // less than 500ms, debounce
  timmy = millis();
  blinky = nextstate;
  nextstate = (BLINK_STYLE)(((byte)nextstate) + 1);
  if (nextstate > BREATHE) nextstate = ONE;
}

void setup() {
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();

  Serial.begin(115200);
  while (!Serial);
  Serial.println("Begin");

  attachInterrupt(0, toggle, FALLING);  // NOTE: GPIO0 need to be full ground when connected for FLASHing!!

  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  timmy = millis();
  leds[0] = CRGB(0,0,0);
  
}
byte R = 0, G = 0, B = 0;

void loop() {
  switch (blinky) {
    default:
      leds[0].setRGB(R, G, B); FastLED.show();
      R += 50 + random(0, 30);
      B += 50 + random(0, 30);
      G += 50 + random(0, 30);
      break;
    case ONE:
      leds[0].setRGB(200, 0, 0); FastLED.show();
      delay(500);
      leds[0].setRGB(0, 0, 0); FastLED.show();
      break;
    case TWO:
      leds[0].setRGB(0, 200, 0); FastLED.show();
      delay(200);
      leds[0].setRGB(0, 0, 0); FastLED.show();
      delay(200);
      leds[0].setRGB(0, 200, 0); FastLED.show();
      delay(200);
      leds[0].setRGB(0, 0, 0); FastLED.show();
      break;
    case ONETWO:
      for (int i = 0; i < 3; i++) {
        leds[0].setRGB(200, 200, 0); FastLED.show();
        delay(100);
        leds[0].setRGB(0, 0, 0); FastLED.show();
      }
      break;
    case FASTBLINK:
      leds[0].setRGB(0, 200, 200); FastLED.show();
      delay(100);
      leds[0].setRGB(0, 0, 0); FastLED.show();
      break;
    case PURPLEONE:
      leds[0] = CRGB::Purple; FastLED.show();
      delay(500);
      leds[0].setRGB(0, 0, 0); FastLED.show();
      break;
    case BROWNTWO:
      leds[0] = CRGB::SaddleBrown; FastLED.show();
      delay(200);
      leds[0].setRGB(0, 0, 0); FastLED.show();
      delay(200);
      leds[0] = CRGB::SaddleBrown; FastLED.show();
      delay(200);
      leds[0].setRGB(0, 0, 0); FastLED.show();
      break;
    case NAVYONETWO:
      for (int i = 0; i < 3; i++) {
        leds[0] = CRGB::Navy; FastLED.show();
        delay(100);
        leds[0].setRGB(0, 0, 0); FastLED.show();
      }
      break;
    case ORANGEFASTBLINK:
      leds[0] = CRGB::Orange; FastLED.show();
      delay(100);
      leds[0].setRGB(0, 0, 0); FastLED.show();
      break;
    case BREATHE:
      int vv = (millis() % 2048);
      if (vv > 1023) vv = 2047 - vv;
      vv = vv / 5;
      leds[0].setRGB((byte)vv, (byte)vv, (byte)vv); FastLED.show();
      break;
  }
  FastLED.setBrightness(80);
  if (millis() - timmy > 5000) {
    timmy = 0;
    blinky = OFF;
  }
  FastLED.delay(100);
}
