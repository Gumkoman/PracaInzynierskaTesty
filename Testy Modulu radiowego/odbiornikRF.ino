#include "VirtualWire.h"
 
#define led_pin 13
#define receive_pin 11
 
 
void setup()
{
    Serial.begin(9600);
    Serial.println("setup");
  // Przygotowanie danych potrzebnych dla biblioteki
    vw_set_rx_pin(receive_pin);// ustalenie pinu do ktorego podlaczone jest zlacze data
    vw_setup(2000);   // Ustalenie predkosci
    vw_rx_start(); // Ustalenie urzadzenia jako odbiornika 
 
    pinMode(led_pin, OUTPUT);
}
 
void loop()
{
  // częśc wymagana do poprawnego działania biblioteki
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
 
    if (vw_get_message(buf, &buflen)) // jezeli odczytane sa dane
    {
  int i;
  Serial.println("Got: ");
  
  for (i = 0; i < buflen; i++)
  {
      Serial.print(buf[i], HEX); // Wypisywanie kolejno odebranych bajtow danych
      Serial.print(' ');
  }
  Serial.println();

    }
}
