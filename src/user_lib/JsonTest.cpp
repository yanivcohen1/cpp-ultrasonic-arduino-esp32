#include <ArduinoJson.h>
#include "Arduino.h"

void json_setup() {
  // Initialize serial port
  Serial.begin(115200);
  while (!Serial) continue;

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the capacity of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> doc_from_json;

  // StaticJsonDocument<N> allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument doc(200);

  // JSON input string.
  //
  // Using a char[], as shown here, enables the "zero-copy" mode. This mode uses
  // the minimal amount of memory because the JsonDocument stores pointers to
  // the input buffer.
  // If you use another type of input, ArduinoJson must copy the strings from
  // the input to the JsonDocument, so you need to increase the capacity of the
  // JsonDocument.
  char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc_from_json, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* sensor = doc_from_json["sensor"];
  long time = doc_from_json["time"];
  double latitude = doc_from_json["data"][0];
  double longitude = doc_from_json["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);

  Serial.println("Changing Sensor");

  doc_from_json["sensor"] = "GPS2";

  // print the update sensore
  serializeJsonPretty(doc_from_json, Serial);

  // ***********************************************************************
  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> doc_to_json;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Add values in the document
  //
  doc_to_json["sensor"] = "gps";
  doc_to_json["time"] = 1351824120;

  // Add an array.
  //
  JsonArray data = doc_to_json.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  // Generate the minified JSON and send it to the Serial port.
  //
  Serial.println();
  serializeJson(doc_to_json, Serial);
  String ser_str = String("");
  serializeJson(doc_to_json, ser_str);
  Serial.println();
  Serial.print("to string cast: ");
  Serial.println(ser_str);
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
  // Start a new line
  Serial.println();
  // Generate the prettified JSON and send it to the Serial port.
  serializeJsonPretty(doc_to_json, Serial);

  Serial.print("contain field 'sensor': ");
  // JsonObject root = doc_to_json.to<JsonObject>();
  JsonVariant sense = doc_to_json["sensor"];
  Serial.println(!sense.isNull());
  if (!sense.isNull()) {
    Serial.println(sense.as<const char*>());
  }
  JsonVariant sense1 = doc_to_json["sensor1"];
  Serial.println();
  Serial.print("contain field 'sensor1': ");
  Serial.println(!sense1.isNull());

}
