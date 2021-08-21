#include "Header/user_lib/Sh1106DispClass.h"

// const char s[] PROGMEM = "Progmem"; // PROGMEM meens write to flash and to read it: (class __FlashStringHelper *)s
U8G2_SH1106_128X64_NONAME_F_HW_I2C* SH1106_DIP::u8g2_s = NULL;

SH1106_DIP::SH1106_DIP() : u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE) {
    SH1106_DIP::u8g2_s = &u8g2;
};

void SH1106_DIP::setup() {
    SH1106_DIP::u8g2_s->begin();
    Serial.println("setup");
};

void SH1106_DIP::dispay(String line1, String line2, String line3, String line4) {
  // u8g2.clear(); // clear screen 
  SH1106_DIP::u8g2_s->clearBuffer();					// clear the internal memory
  SH1106_DIP::u8g2_s->setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  SH1106_DIP::u8g2_s->drawStr(0,10, line1.c_str());
  SH1106_DIP::u8g2_s->drawStr(0,20, line2.c_str()); // write something to the internal memory
  SH1106_DIP::u8g2_s->drawStr(0,30, line3.c_str()); // write something to the internal memory
  SH1106_DIP::u8g2_s->drawStr(0,40, line4.c_str()); // write something to the internal memory
  SH1106_DIP::u8g2_s->sendBuffer();					// transfer internal memory to the display
  delay(10); // 10ms to display 
}
