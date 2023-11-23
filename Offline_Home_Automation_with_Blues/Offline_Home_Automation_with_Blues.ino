#include "DFRobot_DF2301Q.h"
#define GREEN 23
#define RED 22
#define BLUE 24
#define relay 12

int blues = 1;
//I2C communication
DFRobot_DF2301Q_I2C DF2301Q;

#include <Notecard.h>
#define usbSerial Serial

#ifndef PRODUCT_UID
#define PRODUCT_UID "com.gmail.pradeeplogu26:df_people_detecor" // "com.my-company.my-name:my-project"
#pragma message "PRODUCT_UID is not defined in this example. Please ensure your Notecard has a product identifier set before running this example or define it in code here. More details at https://dev.blues.io/tools-and-sdks/samples/product-uid"
#endif

#define myProductID PRODUCT_UID
Notecard notecard;
#define CALIBRATION_TIME   3

void Blues_Init() {

#ifdef usbSerial
  usbSerial.begin(115200);
  const size_t usb_timeout_ms = 3000;
  for (const size_t start_ms = millis(); !usbSerial && (millis() - start_ms) < usb_timeout_ms;)
    ;
  notecard.setDebugOutputStream(usbSerial);
#endif
  notecard.begin();
  J *req0 = notecard.newRequest("hub.set");
  if (myProductID[0])
  {
    JAddStringToObject(req0, "product", myProductID);
  }
  JAddStringToObject(req0, "mode", "continuous");
  notecard.sendRequestWithRetry(req0, 5); // 5 seconds
}


void setup()
{
  Serial.begin(115200);
  pinMode(GREEN, OUTPUT);
  pinMode(relay, OUTPUT);
  while ( !( DF2301Q.begin() ) ) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
  DF2301Q.setVolume(10);
  DF2301Q.setMuteMode(0);
  DF2301Q.setWakeTime(15);
  uint8_t wakeTime = 0;
  wakeTime = DF2301Q.getWakeTime();
  Serial.print("wakeTime = ");
  Serial.println(wakeTime);
  DF2301Q.playByCMDID(23);   // Common word ID

  if (blues == 1) {
    Blues_Init();
  }
}

void loop()
{
  uint8_t CMDID = 0;
  CMDID = DF2301Q.getCMDID();
  int a=CMDID;
  if (CMDID == 5) {
    digitalWrite(GREEN, LOW);  //Turn on the LED
    Serial.println("received Turn on the light");  

    if (blues == 1) {
      J *req = notecard.newRequest("note.add");
      if (req != NULL)
      {
        JAddBoolToObject(req, "sync", true);
        J *body = JAddObjectToObject(req, "body");
        if (body != NULL)
        {
          JAddStringToObject(body, "State", "received Turn on the light");
          JAddStringToObject(body, "CMID", CMDID);
        }
        notecard.sendRequest(req);
      }
    }
  }

  if (CMDID == 6) {
    digitalWrite(GREEN, HIGH);   //Turn off the LED
    Serial.println("received Turn off the light");  
    if (blues == 1) {


      J *req1 = notecard.newRequest("note.add");
      if (req1 != NULL)
      {
        JAddBoolToObject(req1, "sync", true);
        J *body = JAddObjectToObject(req1, "body");
        if (body != NULL)
        {
          JAddStringToObject(body, "State", "received Turn off the light");
          JAddStringToObject(body, "CMID", CMDID);
        }
        notecard.sendRequest(req1);
      }
    }
  }
}
