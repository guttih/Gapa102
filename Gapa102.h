// File created: 2018-09-12
#ifndef GAPA102H
#define GAPA102H

#include <Arduino.h>

// a few colors predefined ready to be sent to the strip
#define COLOR_OFF   0xe0000000 // color to turn a pixel off
#define COLOR_WHITE 0xffffffff // pixel color fully white

#define FRAME_START_NUM 0x00000000
#define FRAME_END_NUM   0xffffffff

enum COLORSETUP {SETUP_RGB,SETUP_BGR};

class Gapa102 {
    private:
        
        uint32_t   *pixels; //the strip pixels
        int        _pixelCount;
        int        _clockPin;
        int        _dataPin;
        COLORSETUP _colorSetup;
        bool       initializedFinished = false;
        void       init(uint8_t clockPin, uint8_t dataPin, uint32_t pixelCount, COLORSETUP colorSetup);
        void       writeBits(uint32_t value, uint32_t bitCount);
        void       write(uint32_t value);
        uint32_t   startFrame();
        uint32_t   ledFrame(uint32_t color);
        uint32_t   endFrame();
        
    public:
        // To create a strip with 5 pixels,
        // where clockPin is connected to pin 23 on the micro controller 
        // and   dataPin  is connected to pin 22 on the micro controller 
        // Example:
        //    Gapa102 apaStrip(23, 22, 5);

        Gapa102(uint8_t clockPin, uint8_t dataPin, uint32_t pixelCount, COLORSETUP colorSetup = SETUP_BGR);
        ~Gapa102();
        
        /*  The color function creates a formatted number which can be 
            sent to the strip to control how a pixel should be illuminated.

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
            |   |      |        |        / 
            111XXXXX RRRRRRRR GGGGGGGG BBBBBBBB
        */
        uint32_t color(uint8_t red,uint8_t green,uint8_t blue,uint8_t brightness = 31);
        
        //sets the same color to all pixels in the strip
        void setAllPixels(uint32_t color);
        void setPixelRange(uint32_t from, uint32_t to,  uint32_t color);
        //Sets color of a single pixel
        // pixelIndex Zero-based numbering of a pixel on the strip
        // a formatted color ready to be sent to the strip. use the color function to create this formatted color.
        void setPixel(uint32_t pixelIndex, uint32_t color);
        //  Gets a color of a single pixel
        // pixelIndex Zero-based numbering of a pixel on the strip
        uint32_t getPixel(uint32_t pixelIndex);

        
        //extracts the red part of a color
        uint8_t colorExtractRed(uint32_t color);
        
        //extracts the green part of a color
        uint8_t colorExtractGreen(uint32_t color);
        //extracts the blue part of a color
        uint8_t colorExtractBlue(uint32_t color);
        //extracts the brightness part of a color
        
        uint8_t colorExtractBrightness(uint32_t color);
        uint32_t pixelCount();

        // displays colors stored in the pixel strip array 
        // Set pixel values in the array by using the functions 
        // setPixel or setAllPixels.
        void show();
        // truns off all pixels on the strip and 
        // and sets all pixel values in the strip array to off
        void off();

        void setBrightness(uint8_t brightness);
};

#endif //GAPA102H
