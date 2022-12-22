// Import required libraries
#include <ESP8266mDNS.h>
#include <LEAmDNS.h>
#include <LEAmDNS_lwIPdefs.h>
#include <LEAmDNS_Priv.h>

#include "Wifi.h"
#include "Websocket.h"

// Replace with your network credentials
#ifndef STASSID
//#define ssid        "esp1234"
//#define password    "azqs12/d"
#define ssid          "SNUFA"
#define password      "guigui59"
#endif

#define   rxPin       13
#define   txPin       15


Websocket   *ws       ;
fingerPrint *finger   ;


void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // ****************************************
  // We start by connecting to a WiFi network
  // ****************************************
  Serial.print("Connecting to ")            ;
  Serial.println(ssid)                      ;
  Wifi    *wifi = new Wifi(ssid,password)   ;
  while(!wifi->WaitingSTAConnection())
    Serial.print(".")   ;
  
  Serial.println("WiFi connected")          ;
  Serial.print("IP address:")               ;
  Serial.println(wifi->local_IP())          ;

  // ****************************************
  // We start DNS service Initialisation
  // ****************************************
// if ( MDNS.begin("myesp")) {  //Start mDNS with name myesp
 //     Serial.println("MDNS started");
//  }
  // ****************************************
  // We create the fingerPrint Object
  // ****************************************
  finger = new fingerPrint(rxPin,txPin)           ;


  // ****************************************
  // We start the WebSocket
  // ****************************************
  //led = new   Led(ledPin,ledState)              ;
  
}

void loop() {

  ws->loop()                                      ;
  //MDNS.update()                                 ;
}
