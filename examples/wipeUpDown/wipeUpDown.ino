
#include <arduino.h>
#include "Gapa102.h"

// Example on how to use the Gapa102  
// to wipe light up and down.
// Both from one to all pixels on and a single pixel on.

unsigned long msDelay = 40; //milliseconds

const int PIXEL_COUNT = 10;//153;
const int CLOCK_PIN   = 18;//D6;
const int DATA_PIN    = 19;//D7;


Gapa102 apaStrip(CLOCK_PIN, DATA_PIN, PIXEL_COUNT);

void wipeDown(Gapa102 &strip, bool oneDot, unsigned long msDelay, uint32_t color) {
    uint32_t i = strip.pixelCount();
    while( i > 0 ) {
        if (oneDot && i < strip.pixelCount() ) {
            strip.setPixel(i, strip.color(0,0,0));
        }
        i--;
        strip.setPixel(i, color);
        
        strip.show();
        delay(msDelay);
    }

    if (oneDot) {
        strip.setPixel(0, COLOR_OFF);
        strip.show();
    }
}

void wipeUp(Gapa102 &strip, bool oneDot, unsigned long msDelay, uint32_t color) {
    uint32_t i = 0;
    do {
        if ( oneDot && i > 0 ) {
            strip.setPixel(i-1, COLOR_OFF);
        }
        strip.setPixel(i, color);
        strip.show();
        delay(msDelay);
        i++;
    } while( i < strip.pixelCount() );

    if ( oneDot ) {
        strip.setPixel(strip.pixelCount() -1, COLOR_OFF);
        strip.show();
    }
}

void setup() {
}

void loop() {
    wipeUp  ( apaStrip, false, msDelay, apaStrip.color(255,  0,  0) ); // RED
    delay(1000);
    wipeDown( apaStrip, false, msDelay, apaStrip.color(  0,  0,255) ); // BLUE
    delay(2000);
    wipeUp  ( apaStrip,  true, msDelay, apaStrip.color(255,255,  0) ); // YELLOW
    delay(1000);
    wipeDown( apaStrip,  true, msDelay, apaStrip.color(  0,255,  0) ); // GREEN
    delay(2000);
}
