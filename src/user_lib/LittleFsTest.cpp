// pio.ini- board_build.filesystem = littlefs
// C:\Users\yaniv\.platformio\penv\Scripts\platformio.exe run -t uploadfs
// pio -> project tasks -> 32devkit -> platform -> upload filesystem Image
#include "SPIFFS.h"
#include "Arduino.h"
 
void ffs_setup() {
  // Serial.begin(115200);
  Serial.println();
  Serial.println("start ffs test");
  delay(1000);
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // ********************** append
  File fileToAppend = SPIFFS.open("/tester/test.txt", FILE_APPEND); // open to append File - FILE_WRITE, open for write
  if(!fileToAppend){
    Serial.println("There was an error opening the file for appending");
    return;
  }
 
  String strToAppend = String("APPENDED LINE: ") + String(random(300));
  if(fileToAppend.println(strToAppend)){ // append this line - if print() for same line
      Serial.println("File content was appended");
  } else {
      Serial.println("File append failed");
  }

  fileToAppend.close();
  // *********************************************
  
  File file = SPIFFS.open("/tester/test.txt");
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
