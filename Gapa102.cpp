// File created: 2018-09-12
#include "Gapa102.h"

Gapa102::Gapa102(uint8_t clockPin, uint8_t dataPin, uint32_t pixelCount, COLORSETUP colorSetup) {
    init(clockPin, dataPin, pixelCount, colorSetup);
}
Gapa102::~Gapa102() {
    delete [] pixels;
}

//this function should only be called from the constructor
void Gapa102::init(uint8_t clockPin, uint8_t dataPin, uint32_t pixelCount, COLORSETUP colorSetup) {
    _clockPin  = clockPin;
    _dataPin   = dataPin;
    _pixelCount= pixelCount;
    _colorSetup = colorSetup;

    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin,  OUTPUT);

    pixels = new uint32_t[pixelCount];
    off();
    initializedFinished = true;
}

void Gapa102::write(uint32_t value) {
  writeBits(value, sizeof(uint32_t)*8);
}
void Gapa102::writeBits(uint32_t value, uint32_t bitCount) {
  uint32_t pos;
  //alternetive use shiftOut 
  for (pos = 0; pos < bitCount; pos++) {
      //  if (initializedFinished) { Serial.print(value >> ((bitCount - 1) - pos) & 1); }
     digitalWrite(_dataPin, value >> ((bitCount - 1) - pos) & 1);
     digitalWrite(_clockPin, HIGH);
     digitalWrite(_clockPin, LOW);
   }
   // if (initializedFinished){ Serial.print(" ");Serial.println(value); }
}

uint32_t Gapa102::startFrame(){
    write(FRAME_START_NUM);
}
uint32_t Gapa102::ledFrame(uint32_t color){
    write(color);
}
uint32_t Gapa102::endFrame(){
    //write(FRAME_END_NUM);
    write(FRAME_START_NUM);
}

 void Gapa102::show() {
    startFrame();
     for(uint32_t  i = 0; i < _pixelCount; i++) {
        ledFrame(pixels[i]);
    }
    endFrame();
}

//tuns off all pixels
void Gapa102::off() {
    setAllPixels(COLOR_OFF);
    show();
}

/*
 Creates a formatted number which can be sent to the strip to control how a pixel should be illuminated.

 Parameters:
     red       : How much should the red   diode be illuminated where 0 is off and 255 is fully lit
     green     : How much should the green diode be illuminated where 0 is off and 255 is fully lit
     blue      : How much should the blue  diode be illuminated where 0 is off and 255 is fully lit
     brightness: How bright should the red, green and blue diodes be illuminated. you can use this
                 variable to dimm all diodes in a pixel at ones. 0 is off and 31 is fully lit. 

 Returns a 32 bit number formatted like this:                                          Decimal values 
       Bits are always 111 this reprisents starting of a led frame.                   /
      /    Bits for the brightness level                         :possible values (0 -  31)
     |    /       Bits for the red color diode                   :possible values (0 - 255)
     |   |       /         Bits for the green color diode        :possible values (0 - 255)
     |   |      |         /        Bits for the blue color diode :possible values (0 - 255)
     |     |      |        |        / 
    111XXXXX RRRRRRRR GGGGGGGG BBBBBBBB
*/
uint32_t Gapa102::color(uint8_t red,uint8_t green,uint8_t blue,uint8_t brightness){
    uint8_t wRed, wBlue;
    if (_colorSetup == SETUP_RGB) {
        wRed = red; 
        wBlue = blue;
    } else {
        wRed = blue; 
        wBlue = red;
    }
    return (((uint32_t)brightness & 31) | 0b11100000) << 24 |
            ((uint32_t)wRed)   << 16                    |
            ((uint32_t)green) <<  8                    |
            ((uint32_t)wBlue) ;
}


void Gapa102::setAllPixels(uint32_t color) {
    for(uint32_t i = 0; i < _pixelCount; i++) {
            pixels[i]=color;
    }
}
void Gapa102::setPixelRange(uint32_t from, uint32_t to,  uint32_t color) {
        if (to >= _pixelCount || from > to)
            return; // error  out of bounce

        for(uint32_t i = from; i < to+1; i++) {
                    pixels[i]=color;
        }
}

// if pixelIndex is out of bounds nothing is changed;
void Gapa102::setPixel(uint32_t pixelIndex, uint32_t color) {
    if (pixelIndex < _pixelCount)
        pixels[pixelIndex] = color;
}

uint32_t Gapa102::pixelCount() { 
    return _pixelCount; 
}

// if pixelIndex is out of bounds 0 is retuned;
uint32_t Gapa102::getPixel(uint32_t pixelIndex) {
    if (pixelIndex < _pixelCount)
        return pixels[pixelIndex];
    return 0;
}

//extracts the red part of a color
uint8_t Gapa102::colorExtractRed(uint32_t color) {
    if (_colorSetup == SETUP_RGB) {
        return (color & 0x00ff0000) >> 16;
    }
    else {
        return colorExtractBlue(color);
    }
}

//extracts the green part of a color
uint8_t Gapa102::colorExtractGreen(uint32_t color) {
    return (color & 0x0000ff00) >> 8;
}
//extracts the blue part of a color
uint8_t Gapa102::colorExtractBlue(uint32_t color) {
    if (_colorSetup == SETUP_RGB) {
        return (color & 0x000000ff);
    }
    else {
        return colorExtractRed(color);
    }
}

//extracts the brightness part of a color
uint8_t Gapa102::colorExtractBrightness(uint32_t color) {
    return (color & 0x1f000000) >> 24; 
}

void Gapa102::setBrightness(uint8_t brightness) {
    //write(0xE0FFFFFF | ((uint32_t)(uint8_t)brightness) << 24 );
    for(uint32_t i = 0; i < _pixelCount; i++) {
            pixels[i] = (pixels[i] & 0xE0FFFFFF) | (((uint32_t)brightness) << 24 );
    }
}



