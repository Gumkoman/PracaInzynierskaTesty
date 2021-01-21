#include "Arduino.h"
#include "LightWire.h"

#define MAX_BIT_TRANSMISION_TIME 100000

void LightReciver::init(int pin, int threashold)
{
  pinMode(pin, INPUT);
  _sensorPin = pin;
  _threashold = threashold;
  _currentState = getBit();

}
bool LightReciver::isDataAviable(uint8_t* buf, uint8_t& len) {
  _t0 = micros();
  _counter = 0;
  len = 10;
  while (micros() - _t0 < MAX_BIT_TRANSMISION_TIME) {
    if (getBit() != _currentState) {
      _t1 = micros() - _t0;
      _t0 = micros();
      _currentState = !_currentState;
      if (_time * 0.5 < _t1 && _time * 1.5 > _t1) {
        _counter++;
      } else {
        _time = _t1;
        _counter = 0;
      }
      if (_counter > 10) {
        _waitForKey(buf, len);
        return true;
      }
    }
  }
  return false;
}

void LightReciver::_getMessage(uint8_t* buf, uint8_t& len) {
  _msgLen = 0;
  uint8_t msgLen1 = _getByte();
  uint8_t msgLen2 = _getByte();
  _msgLen = ((uint16_t)msgLen1 << 8) | msgLen2;
  for (int j = 0; j < _msgLen; j++) {
    buf[j] = _getByte();
  }
  len = _msgLen;
  
}

uint8_t LightReciver::_getByte() {
  int i = 7;
  _tempByte = 0;
  while (i >= 0) {
    if (getBit() != _currentState) {
      _t1 = micros() - _t0;
      _t0 = micros();
      _currentState = !_currentState;
      if (_time * 0.5 < _t1 && _time * 1.5 > _t1) {
        if (_zeroFlag) {
          bitWrite(_tempByte, i, 1);
          i--;
          _zeroFlag = false;
        } else {
          _zeroFlag = true;
        }
      } else if (_time * 1.5 < _t1 && _time * 2.5 > _t1) {
        bitWrite(_tempByte, i, 0);
        _zeroFlag = false;
        i--;
      }
    }
  }
  return _tempByte;
}


void LightReciver::_waitForKey(uint8_t* buf, uint8_t& len) {
  _tempByte = 0;
  while (true) {
    if (getBit() != _currentState) {
      _t1 = micros() - _t0;
      _t0 = micros();
      _currentState = !_currentState;
      if (_time * 0.5 < _t1 && _time * 1.5 > _t1) {
        if (_zeroFlag) {
          bitWrite(_tempByte, 0, 1);
          if (_tempByte == 195) {
            _zeroFlag = false;
            _getMessage(buf, len);
            break;
          }
          _tempByte = _tempByte << 1;
          _zeroFlag = false;
        } else {
          _zeroFlag = true;
        }
      } else if (_time * 1.5 < _t1 && _time * 2.5 > _t1) {
        bitWrite(_tempByte, 0, 0);
        if (_tempByte == 195) {
          _getMessage(buf, len);
          break;
        }
        _tempByte = _tempByte << 1;
      }
    }
  }
}

bool LightReciver::getBit() {
  if (analogRead(_sensorPin) > _threashold) {
    return true;
  } else {
    return false;
  }
}


void LightTransmiter::init(int pin, int _spd)
{
  pinMode(pin, OUTPUT);
  _ledPin = pin;
  _speed = _spd;
  digitalWrite(_ledPin, HIGH);
}


void LightTransmiter::sendData(uint8_t *buff, uint16_t bufferLen)
{
  _t0 = micros();
  //sending header
  _sendByte(255);          // sending 1111.. to detect t and 2*t
  _sendByte(255);          // for diffrential manchaster coding
  _sendByte(255);
  _sendByte(255);
  _sendByte(0);            // clearing buffor
  _sendByte(195);          //key
  _sendByte((uint8_t)(bufferLen >> 8));
  _sendByte((uint8_t)bufferLen);
  for (int i = 0; i < bufferLen; i++) {
    _sendByte(buff[i]);      // seding data
  }
}
void LightTransmiter::_sendByte(uint8_t msg)
{
  bool currentBit;
  for (int i = 7; i >= 0; i--)
  {

    if (bitRead(msg, i) == 1) {
      digitalWrite(_ledPin, !digitalRead(_ledPin));
    }


    while (micros() - _t0 < _speed / 2) {

    }


    digitalWrite(_ledPin, !digitalRead(_ledPin));
    while (micros() - _t0 < _speed) {

    }

    _t0 = micros();
  }
}
