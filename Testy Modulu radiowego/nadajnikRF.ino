#include "VirtualWire.h"
 
// Definicja pinow
#define led_pin 11
#define transmit_pin 10
 
void setup()
{
  // Przygotowanie danych potrzebnych dla biblioteki
    vw_set_tx_pin(transmit_pin);// ustalenie pinu do ktorego podlaczone jest zlacze data
    vw_setup(2000); // ustalenie predkosci
}
 
void loop()
{
  
  String toSend = ("hello world"); // tekst wiadomości
  char msg[23]; // stworzenie tablicy bajtow
  toSend.toCharArray(msg, toSend.length() + 1); // konwersja tekstu do tablicy bajtow
  
  vw_send((uint8_t *)msg, strlen(msg));// wysylanie wiadomosci 
  vw_wait_tx(); 
  delay(1000); // odczekanie czasu
  
}
