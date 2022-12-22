#include "CFingerPrint.h"

/**** 
 *  ATTENTION à bien mettre unsigned int pour fingerBauds
 *  car sur un 16 bits -32768......32767
 *  donc on ne peut pas coder 57600 sur un int (16 bits)
 */
fingerPrint::fingerPrint(int rxPIN, int txPin, unsigned int fingerBauds)  
 {
    this->rxPin       = rxPin ;
    this->txPin       = txPin ;
    this->hostBauds   = hostBauds ;
    this->fingerBauds = fingerBauds ;

    mySerial          = new SoftwareSerial(rxPIN, txPin)      ;
    myFinger          = new Adafruit_Fingerprint(mySerial)    ;
    myFinger->begin(fingerBauds)                              ;  // bauds rate between FingerPrint and Arduino
 }

bool fingerPrint::verifyPassword(int numberOfAttempts)      
{
  int i=0 ; 
  while(    (myFinger->verifyPassword()==false) && (i++<numberOfAttempts))
      delay(10);
  if(i<numberOfAttempts)
    return true ;
  else
    return false  ;
}
        
int fingerPrint::getTemplateCount()            
{
    if(myFinger->getTemplateCount()==FINGERPRINT_OK)
      return myFinger->templateCount  ;
    else
      return 0  ;
    
}

 bool  fingerPrint::snapPicture()         
{
    uint8_t p = myFinger->getImage();
    if(p != FINGERPRINT_OK)  
      return false  ;
    else
      return true   ;
}

bool  fingerPrint::picture2Tz(int imageNumber) 
{
    uint8_t p = myFinger->image2Tz(imageNumber);  // 1 ou 2 selon 
    if (p != FINGERPRINT_OK)
      return false  ;
    else
      return true   ; 
}

bool  fingerPrint::pictureFastSearch()
{
   uint8_t p = myFinger->fingerFastSearch();
    if(p != FINGERPRINT_OK)  
      return false  ;
    else
      return true   ;
}
 
bool fingerPrint::authenticate()                
{
  while(!snapPicture());  // attente capture finger
  if(!picture2Tz())
    return false ;
  else
    if(!pictureFastSearch())
      return false ;
    else
      return   true ;  
}

bool   fingerPrint::createModel()         
{
    uint8_t p = myFinger->createModel();
    if(p != FINGERPRINT_OK)  
      return false  ;
    else
      return true   ;
}

bool  fingerPrint::clearAllFingerPrint()                     
{
  myFinger->emptyDatabase();
  return true;
}
      
bool   fingerPrint::storeModel(int id)         
{
  uint8_t p = myFinger->storeModel(id);
  if(p != FINGERPRINT_OK)  
    return false  ;
  else
    return true   ;
}

bool    fingerPrint::enroll_Step_1()                               
{
  while(!snapPicture());  // attente capture finger
  if(!picture2Tz(1))
    return false ;
  else
   return true;   
}

bool fingerPrint::waitingNoFinger(int ms)
{
  delay(ms);
  uint8_t p;
  while ((p =myFinger->getImage())!= FINGERPRINT_NOFINGER) ;
  return true;
}

bool    fingerPrint::enroll_Step_2(int id)                               
{
  while(!snapPicture());  // attente capture finger
  if(!picture2Tz(2))
    return false ;
  else
  {
    if(!createModel())
      return false  ;
    else
        return(storeModel(id));
  }
}
