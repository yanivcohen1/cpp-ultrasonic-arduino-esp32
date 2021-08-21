// pio.ini- board_build.filesystem = littlefs
// C:\Users\yaniv\.platformio\penv\Scripts\platformio.exe run -t uploadfs
// pio -> project tasks -> 32devkit -> platform -> upload filesystem Image
#include "SPIFFS.h"
#include "Arduino.h"
 
void ffs_setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = SPIFFS.open("/tester/test.pyhtml");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.println("File Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}
 
void ffs_loop() {

}
