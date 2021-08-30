#pragma once
#include "Header/user_lib/Sh1106DispClass.h" // display SH1106_DIP class
#include "Header/user_lib/Ultrasinic.h" // ultrasinic calc distance
// libs
#include "Arduino.h" // arduino
// #include <WiFi.h> // wifi
// #include <AsyncTCP.h>
#include <ESPAsyncWebserver.h> // webserver
#include "SPIFFS.h" // files on flash
#include <ArduinoJson.h> // json

namespace UltrasinicPage
{
    void setup(AsyncWebServer &_server);
    void loop(void);
}