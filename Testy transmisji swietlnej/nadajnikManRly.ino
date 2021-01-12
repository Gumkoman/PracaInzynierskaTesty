 #define ledPin 3
unsigned long interval = 5000;

void setup() {
  pinMode(ledPin,OUTPUT);
}

void loop() {
  unsigned long nextTime = sendByte(255,micros());// wysylane są tutaj kolejne bajty testowe
  nextTime = sendByte(255,nextTime);
  nextTime = sendByte(255,nextTime);
  nextTime = sendByte(255,nextTime);
  nextTime = sendByte(255,nextTime);
  nextTime = sendByte(255,nextTime);
  nextTime = sendByte(255,nextTime);
  nextTime = sendByte(0,nextTime);
  nextTime = sendByte(195,nextTime);
  nextTime = sendByte(0,nextTime);
  nextTime = sendByte(10,nextTime);
  nextTime = sendByte(11,nextTime);
  nextTime = sendByte(12,nextTime);
  nextTime = sendByte(13,nextTime);
  nextTime = sendByte(14,nextTime);
  nextTime = sendByte(15,nextTime);
  nextTime = sendByte(129,nextTime);
  nextTime = sendByte(130,nextTime);
  nextTime = sendByte(0,nextTime);
  nextTime = sendByte(131,nextTime);
  nextTime = sendByte(132,nextTime);
  nextTime = sendByte(0,nextTime);

  digitalWrite(ledPin,HIGH);
  delay(10000);
}

unsigned long sendByte(uint8_t msg,unsigned long t0)
{
  bool currentBit;
  for (int i = 7;i>=0;i--)
  {
      printInfo(i,msg);
      while(true){
        if(micros() - t0 > interval){
          break;
        }  
      }
      t0 = micros();
      if(bitRead(msg,i) == 1){
        digitalWrite(ledPin,!digitalRead(ledPin));// zmiana stanu jezeli nadawana jest jedynka
      }
      printInfo(i,msg);
      while(true){
        if(micros() - t0 > interval){
          break;
        }  
      }
      t0 = micros();
      digitalWrite(ledPin,!digitalRead(ledPin));
  }
  return t0;
}
