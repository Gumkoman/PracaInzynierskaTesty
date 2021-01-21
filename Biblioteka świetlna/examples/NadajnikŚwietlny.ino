#include "LightWire.h"
#define MAX_LEN 100
LightTransmiter lt;
LightReciver lr;
uint8_t buf[MAX_LEN];
uint8_t buflen = MAX_LEN;
void setup() {
  
  lt.init(4, 20000);
  uint8_t myPins[] = {137, 0, 0, 5, 0};
  lt.sendData(myPins, 5);
  digitalWrite(4,HIGH);
  Serial.println("Testy Biblioteki");

}


void setup() {
  //Serial.begin(115200);
  lt.init(4, 500);
  uint8_t myPins[] = {137, 0, 0, 5, 0};
  lt.sendData(myPins, 5);
 digitalWrite(4,HIGH);
}

void loop() {

}
