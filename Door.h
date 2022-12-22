#ifndef _DOOR
#define _DOOR
#include <Arduino.h>

class Door {
  private: 
      int   latchPin          ;
  public  :
            Door(int latchPin);
      bool  Lock()            ;
      bool  Unlock()          ;      
};

#endif
