/*
  lightWire.h - library made for light communication
  Created by Maciej Dabkowski, December 27,2020
*/
#ifndef lightWire_h
#define lightWire_h

#include "Arduino.h"

class LightReciver
{
  public:
    void init(int pin,int threashold);
    bool isDataAviable(uint8_t* buf,uint8_t& len);
    bool getBit();
    uint8_t _getByte();
    void  _waitForKey(uint8_t* buf,uint8_t& len);
    void _getMessage(uint8_t* buf, uint8_t& len);
    int _sensorPin;
    unsigned long _t0;
    unsigned long _time;
    unsigned long _t1;
    uint8_t _tempByte;
    bool _currentState;
    bool _zeroFlag;
    int _threashold;
    uint8_t _counter;
    uint16_t _msgLen;
};

class LightTransmiter
{
  public:
    void init(int pin,int _spd);
    void sendData(uint8_t *buff,uint16_t bufferLen);
  private:
    int _ledPin;
    int _speed;
    void _sendByte(uint8_t _byte);
    unsigned long _t0;
};

#endif
