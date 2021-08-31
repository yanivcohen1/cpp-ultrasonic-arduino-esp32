#pragma once
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C 
#include <Wire.h> // my I2C display
#endif
#include "Arduino.h"
#include <U8g2lib.h>

class SH1106_DIP {

    private:
        U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
        static U8G2_SH1106_128X64_NONAME_F_HW_I2C *u8g2_s;
    public:
        SH1106_DIP();
        void setup();
        static void dispay(String l1, String l2, String l3, String l4);
};