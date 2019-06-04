/*
  RemoteXY example.
  Smartphone connect via BLE on nRF51822.
  This shows an example of using the library RemoteXY.

  Download the mobile app from the
  website: http://remotexy.com/download/ for connect this sketch.

  Use the website http://remotexy.com/ to create your own management
  interface your arduino with your smartphone or tablet.
  You can create different management interfaces. Use buttons,
  switches, sliders, joysticks (g-sensor) all colors and sizes
  in its interface. Next, you will be able to get the sample
  code for arduino to use your interface for control from a
  smartphone or tablet. You will not need to re-install the
  android app, as it will determine which interface you have
  downloaded the arduino.

  This example uses the library arduino-BLEPeripheral to use BLE on nRF51822 chips. 
    source: https://github.com/sandeepmistry/arduino-BLEPeripheral
  
*/

/////////////////////////////////////////////
//        RemoteXY include library         //
/////////////////////////////////////////////

// #define REMOTEXY__DEBUGLOGS Serial

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__NRF_BLE
#include <RemoteXY.h>

// RemoteXY connection settings
#define REMOTEXY_BT_DEVICENAME "RemoteXY"

// RemoteXY configurate
unsigned char RemoteXY_CONF[] =
  { 1,0,11,0,1,5,1,0,21,2
  ,59,59,2,88,0 }; 

// this structure defines all the variables of your control interface
struct {

  // input variable
  unsigned char button_1; // =1 if button pressed, else =0

  // other variable
  unsigned char connect_flag;  // =1 if wire connected, else =0

} RemoteXY;

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_P2 2

void setup()
{
#ifdef REMOTEXY__DEBUGLOGS
  REMOTEXY__DEBUGLOGS.begin(115200);
  REMOTEXY__DEBUGLOGS.println("RemoteXY nRF test program");
#endif

  RemoteXY_Init ();

  pinMode (PIN_P2, OUTPUT);
}

void loop()
{
  RemoteXY_Handler ();

  digitalWrite(PIN_P2, (RemoteXY.button_1 == 0) ? LOW : HIGH);

}
