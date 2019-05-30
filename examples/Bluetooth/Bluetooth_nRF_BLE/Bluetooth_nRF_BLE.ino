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

*/

/////////////////////////////////////////////
//        RemoteXY include library         //
/////////////////////////////////////////////


#define REMOTEXY__DEBUGLOGS Serial

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__NRF_BLE
#include <RemoteXY.h>

// RemoteXY connection settings
#define REMOTEXY_BT_DEVICENAME "RemoteXY"

// RemoteXY configurate
unsigned char RemoteXY_CONF[] =
  { 255,1,0,0,0,13,0,8,13,0,
  1,0,38,12,12,12,2,31,88,0 }; 

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

#define LED_BUILTIN 2

void setup()
{
#ifdef REMOTEXY__DEBUGLOGS
  REMOTEXY__DEBUGLOGS.begin(115200);
  REMOTEXY__DEBUGLOGS.println("RemoteXY nRF test program");
#endif


  RemoteXY_Init ();

  pinMode (LED_BUILTIN, OUTPUT);

}

void loop()
{
  RemoteXY_Handler ();

  digitalWrite(LED_BUILTIN, (RemoteXY.button_1 == 0) ? LOW : HIGH);

}
