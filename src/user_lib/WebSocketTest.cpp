#include "Arduino.h"
#include <WiFi.h>
// #include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
namespace WebServer
{
  // Replace with your network credentials
  const char* ssid = "HOTBOX-89BA-yaniv";
  const char* password = "0528728544";

  bool ledState = 0;
  const int ledPin = 27;

  // Create AsyncWebServer object on port 80
  AsyncWebServer server(80);
  AsyncWebSocket ws("/ws");

  void notifyClients() {
    ws.textAll(String(ledState));
  }

  void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      data[len] = 0;
      if (strcmp((char*)data, "toggle") == 0) {
        ledState = !ledState;
        notifyClients();
      }
    }
  }

  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
              void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
    }
  }

  void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
  }

  String processor(const String& var){
    Serial.println(var);
    if(var == "STATE"){
      if (ledState){
        return "ON";
      }
      else{
        return "OFF";
      }
    }
    return "";
  }

  void ws_setup(){
    // Serial port for debugging purposes
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    
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

    initWebSocket();

    // Route for root / web page
    /* server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
          request->send_P(200, "text/html", index_html, processor);
    }); */
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    // Start server
    server.begin();
  }

  void ws_loop() {
    ws.cleanupClients();
    digitalWrite(ledPin, !ledState);
  }
}