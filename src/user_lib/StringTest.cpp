#include "Arduino.h"

void strimg_setup() {
    String str = "yaniv";
    String str2 = "cohen";
    str = str2 + String(" : ") + str; // concatenate strings
    Serial.println(str);// cohen : yaniv
    int index = str.indexOf("ohen");
    Serial.println(index); // 1
    Serial.println(str.indexOf("1")); // -1
    if (index != -1) { // find in str
        String str3 = str.substring(index, str.length()); // sub srtring
        Serial.println(str3); // ohen : yaniv
    }
    if (str2 == "cohen") { // conpare str
        Serial.println(str2); // cohen
    }
    const char *s = str.c_str(); // str to char*
    Serial.println(String(s)); // cohen : yaniv
    int i = 1;
    String str4 = String(i); // int to str
    int i1 = str4.toInt(); // str to int
    Serial.println(i1); // 1
    char *myStringsArry[] = {"This is string 1", "This is string 2", "This is string 3",
                     "This is string 4", "This is string 5", "This is string 6"
                    };
    Serial.println("dist Arry:");
    int size = sizeof(myStringsArry)/sizeof(myStringsArry[0]);
    Serial.println(size); // 6
    for(int i = 0; i < size; i++)
    {
      Serial.println(String(myStringsArry[i]));// 6 lines
    }
    // const char s[] PROGMEM = "Progmem"; // PROGMEM meens write to flash and to read it: (class __FlashStringHelper *)s
}
