#define sensorPin A0
#define key 195

uint8_t data_buffer[300];
uint8_t byte_buffer;
uint8_t byte_buffer_1;
bool isSync = false;
bool isKeyDetected = false;
bool currentState;
uint16_t threashold = 500;
int counter;

unsigned long t0 = 0;
unsigned long t1 = 0;
unsigned long time_lo = 0;
unsigned long time_mid = 0;
unsigned long time_high = 0;
unsigned long time_temp = 0;

bool zeroFlag = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Odbiornik");
}

void loop() {
  mainLoop();
}


void mainLoop() {
  detectStartingSequence();
}

void detectStartingSequence() {
  currentState = getBit();
  t0 = micros();
  counter = 0;
  while (!isSync) {
    if (getBit() != currentState) {
      time_temp = micros() - t0;
      t0 = micros();
      Serial.println("Zmiana bitu");
      currentState = !currentState;
      if (time_temp > time_lo && time_temp < time_mid) {
        counter++;
        Serial.print("Zwiekszenie licznika");
        Serial.println(time_temp);
      } else {
        counter = 0;
        time_lo   = time_temp * 0.5;
        t1 =   time_temp;
        time_mid  = time_temp * 1.3;
        time_high = time_temp * 2.5;
        Serial.println("Ustalenie czasu time i wynikajacych z niego czasow");
      }
      if (counter  > 12) {
        isSync = true;
        Serial.println("Synchronizacja");
        break;
      }
    }
  }
  detectKey();
}

void detectKey() {
  byte_buffer = 0;
  Serial.print("time_lo: ");
  Serial.println(time_lo);
  Serial.print("time_mid: ");
  Serial.println(time_mid);
  Serial.print("time_high: ");
  Serial.println(time_high);
  Serial.print("t1: ");
  Serial.println(t1);
  while (!isKeyDetected) {
    if (getBit() != currentState) {
      time_temp = micros() - t0;
      Serial.print("wykrycie czasu ") ;
      Serial.println(zeroFlag) ;
      currentState = !currentState;
      if (time_temp > time_mid && time_temp < time_high) {
        t0 = micros();
        currentState = !currentState;
        bitWrite(byte_buffer, 0, 0);
        Serial.print("Zero :");
        Serial.print(time_temp);
        Serial.print(" ");
        Serial.print(byte_buffer);
        Serial.print(" ");
        Serial.println(byte_buffer, BIN);
        if (byte_buffer == 195 ) {
          Serial.println("LUBIE PLACKI");
          readLightProtoHeader();
        }
        byte_buffer = byte_buffer << 1;
        zeroFlag = false;
      } else if (time_temp > time_lo && time_temp < time_mid) {
        if (zeroFlag == true) {
          t0 = micros();
          currentState = !currentState;
          bitWrite(byte_buffer, 0, 1);
          Serial.print("Jeden :");
          Serial.print(time_temp);
          Serial.print(" ");
          Serial.print(byte_buffer);
          Serial.print(" ");
          Serial.println(byte_buffer, BIN);
          if (byte_buffer == 195 ) {
            Serial.println("Wykryto Klucz");
            readLightProtoHeader();
          }
          byte_buffer = byte_buffer << 1;
          zeroFlag = false;
        } else {
          Serial.print("Zmiana flagi przy wykryciu Jeden ");
          t0 = micros();
          currentState = !currentState;
          zeroFlag = true;
        }


      }

    }
  }

}

void readLightProtoHeader() {
  Serial.println("Nagłówek protokolu swietlnego");
  uint16_t msgLenght = 0;
  msgLenght = readByte()<<8;
  msgLenght = msgLenght | readByte();
  Serial.print("Wiadomosc o dlugosci: ");
  Serial.println(msgLenght);
}


uint8_t readByte() {
  byte_buffer = 0;
  int i = 7;
  while (true) {
    if (getBit() != currentState) {
      time_temp = micros() - t0;
      currentState = !currentState;
      if (time_temp > time_mid && time_temp < time_high) {
        t0 = micros();
        currentState = !currentState;
        i--;
        if (i == -1) {
          return byte_buffer;
        }

        zeroFlag = false;
      } else if (time_temp > time_lo && time_temp < time_mid) {
          
        if (zeroFlag == true) {
          t0 = micros();
          currentState = !currentState;
          bitWrite(byte_buffer, i, 1);
        
          i--;
          if (i == -1) {
            return byte_buffer;
          }
          zeroFlag = false;
        } else {
          t0 = micros();
          currentState = !currentState;
          zeroFlag = true;
        }


      }
    }
  }
}


bool getBit() {
  if ( analogRead(sensorPin)  > threashold ) {
    return true;
  }
  else {
    return false;
  }
}
