/*************************************************** 
  This is an example for the Adafruit NEC Remote Control

  Designed specifically to work with the Adafruit NEC Remote Control
	----> http://www.adafruit.com/products/389
  and IR Receiver Sensor
	----> http://www.adafruit.com/products/157

  These devices use IR to communicate, 1 pin is required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include "iodef.h"
#include "nec.h"
#include <util/delay.h>

#define true 1
#define false 0

//#include "HD44780.h"

//  uint8_t _irpin, _irpinport, _irpinmask;

  uint8_t timedout, timing;
  int32_t maxwaiting,maxwaiting;

  uint16_t measurePulse(uint8_t state);
  uint8_t readNECbit(void);


void Adafruit_NECremote(uint8_t pin) {
 // _irpin = pin;
// _irpinport = PORTA;
// _irpinmask = 1<<5;

  timedout = false;
  timing = false;
  maxwaiting = 0;
}

int16_t listen(int16_t maxwaitseconds,uint8_t * dataArg) {
  uint16_t p1, p2, timer;

int16_t tryCount=0;
  p1 = p2 = 0;
  timer = 0; 
  maxwaiting = maxwaitseconds;
  maxwaiting *= 1000;  // to milliseconds
  maxwaiting *= 1000;  // to microseconds
  
  maxwaiting = 80000; //us, 100ms
  
  if (maxwaitseconds) timing = true;
  timedout = false;






  // wait for starting pulses
  while ( ((p1 < 7000/RESOLUTION) || (p1 > 10000/RESOLUTION)) && 
          ((p2 < 3000/RESOLUTION) || (p2 > 5000/RESOLUTION)))  {
    p1 = measurePulse(0);
    if (timing) {
        maxwaiting -= p1;
        if (maxwaiting < 0) {
          timedout = true;     // timed out waiting
        }
    }
    if (timedout)           return -1;

    p2 = measurePulse(1);     
    if (timing) {
        maxwaiting -= p2;
        if (maxwaiting < 0) {
          timedout = true;     // timed out waiting
        }
    }
    if (timedout)           return -1;
    //Serial.println(p1);
    //Serial.println(p2);
  }






 uint8_t data[4] = {0, 0, 0, 0};
 //data[0]=data[1]=data[2]=data[3]=0;

 
 
  int8_t b; uint8_t ret=0;
  for (uint8_t i=0; i<32; i++) {
    b = readNECbit();
    if (timedout) {
      return -1;
    }
    if  (b < 0) {
      if (i == 0) 
	return -3; // repeat!
      else
	return -1;
		
    }
    data[i/8] |= b << (i%8);
  }
  
  //memcpy(dataArg, data, 4); //3668
  dataArg[0]=data[0]; //3658
  dataArg[1]=data[1];
  dataArg[2]=data[2];
  dataArg[3]=data[3];
 
 
 
  //return 1;

 
 return 1;
}




























uint16_t measurePulse(uint8_t state) {
  uint16_t pulse = 0;
  uint8_t statemask;
  
 if (state) {
   statemask = 1;
 } else {
   statemask = 0;
 }
 
// volatile uint8_t *_irpinreg = PINA;
  // wait for the state to change
  //  while (((IRpin_PIN >> IRpin) & 0x1) != state) {
  while (PA.PIN.B5 != statemask) {
    _delay_us(RESOLUTION);
    if (timing) {
      maxwaiting -= RESOLUTION;
      if (maxwaiting < 0) {
        timedout = true;
        return 0;
      }
    }
  }
  
  // in the proper state, keep track
  while (PA.PIN.B5 == statemask) {
     _delay_us(RESOLUTION);
     pulse++;
     if (timing) {
      maxwaiting -= RESOLUTION;
      if (maxwaiting < 0) {
        timedout = true;
        return 0;
      }
    }
  }
  return pulse;
}


uint8_t retryCounter (void)
{
   //static int32_t maxwaiting;
   uint8_t timedout=false;
   int p1, p2;
   uint8_t tryCount=0;

 while(1)
 {


	/*	p1=measurePulse(0)>maxwaiting;
		p2=measurePulse(1)>maxwaiting;
		
		if(( ((p1 < 7000/RESOLUTION) || (p1 > 10000/RESOLUTION)) && 
		((p2 < 3000/RESOLUTION) || (p2 > 5000/RESOLUTION)))  )
			return tryCount;
*/
//return 200;

	 maxwaiting = 80000; //80ms
	 p1 = p2 = 0;
	 
	  // wait for starting pulses
 while ( ((p1 < 7000/RESOLUTION) || (p1 > 10000/RESOLUTION)) && 
          ((p2 < 3000/RESOLUTION) || (p2 > 5000/RESOLUTION)))  {
    p1 = measurePulse(0);
    if (timing) {
        maxwaiting -= p1;
        if (maxwaiting < 0) {
          timedout = true;     // timed out waiting
        }
    }
    if (timedout)          return tryCount;

    p2 = measurePulse(1);     
    if (timing) {
        maxwaiting -= p2;
        if (maxwaiting < 0) {
          timedout = true;     // timed out waiting
        }
    }
    if (timedout)           return tryCount;
    //Serial.println(p1);
    //Serial.println(p2);
  }

	
	
	
	tryCount++;

} 
}


uint8_t readNECbit(void) {
  int16_t p1, p2;
  
  p1 = measurePulse(0);
  if (timedout) return -1;
  p2 = measurePulse(1); 
  if (timedout) return -1;

  if ((p1 < 400/RESOLUTION) || (p1 > 700/RESOLUTION)) 
    return -1;
  if ((p2 > 400/RESOLUTION) && (p2 < 700/RESOLUTION)) {
     return 0;   // valid bit
  }
   if ((p2 > 1400/RESOLUTION) && (p2 < 1800/RESOLUTION)) {
     return 1;   // valid bit
  } 
  return -1;
}
