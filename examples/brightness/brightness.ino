
#include <arduino.h>
#include "Gapa102.h"

// Example on how to use the Gapa102  
// to change brightness of the whole strip
// max brightness is 31 and min is 0

// Note: When using full white the first pixcel can flicker
// when going from full brightness to lower

unsigned long msDelay = 200; //milliseconds

const int PIXEL_COUNT = 10;//150;
const int CLOCK_PIN   = 18;//D6;
const int DATA_PIN    = 19;//D7;


Gapa102 apaStrip(CLOCK_PIN, DATA_PIN, PIXEL_COUNT);

int brightness = 31;
int incker = -1;
void setup() {
  apaStrip.setAllPixels(apaStrip.color(100,20,50));
  apaStrip.show();
}

void loop() {
    brightness+=incker;
    if (brightness < 0)
    {
      brightness = 0;
      incker = 1;
    } else if (brightness > 31) {
      brightness = 31;
      incker = -1;
    }
    apaStrip.setBrightness(brightness);
    apaStrip.show();
    delay(40);
}
