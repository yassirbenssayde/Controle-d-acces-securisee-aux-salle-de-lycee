
#include "Door.h"
Door::Door(int latchPin)
{
  this->latchPin  = latchPin;
  pinMode(latchPin, OUTPUT);
  Lock()  ;   // Lock the door        
}

bool   Door::Lock()  
{
  digitalWrite(latchPin, HIGH  );   // Lock the door 
  return true;    
}

bool   Door::Unlock() 
{
  digitalWrite(latchPin,LOW );   // unLock the door for 5s
  delay(5000);
  Lock()    ;   // Lock the door 
  return true;     
}

      
