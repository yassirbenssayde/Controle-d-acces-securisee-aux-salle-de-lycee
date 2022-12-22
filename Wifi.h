#ifndef WIFI_H_
#define WIFI_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>

class Wifi{

  private :
    String    STA_SSID    ;
    String    STA_PSK   ;
    bool      connected   ;   

  public  :
    bool        WaitingSTAConnection(int delay=500) ;    
                Wifi(String ssid, String passwd)    ;
   IPAddress   local_IP()                          ;     
};

#endif
