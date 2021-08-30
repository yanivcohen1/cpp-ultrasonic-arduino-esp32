// user lib
#include "Header/UltrasinicPage.h"

namespace UltrasinicPage {
    // HW pins
    int potPin = 34;    // select the input pin for the potentiometer
    int ledPin = 0;   // select the pin for the LED
    int echoPin = 15; // Pin.IN, pull=None
    int trigPin = 2; // Pin.OUT, pull=None

    // SH1106_DIP sh1106_dip; // screen display

    bool ledState = 0;
    bool ledOn = false;
    int current_distance = 0;
    int sliderIn = -1;
    int last_sliderPot = -1;
    bool markForSave = false;
    String potString = "";
    bool wsConnected = true; // false to disable auto start just on www page load

    struct Send_Data {
        String distance = "distance";
        String led = "led";
        String slider = "slider";
    } SendData;

    struct Rec_Data {
        String slider = "slider";
    } RecData;

    // Create AsyncWebServer object on port 80
    AsyncWebServer* server = NULL;
    AsyncWebSocket ws("/ultrasonic_page");

    void socketSendJsonTextToAllUsers(String key, String val) {
        StaticJsonDocument<200> send;
        send[key] = String(val);
        String send_str = "";
        serializeJson(send, send_str);
        try {ws.textAll(send_str);}
        catch (...) {};
    }

    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0;
            String json_txt_msg_recive = (char*)data;
            StaticJsonDocument<200> doc_from_json;
            // Deserialize the JSON document
            DeserializationError error = deserializeJson(doc_from_json, json_txt_msg_recive);
            // Test if parsing succeeds.
            if (error) {
            Serial.print(F("sw recive deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
            }
            // read RecData.slider from json
            if (!doc_from_json[RecData.slider].isNull()) { // if exist in json 
            Serial.print("ws recive slider: ");
            String slide = doc_from_json[RecData.slider].as<const char*>();
            Serial.println(slide);
            sliderIn = slide.toInt();
            markForSave = true;
            }
        }
    }

    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                void *arg, uint8_t *data, size_t len) {
        Serial.println(String("server url: ") + server->url()); // print: /ws
        switch (type) {
            case WS_EVT_CONNECT: {
                wsConnected = true;
                Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
                socketSendJsonTextToAllUsers(SendData.slider, String(sliderIn));
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
        server->addHandler(&ws);
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
            socketSendJsonTextToAllUsers(SendData.slider, String(sliderIn));
        } else {
            if (markForSave){
                // saveLastSlider(sliderIn); save sliderIn to file
                OLED_display();
                markForSave = false;
            }
        }
        int distance = Ultrasinic::ultrasinicDistance();
        if (distance > 180 && current_distance == 180)
            return;
        if (distance > 180 && current_distance != 180) current_distance = 180;
        else current_distance = distance;
        socketSendJsonTextToAllUsers(SendData.distance, String(current_distance));
        if (current_distance > sliderIn && ledOn) {
            ledOn = false;
            digitalWrite(ledPin, LOW);
            socketSendJsonTextToAllUsers(SendData.led, String(false));
            Serial.println(String("ws sending led: ") + String(false));
        } else if (current_distance <= sliderIn && !ledOn) {
            ledOn = true;
            digitalWrite(ledPin, HIGH);
            socketSendJsonTextToAllUsers(SendData.led, String(true));
            Serial.println(String("ws sending led: ") + String(true));
        }
        OLED_display();
    }

    void setup(AsyncWebServer &_server) {
        server = &_server;
        Ultrasinic::setup(trigPin, echoPin);
        pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        initWebSocket();
        // load ultrasonic_page.html
        server->on("/ultrasonic_page.html", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/ultrasonic_page.html", "text/html");
        });
    }

    // not in paralle so no need of semafore
    void loop(void) {
        if (wsConnected) {// false to disable auto start just on ws page load
            ws.cleanupClients(); // close first connection if reach max clients connection
            main_loop(); // ultrasonic main loop
            //delay(500);
        }
    }
}