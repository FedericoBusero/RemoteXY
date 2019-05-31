#ifndef _REMOTEXY_MOD_NRF_BLE_H_
#define _REMOTEXY_MOD_NRF_BLE_H_

#include "classes/RemoteXY_API.h"

#define REMOTEXY_SEND_BUFFER_LENGTH     20
#define REMOTEXY_RECEIVE_BUFFER_LENGTH 512

#define NRF_51822_DEBUG

#include <BLEPeripheral.h>

#define SERVICE_UUID             "ffe0" 
#define CHARACTERISTIC_UUID_RXTX "ffe1"


void CRemoteXY_onConnect(BLECentral& central);
void CRemoteXY_onDisconnect(BLECentral& central);

class CRemoteXY : public CRemoteXY_API, public BLEPeripheral {

  public:

    CRemoteXY (const void * _conf, void * _var, const char * _accessPassword, const char * _bleDeviceName) :
	  BLEPeripheral(0, 0, 0)
	{
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.println("nRF initModule");
#endif
      init (_conf, _var, _accessPassword);

	  this->sendBytesAvailable = 0;
      this->sendBufferCount = 0;
	  
      this->_rxHead = this->_rxTail = 0;

      CRemoteXY::_instance = this;

      this->setLocalName(_bleDeviceName);
      this->setDeviceName(_bleDeviceName);
      this->setAppearance(0x0080);

      addAttribute(this->pService);
      setAdvertisedServiceUuid(this->pService.uuid());
      addAttribute(this->pRxTxCharacteristic);
	  
      this->pRxTxCharacteristic.setEventHandler(BLEWritten, CRemoteXY::onWrite);
      
      this->setEventHandler(BLEConnected, CRemoteXY_onConnect);
      this->setEventHandler(BLEDisconnected, CRemoteXY_onDisconnect);

      BLEPeripheral::begin();
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.println("Waiting a client connection to notify...");
#endif
    }

    void onConnect() {
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.println("onConnect");
#endif
    };

    void onDisconnect() {
#if defined(REMOTEXY__DEBUGLOGS)
      REMOTEXY__DEBUGLOGS.println("onDisconnect");
#endif
    }

    void onWrite(const uint8_t* data, size_t size) {
      if (size > 0) {
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print("onWrite: ");
        for (size_t i = 0; i < size; i++)
        {
          REMOTEXY__DEBUGLOGS.print((uint8_t)data[i], HEX);
          REMOTEXY__DEBUGLOGS.print(" ");
        }
        REMOTEXY__DEBUGLOGS.println();
#endif
        for (size_t i = 0; i < size; i++) {
          this->_rxHead = (this->_rxHead + 1) % sizeof(this->_rxBuffer);
          this->_rxBuffer[this->_rxHead] = data[i];
        }      
	  }
    }

  protected:
    uint8_t sendBuffer[REMOTEXY_SEND_BUFFER_LENGTH];
    uint16_t sendBufferCount;
    uint16_t sendBytesAvailable;
	
    size_t _rxHead;
    size_t _rxTail;
    uint8_t _rxBuffer[REMOTEXY_RECEIVE_BUFFER_LENGTH];
	
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
#endif
		BLEPeripheral::poll();
		
		if (this->pRxTxCharacteristic.subscribed() == false) 
		{
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.println("tx characteristic not subscribed");
#endif
		//	return;
		}
		
        bool ret;
		ret = pRxTxCharacteristic.setValue((uint8_t *)sendBuffer, sendBufferCount);
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print("setValue: ");
        REMOTEXY__DEBUGLOGS.println(ret);
#endif		
		BLEPeripheral::poll();
        sendBufferCount = 0;
      }
    }

    uint8_t receiveByte () {
      BLEPeripheral::poll();
      if (this->_rxTail == this->_rxHead) return -1;
      this->_rxTail = (this->_rxTail + 1) % sizeof(this->_rxBuffer);
      uint8_t byte = this->_rxBuffer[this->_rxTail];

      return byte;
    }

    uint8_t availableByte () {
      BLEPeripheral::poll();
      int retval = (this->_rxHead - this->_rxTail + sizeof(this->_rxBuffer)) % sizeof(this->_rxBuffer);

      return retval;
    };

    static void onWrite(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic)
	{
      CRemoteXY::_instance->onWrite(rxCharacteristic.value(), rxCharacteristic.valueLength());
	}


  private:
    BLEService pService = BLEService(SERVICE_UUID);
    BLECharacteristic    pRxTxCharacteristic  = BLECharacteristic(CHARACTERISTIC_UUID_RXTX, BLERead|BLEWriteWithoutResponse|BLENotify, BLE_ATTRIBUTE_MAX_VALUE_LENGTH );

	public:
	    static CRemoteXY* _instance;

	};

CRemoteXY* CRemoteXY::_instance = NULL;

void CRemoteXY_onConnect(BLECentral& central) {
  CRemoteXY::_instance->onConnect();
}

void CRemoteXY_onDisconnect(BLECentral& central) {
  CRemoteXY::_instance->onDisconnect();
}

#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY,REMOTEXY_ACCESS_PASSWORD, REMOTEXY_BT_DEVICENAME)

#endif //_REMOTEXY_MOD_NRF_BLE_H_
