///////////////////////////////////////////// 
//        RemoteXY include library         // 
///////////////////////////////////////////// 

/* RemoteXY select connection mode and include library */ 

#define REMOTEXY__DEBUGLOGS Serial

#define REMOTEXY_MODE__ESP32_BLE 
#include <RemoteXY.h>

/* RemoteXY connection settings */ 
#define REMOTEXY_BT_DEVICENAME "RemoteXY"

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

} RemoteXY; 

///////////////////////////////////////////// 
//           END RemoteXY include          // 
///////////////////////////////////////////// 

#define LED_BUILTIN 2

void setup()  
{ 
  RemoteXY_Init ();  
   
  pinMode (LED_BUILTIN, OUTPUT);
   
} 

void loop()  
{  
  RemoteXY_Handler (); 
   
  digitalWrite(LED_BUILTIN, (RemoteXY.button_1==0)?LOW:HIGH);

}

