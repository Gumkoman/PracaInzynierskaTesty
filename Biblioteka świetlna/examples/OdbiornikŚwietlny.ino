#include "LightWire.h"
#define MAX_LEN 100
LightTransmiter lt;
LightReciver lr;
uint8_t buf[MAX_LEN];
uint8_t buflen = MAX_LEN;
void setup() {
  
  Serial.begin(115200);
  lr.init(A0, 50);

  Serial.println("Testy Biblioteki");

}

void loop() {
  if (lr.isDataAviable(buf, buflen)) {
    Serial.println("ODBIORNIK");
    Serial.print("Otrzymano wiadomosc o dlugosci: ");
    Serial.println(buflen);
    Serial.println("Zawierajaca informacje:");
    for (int i = 0; i < buflen; i++) {
      Serial.print(buf[i]);
      Serial.print(" ");
    }
  }
}
