/*
  RemoteXY example. 
  Smartphone connect over Wi-Fi access point from ESP32 

  This shows an example of using the library RemoteXY.
  In the example you can control an LED e.g. on ESP_IO32 using the button on the 
  smartphone.
  
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

/* RemoteXY select connection mode and include library */ 
#define REMOTEXY_MODE__ESP32WIFI_LIB_POINT
#define REMOTEXY_CHANGED_FLAG
#include <WiFi.h>
#include <RemoteXY.h> 

/* RemoteXY connection settings */ 

#define REMOTEXY_WIFI_SSID "RemoteXY" 
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

/* RemoteXY configurate  */ 
unsigned char RemoteXY_CONF[] = 
  { 1,0,11,0,1,5,1,0,21,2
  ,59,59,2,88,0 }; 
   
/* this structure defines all the variables of your control interface */ 
struct { 

    /* input variable */
  unsigned char button_1; /* =1 if button pressed, else =0 */

    /* other variable */
  unsigned char connect_flag;  /* =1 if wire connected, else =0 */
#ifdef REMOTEXY_CHANGED_FLAG
  unsigned char changed_flag;  /* =1 if any variable has changed, else =0 */
#endif  
} RemoteXY; 

///////////////////////////////////////////// 
//           END RemoteXY include          // 
///////////////////////////////////////////// 

#define PIN_BUTTON_1 32


void setup()  
{ 
  RemoteXY_Init ();  
   
  pinMode (PIN_BUTTON_1, OUTPUT);
   

  // TODO your setup code 
   
} 

void loop()  
{  
  RemoteXY_Handler (); 

#ifdef REMOTEXY_CHANGE_FLAG  
  if (RemoteXY.changed_flag)
  { 
    // This code is only executed if one or more of the RemoteXY variables have been changed  
	RemoteXY.changed_flag = 0;	// Clear the flag ready for the next time the RemoteXY_Handler is called.
    digitalWrite(PIN_BUTTON_1, (RemoteXY.button_1==0)?LOW:HIGH);
  }
#else
  digitalWrite(PIN_BUTTON_1, (RemoteXY.button_1==0)?LOW:HIGH);
#endif

  // TODO your loop code 
  // use the RemoteXY structure for data transfer 

}