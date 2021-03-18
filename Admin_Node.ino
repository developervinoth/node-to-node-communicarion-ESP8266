

#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#include "painlessMesh.h"
#include <ArduinoJson.h>




// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555



//Pin Declaration
//#define Button1 D6
//#define Button2 D3

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;


// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() 
{
  

  DynamicJsonDocument doc(1024);
  doc["ADMIN_STATUS"] = "CONNECTED";
  String msg ;
  serializeJson(doc, msg);
  // msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  Serial.println(msg);
  taskSendMessage.setInterval((TASK_SECOND * 1));
}
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {

  String json;
  DynamicJsonDocument doc(1024);
  json = msg.c_str();
  DeserializationError error = deserializeJson(doc, json);

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }

 String Temp_Node_One = doc["TEMP_NODE_1"];
String Lati_Node_One = doc["LAT_NODE_1"]; 
String Longi_Node_One = doc["LONGI_NODE_1"]; 
String Beat_Node_One = doc["BEAT_NODE_1"]; 
   
String Temp_Node_Two = doc["TEMP_NODE_2"];
String Lati_Node_Two = doc["LAT_NODE_2"]; 
String Longi_Node_Two = doc["LONGI_NODE_2"]; 
String Beat_Node_Two = doc["BEAT_NODE_2"]; 



    u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_profont17_mr);
  

  u8g2.drawStr(0,11,"Node One");
     u8g2.drawStr(0,27,"Data");
     delay(1000);
      u8g2.clearBuffer();  
u8g2.drawStr(0,11,"T1: ");
 u8g2.setCursor(27,11);
   u8g2.print(Temp_Node_One);

   
   u8g2.drawStr(0,27,"La1: ");
  u8g2.setCursor(35,27);
   u8g2.print(Lati_Node_One);

   
      u8g2.drawStr(0,43,"Lo1: ");
   u8g2.setCursor(35,43);
   u8g2.print(Longi_Node_One);
   
         u8g2.drawStr(0,59,"B1: ");
   u8g2.setCursor(27,59);
   u8g2.print(Beat_Node_One);

  u8g2.sendBuffer(); 

    delay(1000);
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_profont17_mr);


  u8g2.drawStr(0,11,"Node Two");
     u8g2.drawStr(0,27,"Data");
     delay(1000);
      u8g2.clearBuffer();  
u8g2.drawStr(0,11,"T2: ");
 u8g2.setCursor(27,11);
   u8g2.print(Temp_Node_Two);

   
   u8g2.drawStr(0,27,"La2: ");
  u8g2.setCursor(35,27);
   u8g2.print(Lati_Node_Two);

   
      u8g2.drawStr(0,43,"Lo2: ");
   u8g2.setCursor(35,43);
   u8g2.print(Longi_Node_Two);
   
         u8g2.drawStr(0,59,"B2: ");
   u8g2.setCursor(27,59);
   u8g2.print(Beat_Node_Two);
   
  u8g2.sendBuffer();      
   if (Temp_Node_One !="null"){
    Serial.println("Node one Data");
   Serial.println(Temp_Node_One);
   }

      if (Lati_Node_One !="null"){
   Serial.println(Lati_Node_One);
      }
      
      if (Longi_Node_One !="null"){
   Serial.println(Longi_Node_One);
      }
           if (Beat_Node_One !="null"){
   Serial.println(Beat_Node_One);
      }
      
   if (Temp_Node_Two !="null"){
   Serial.println("Node Two Data");
   Serial.println(Temp_Node_Two);
   }
      if (Lati_Node_Two !="null"){
   Serial.println(Lati_Node_Two);
      }
     if (Longi_Node_Two !="null"){
   Serial.println(Longi_Node_Two);
     }
     
     if (Beat_Node_Two !="null"){
   Serial.println(Beat_Node_Two);
     }
     
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  u8g2.begin();
Serial.begin(115200);
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
    mesh.update();
 

}
