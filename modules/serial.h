#ifndef _REMOTEXY_MOD_SERIAL_H_
#define _REMOTEXY_MOD_SERIAL_H_

#include "classes/RemoteXY_Serial.h"

#ifdef REMOTEXY_PORT__ESP32_BT
#define REMOTEXY_SEND_BUFFER_LENGTH 200
#endif

class CRemoteXY : public CRemoteXY_Serial {

  public:

#if defined(REMOTEXY_PORT__HARDSERIAL)
    CRemoteXY (const void * _conf, void * _var, const char * _accessPassword, HardwareSerial * _serial, long _serialSpeed) {
      initSerial (_serial, _serialSpeed);
      init (_conf, _var, _accessPassword);
    }
#elif defined(REMOTEXY_PORT__SOFTSERIAL)
    CRemoteXY (const void * _conf, void * _var, const char * _accessPassword, uint8_t _serialRx, uint8_t _serialTx, long _serialSpeed) {
      initSerial (_serialRx, _serialTx, _serialSpeed);
      init (_conf, _var, _accessPassword);
    }
#elif defined(REMOTEXY_PORT__ESP32_BT)
    CRemoteXY (const void * _conf, void * _var, const char * _accessPassword, const char * _btDeviceName) {
      initSerial (_btDeviceName);
      init (_conf, _var, _accessPassword);
    }
#endif

  protected:

#ifdef REMOTEXY_PORT__ESP32_BT
    uint8_t sendBuffer[REMOTEXY_SEND_BUFFER_LENGTH];
    uint16_t sendBufferCount;
    uint16_t sendBytesAvailable;


    void sendStart (uint16_t len) {
      sendBytesAvailable = len;
      sendBufferCount = 0;
    }
#endif

#ifdef REMOTEXY_PORT__ESP32_BT
    void sendByte (uint8_t b) {
      sendBuffer[sendBufferCount++] = b;
      sendBytesAvailable--;
      if ((sendBufferCount == REMOTEXY_SEND_BUFFER_LENGTH) || (sendBytesAvailable == 0)) {
        uint8_t buf[sendBufferCount];

#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print("[");
#endif

        for (uint16_t i = 0; i < sendBufferCount; i++)
        {
          buf[i] = sendBuffer[i];
#if defined(REMOTEXY__DEBUGLOGS)
          REMOTEXY__DEBUGLOGS.print(" ");
          REMOTEXY__DEBUGLOGS.print(buf[i], HEX);
#endif
        }
        serial->write ((uint8_t *)buf, sendBufferCount);
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print(" ] ");
#endif
        sendBufferCount = 0;
      }

    }
#else

    void sendByte (uint8_t b) {
      serial->write (b);
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.print(b, HEX);
      REMOTEXY__DEBUGLOGS.print(' ');
#endif
    }
#endif

    uint8_t receiveByte () {
      uint8_t b = serial->read ();
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.print(b, HEX);
      REMOTEXY__DEBUGLOGS.print(' ');
#endif
      return b;
    }


    uint8_t availableByte () {
      return serial->available ();
    };


};


#if defined(REMOTEXY_PORT__HARDSERIAL)
#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, REMOTEXY_ACCESS_PASSWORD, &REMOTEXY_SERIAL, REMOTEXY_SERIAL_SPEED)
#elif defined(REMOTEXY_PORT__SOFTSERIAL)
#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, REMOTEXY_ACCESS_PASSWORD, REMOTEXY_SERIAL_RX, REMOTEXY_SERIAL_TX, REMOTEXY_SERIAL_SPEED)
#elif defined(REMOTEXY_PORT__ESP32_BT)
#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, REMOTEXY_ACCESS_PASSWORD,  REMOTEXY_BT_DEVICENAME)
#endif

#endif //_REMOTEXY_MOD_SERIAL_H_
