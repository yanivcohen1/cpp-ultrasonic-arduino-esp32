// user lib
#include "user_lib/Sh1106DispClass.h" // display SH1106_DIP class
#include "UltrasinicPage.h" // UltrasinicPage
#include "user_lib/all_tests.h"
// libs
#include "Arduino.h" // arduino
#include <WiFi.h> // wifi
// #include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> // webserver
#include "SPIFFS.h" // files on flash
// #include <ArduinoJson.h> // json
#include <AsyncElegantOTA.h> // url:<ip>/update for upload files OTA(over the air)

// HW pins
//int potPin = 34;    // select the input pin for the potentiometer
//int ledPin = 0;   // select the pin for the LED
//int echoPin = 15; // Pin.IN, pull=None
//int trigPin = 2; // Pin.OUT, pull=None

SH1106_DIP sh1106_dip; // screen display

const char* ssid = "ssid-name";
const char* password = "password";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// AsyncWebSocket ws2("/ws");

void setup(void) {
  Serial.begin(115200);
  // Serial.println("in setup");
  sh1106_dip.setup();
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  //ffs_setup();
  
  // load index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico", "image/x-icon");
  });
  server.on("/chart.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/chart.min.js", "application/javascript");
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  // start UltrasinicPage
  UltrasinicPage::setup(&server);
  // Start ElegantOTA
  AsyncElegantOTA.begin(&server);    
  // Start server
  server.begin();
}

void loop(void) {
  UltrasinicPage::loop();
  delay(500);
}
