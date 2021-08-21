// user lib
#include "Header/user_lib/Sh1106DispClass.h" // display SH1106_DIP class
#include "Header/user_lib/Ultrasinic.h" // ultrasinic calc distance
// libs
#include "Arduino.h" // arduino
#include <WiFi.h> // wifi
// #include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> // webserver
#include "SPIFFS.h" // files on flash
#include <ArduinoJson.h> // json

// HW pins
int potPin = 34;    // select the input pin for the potentiometer
int ledPin = 0;   // select the pin for the LED
int echoPin = 15; // Pin.IN, pull=None
int trigPin = 2; // Pin.OUT, pull=None

SH1106_DIP sh1106_dip; // screen display

bool ledState = 0;
bool ledOn = false;
int current_distance = 0;
int sliderIn = -1;
int last_sliderPot = -1;
bool markForSave = false;
String potString = "";

const char* ssid = "HOTBOX-89BA-yaniv";
const char* password = "0528728544";

struct  Send_Data {
    String distance = "distance";
    String led = "led";
    String slider = "slider";
} SendData;

struct  Rec_Data {
   String slider = "slider";
} RecData;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    StaticJsonDocument<200> doc_from_json;
    String json_txt_msg_recive = (char*)data;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc_from_json, json_txt_msg_recive);
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("sw recive deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    JsonVariant sense = doc_from_json[RecData.slider];
    if (!sense.isNull()) {
      Serial.print("ws recive: ");
      Serial.println(sense.as<const char*>());
      String slide = sense.as<const char*>();
      sliderIn = slide.toInt();
      markForSave = true;
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
            void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT: {
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      StaticJsonDocument<200> send;
      send[SendData.slider] = String(sliderIn);
      String send_str = String("");
      serializeJson(send, send_str);
      try {ws.textAll(send_str);}
      catch (...) {};
      break;
    }
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

void OLED_display() {
    String line1 = String("Distance System!"); // 16 lines a row
    String line2 = String("Distance is: ") + String(current_distance);
    String line3 = String("Distance Set ") + String(sliderIn);
    String line4 = String("Alarm is ") + String(ledOn ? "ON" : "OFF");
    SH1106_DIP::dispay(line1, line2, line3, line4);
}

void main_loop() {  // main loop delay 1s
    int curt_slider = round(analogRead(potPin) * 100 / 4095);
    if (!(last_sliderPot == curt_slider || last_sliderPot + 1 == curt_slider ||
            last_sliderPot - 1 == curt_slider)) { // || firstUpdateSlider == "update" ) {
        markForSave = true;
        last_sliderPot = curt_slider;
        sliderIn = curt_slider;
        Serial.println(String("slider set to is: ") + String(sliderIn));
        OLED_display();
        // send slider val to ws
        StaticJsonDocument<200> send;
        send[SendData.slider] = String(sliderIn);
        String send_str = String("");
        serializeJson(send, send_str);
        try {ws.textAll(send_str);} // send text to all users
        catch (...) {};
    } else {
        if (markForSave){
            // saveLastSlider(sliderIn); save sliderIn to file
            OLED_display();
            markForSave = false;
        }
    }
    int distance = Ultrasinic::ultrasinicDistance(trigPin, echoPin);
    if (distance > 180 && current_distance == 180)
        return;
    if (distance > 180 && current_distance != 180) current_distance = 180;
    else current_distance = distance;
    StaticJsonDocument<200> send;
    send[SendData.distance] = String(current_distance);
    String send_str = String("");
    serializeJson(send, send_str);
    try {ws.textAll(send_str);}
    catch (...) {};
    if (current_distance > sliderIn && ledOn) {
        ledOn = false;
        digitalWrite(ledPin, LOW);
        StaticJsonDocument<200> send;
        send[SendData.led] = String(false);
        String send_str = String("");
        serializeJson(send, send_str);
        try {ws.textAll(send_str);}
        catch (...) {};
        Serial.println(String("ws sending led: ") + String(false));
    } else if (current_distance <= sliderIn && !ledOn) {
        ledOn = true;
        digitalWrite(ledPin, HIGH);
        StaticJsonDocument<200> send;
        send[SendData.led] = String(true);
        String send_str = String("");
        serializeJson(send, send_str);
        try {ws.textAll(send_str);}
        catch (...) {};
        Serial.println(String("ws sending led: ") + String(true));
    }
    OLED_display();
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("in setup");
  sh1106_dip.setup();
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
  // load index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/ultrasonic_page.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/ultrasonic_page.html", "text/html");
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
  // Start server
  server.begin();
}

void loop(void) {
  ws.cleanupClients();
  main_loop();
  delay(500);
}