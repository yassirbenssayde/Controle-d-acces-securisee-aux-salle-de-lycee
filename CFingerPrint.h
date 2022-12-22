#ifndef _FINGER
#define _FINGER
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

class fingerPrint {
  private:
        SoftwareSerial        *mySerial             ;
        Adafruit_Fingerprint  *myFinger             ;

        String                message               ;
        int                   rxPin                 ; // Rx de l'arduino <---Tx du FingerPrint
        int                   txPin                 ; // Tx de l'arduino <---Rx du FingerPrint
        int                   hostBauds             ; // 9600 par défaut, between arduino and host
        unsigned int          fingerBauds           ; // 57600 par défaut, between arduino and host
        bool                  snapPicture()         ;
        bool                  picture2Tz(int=1)     ; // paramètre 1 ou 2
        bool                  pictureFastSearch()   ;
        
        bool                  createModel()         ;
        bool                  storeModel(int )      ;        
  public:
                              fingerPrint(int rsPIN, int txPin, unsigned int fingerBauds=57600)  ;
      bool                    verifyPassword(int numberOfAttempts=10)       ;
      int                     getTemplateCount()                            ;
      bool                    authenticate()                                ;
      bool                    enroll_Step_1()                               ;
      bool                    enroll_Step_2(int)                            ;
      bool                    waitingNoFinger(int = 1000)                   ;
      bool                    clearAllFingerPrint()                         ;
      int                     getFingerID() { return myFinger->fingerID ;}
      int                     getConfidence() { return myFinger->confidence ;}
};

#endif
