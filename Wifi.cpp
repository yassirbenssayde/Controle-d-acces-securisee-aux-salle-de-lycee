#include "Wifi.h"

Wifi::Wifi(String ssid, String passwd){
    STA_SSID    = ssid            ;
    STA_PSK     = passwd          ;

    WiFi.mode(WIFI_STA)           ;
    WiFi.begin(STA_SSID, STA_PSK) ;  
}

bool Wifi::WaitingSTAConnection(int elapse) {
    delay(elapse);   
    if (WiFi.status() != WL_CONNECTED) 
      return false;
    else
    return true ;
}

IPAddress Wifi::local_IP() {
  return WiFi.localIP();
}

  
