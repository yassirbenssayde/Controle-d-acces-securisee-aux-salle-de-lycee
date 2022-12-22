#include "Websocket.h"
// static : mécanisme de « variables globales » internes à la classe
//Déclarer un membre static dans une classe permet de n'avoir qu'une instance de ce membre en mémoire. 
//Toute modification effectuée sur ce membre dans une instance de cette classe sera visible par les autres instances
WebSocketsServer*       Websocket::ws           ;
fingerPrint*            Websocket::finger       ;

 
Websocket::Websocket(fingerPrint* finger, int port) { 
 ws = new WebSocketsServer(port) ;
 ws->begin()                ;
 ws->onEvent(onEvent)       ;
 // enregistrement du lien vers l'objet led
 this->finger = finger      ;

}

void  Websocket::sendH1TXT(int num, String message)
{
  String messageToSend = "<h1>"+message+"</h1>";
   ws->sendTXT(num,messageToSend);  
}

void  Websocket::sendH2TXT(int num, String message)
{
   String messageToSend = "<h2>"+message+"</h2>";
   ws->sendTXT(num,messageToSend);  
}


//
// La page Web envoie un message toggle afin de changer l'état de la led
//
void Websocket::handleWebSocketMessage(uint8_t num, uint8_t *payload, size_t len) 
{ 
   // Si le message envoyé par le client webSocket est "lecture d'empreinte"
   if (strcmp((char*)payload, "empreinte") == 0) 
   { 
        if(Websocket::finger->verifyPassword()==true)
        {
            // rien ne sert de vérifier une empreintes si le lecteur n'en a aucune
            int nbFingerPrint = Websocket::finger->getTemplateCount() ;
            if(nbFingerPrint>0)
            {
              if(Websocket::finger->authenticate()==true)
              {
                int fingerPrintID = Websocket::finger->getFingerID();
                String reponse = "<h1>OK, je vous ai reconnu vous avez l'id  :"+ String(fingerPrintID)+"</p></h1>";
                sendH1TXT(num, reponse);  
              } 
              else
                sendH1TXT(num,"OK, je ne vous ai pas reconnu");   
            }
            else
                sendH1TXT(num, "OK, aucune empreinte au sein du lecteur");           
        }
        else
        {
            sendH1TXT(num, "OK mais la lecture est inaccessible");           
        }
   }
   else
   { 
      // Si le message envoyé par le client webSocket est "reset de toutes les empreintes"
      if(strcmp((char*)payload, "reset") == 0)
      {
        if(Websocket::finger->verifyPassword()==true)
        {
          if(Websocket::finger->clearAllFingerPrint())
          {
            Serial.print(Websocket::finger->getTemplateCount()); 
            Serial.println(" empreintes dans le fingerPrint");             
            String reponse = "Vous venez d'effacer toutes les empreintes";
            sendH1TXT(num,reponse); 
          }
          else
          {
            Serial.println("Error in clearAllFingerPrint");             
            String reponse = "Error in clearAllFingerPrint";
            sendH1TXT(num,reponse); 
          }
        }
      }
      // Sinon,le message envoyé par le client webSocket est "enregistrement d'empreintes"
      else
      {
        if(strcmp((char*)payload, "register") == 0)
        {
          // la commande est du type : <register:17>
          char * pch;
          pch = strchr((char*)payload,':');
          Serial.print("a partir de pch :");   
          Serial.println(pch+1); 
          int id = atoi(pch+1)  ; // id pour l'enregistrement au sein du fingerPrint
          Serial.print("id pour register :");   
          Serial.println(id);    
          if(Websocket::finger->verifyPassword()==true)
          {
            Serial.print(Websocket::finger->getTemplateCount()); 
            Serial.println(" empreintes dans le fingerPrint");             
            Serial.println("Waiting for valid finger to enroll as #");
            if(Websocket::finger->enroll_Step_1())
            {
              Serial.println("Remove finger");
              sendH1TXT(num,"Remove finger");
              Websocket::finger->waitingNoFinger();  //// 1000 ms par défaut
              Serial.println("Place same finger again");
              sendH1TXT(num,"Place same finger again");
              
              if(!Websocket::finger->enroll_Step_2(id))
              {
                Serial.println("Erreur de enroll_Step_2");
                sendH1TXT(num,"Erreur de enroll_Step_2"); 
              }
              else
              {
                Serial.println("Stored!");
                String reponse = "<h1>OK, la nouvelle empreinte est mémorisée</h1>";
                sendH1TXT(num,reponse); 
              }
            }
            else
            {
                Serial.println("Erreur de enroll_Step_1"); 
                sendH1TXT(num,"Erreur de enroll_Step_1");
            }     
          }
          else
          {
            Serial.println("Erreur de VerifyPassword");
            sendH1TXT(num,"Erreur de VerifyPassword");
          } 
        }
        else
        {
          if(strcmp((char*)payload, "open") == 0)
          {
           
          }
        }
      }
   }
}

// num is the current client/connection number/ID
// The arduinoWebsocket library allows a maximum of 5 at any one time.
// payload is the received data (when there is received data)
// type is the response type:
//0 – WStype_ERROR
//1 – WStype_DISCONNECTED
//2 – WStype_CONNECTED
//3 – WStype_TEXT
//4 – WStype_BIN
//5 – WStype_FRAGMENT_TEXT_START
//6 – WStype_FRAGMENT_BIN_START
//7 – WStype_FRAGMENT
//8 – WStype_FRAGMENT_FIN
//9 – WStype_PING
//10- WStype_PONG

void Websocket::onEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) 
{
    IPAddress ip ;
    switch(type) 
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        //ws->sendTXT(num, "Disconnected");
        break;
        
    case WStype_CONNECTED:
       ip = ws->remoteIP(num);
       Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3],payload);
       // send message to client
       //ws->sendTXT(num, "Connected");
       break;
       
    case WStype_TEXT:
       Serial.printf("[%u] get Text: %s\n", num, payload);
       handleWebSocketMessage(num, payload, length);
       // send message to client
       // webSocket.sendTXT(num, "message here");
       // send data to all connected clients
       // webSocket.broadcastTXT("message here");
       break;
       
    case WStype_BIN:
       Serial.printf("[%u] get binary length: %u\n", num, length);
       hexdump(payload, length);
       // send message to client
       // ws->sendBIN(num, payload, length);
       break;
   
    case WStype_PING:
       // pong will be send automatically
       Serial.printf("[WSc] get ping\n");
       break;
       
    case WStype_PONG:
       // answer to a ping we send
       Serial.printf("[WSc] get pong\n");
       break;
    }
}

void Websocket::loop() {
 ws->loop();
}
