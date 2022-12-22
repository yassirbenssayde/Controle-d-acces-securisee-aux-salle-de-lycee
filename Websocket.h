#ifndef WEBSOCKET_H_
#define WEBSOCKET_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "CFingerPrint.h"

#include <WebSocketsServer.h>

class Websocket{
 private :
  // Create AsyncWebServer object on port 81
  static      WebSocketsServer     *ws            ;
  static      fingerPrint          *finger        ;
 
 public :
                Websocket(fingerPrint* finger ,int port=81) ; // to let port 80 for Http server for instance
  static  void  onEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) ;
  static  void  handleWebSocketMessage(uint8_t num, uint8_t *data, size_t len) ;
  static  void  sendH1TXT(int num, String message);  
  static  void  sendH2TXT(int num, String message);  
          void  loop() ; 
};

#endif
