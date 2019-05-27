#ifndef _REMOTEXY_MOD_ESP32_BLE_H_
#define _REMOTEXY_MOD_ESP32_BLE_H_

#include "classes/RemoteXY_API.h"

#define REMOTEXY_SEND_BUFFER_LENGTH 20

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define SERVICE_UUID             "0000FFE0-0000-1000-8000-00805F9B34FB" // UART service UUID
#define CHARACTERISTIC_UUID_RXTX "0000FFE1-0000-1000-8000-00805F9B34FB"

class CRemoteXY : public CRemoteXY_API, BLEServerCallbacks, BLECharacteristicCallbacks   {

  public:

    CRemoteXY (const void * _conf, void * _var, const char * _accessPassword, const char * _bleDeviceName) {
      init (_conf, _var, _accessPassword);
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.println("init BLE Module");
#endif

      this->sendBytesAvailable = 0;
      this->sendBufferCount = 0;

      this->currentValueIndex = 0;
	    
      // Create the BLE Device
      BLEDevice::init(_bleDeviceName);

      // Create the BLE Server
      pServer = BLEDevice::createServer();
      pServer->setCallbacks(this);

      // Create the BLE Service
      BLEService *pService = pServer->createService(SERVICE_UUID);

      // Create a BLE Characteristic
      pRxTxCharacteristic = pService->createCharacteristic(
                              CHARACTERISTIC_UUID_RXTX,
                              BLECharacteristic::PROPERTY_READ |
                              BLECharacteristic::PROPERTY_NOTIFY |
                              BLECharacteristic::PROPERTY_WRITE_NR 
                            );

      BLE2902 *ble2902 = new BLE2902();
      ble2902->setNotifications(true);
      pRxTxCharacteristic->addDescriptor(ble2902);
      pRxTxCharacteristic->setCallbacks(this);

      // Start the service
      pService->start();

      // Start advertising
      pServer->getAdvertising()->addServiceUUID(pService->getUUID());
      pServer->getAdvertising()->start();

#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.println("Waiting a client connection to notify...");
#endif
    }

    void onConnect(BLEServer* pServer) {
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.println("onConnect");
#endif
    };

    void onDisconnect(BLEServer* pServer) {
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.println("onDisconnect");
#endif
    }

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print("onWrite: ");
        for (int i = 0; i < rxValue.length(); i++)
        {
          REMOTEXY__DEBUGLOGS.print((uint8_t)rxValue[i], HEX);
          REMOTEXY__DEBUGLOGS.print(" ");
        }
        REMOTEXY__DEBUGLOGS.println();
        REMOTEXY__DEBUGLOGS.flush();
#endif
        currentValue += rxValue;
      }
    }

  protected:
    uint8_t sendBuffer[REMOTEXY_SEND_BUFFER_LENGTH];
    uint16_t sendBufferCount;
    uint16_t sendBytesAvailable;

    void sendStart (uint16_t len) {
      sendBytesAvailable = len;
      sendBufferCount = 0;
    }

    void sendByte (uint8_t b) {
      sendBuffer[sendBufferCount++] = b;
      sendBytesAvailable--;
      if ((sendBufferCount == REMOTEXY_SEND_BUFFER_LENGTH) || (sendBytesAvailable == 0)) {
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print("Sendbyte:");
        for (uint16_t i = 0; i < sendBufferCount; i++)
        {
          REMOTEXY__DEBUGLOGS.print(" ");
          REMOTEXY__DEBUGLOGS.print(sendBuffer[i], HEX);
        }
        REMOTEXY__DEBUGLOGS.println();
        REMOTEXY__DEBUGLOGS.flush();
#endif
        pRxTxCharacteristic->setValue((uint8_t *)sendBuffer, sendBufferCount);
        pRxTxCharacteristic->notify();
        sendBufferCount = 0;
      }

    }

    uint8_t receiveByte () {
      uint8_t b = currentValue[currentValueIndex++];
      if (currentValueIndex == currentValue.length())
      {
        currentValue.clear();
        currentValueIndex = 0;
      }
      return b;
    }


    uint8_t availableByte () {
      return currentValue.length();

    };

  private:
    BLEServer *pServer = NULL;
    BLECharacteristic * pRxTxCharacteristic = NULL;
    std::string currentValue;
    int currentValueIndex;
};


#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY,REMOTEXY_ACCESS_PASSWORD, REMOTEXY_BT_DEVICENAME)

#endif //_REMOTEXY_MOD_ESP32_BLE_H_
