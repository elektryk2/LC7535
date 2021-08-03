#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "volArr.h"

#include "vol.h"

#define CCB_CE_ON()       PORTA|=1<<3  
#define CCB_CE_OFF()      PORTA&=~(1<<3)

#define CCB_DATA_ON()     PORTA|=1<<4 
#define CCB_DATA_OFF()    PORTA&=~(1<<4)  

#define	CCB_CLOCK_ON()	  PORTA|=1<<1  
#define CCB_CLOCK_OFF()   PORTA &=~(1<<1) 



#define CCB_WAIT()        _delay_us(5)

void    CCBDataOut(uint8_t d);
void CCBDataOutHalf(uint8_t d);

static int vol_old;



int8_t setVol(int8_t vol)
{/*
CCB_CE_ON();
CCB_DATA_ON();
CCB_CLOCK_ON();
return 0;*/
	if(vol>81) vol=81;
	if(vol<0) vol=0;

	//if(vol_old==vol) return vol; //bez zmian


	uint8_t Data11, Data12;

	CCBDataOutHalf(0b1001);
	CCB_WAIT();
	CCB_CE_ON();

	Data11=pgm_read_byte(&tab[vol]);
	Data12=Data11;

	CCBDataOut(Data11);
	CCBDataOut(Data12);


	CCB_CE_OFF();
	vol_old=vol;

	return vol;
}




/*-----------------------------------------------------------
	CCBDataOut()
-----------------------------------------------------------*/

void CCBDataOut(uint8_t d)
{
	int n;

	for(n = 0; n < 8; n++){
		if(d & 1){
			CCB_DATA_ON();
		}else{
			CCB_DATA_OFF();
		}
		CCB_WAIT();
		CCB_CLOCK_ON();
		CCB_WAIT();
		CCB_CLOCK_OFF();
		d >>= 1;
	}
	CCB_DATA_OFF();
}	

void CCBDataOutHalf(uint8_t d)
{
	int n;

	for(n = 0; n < 4; n++){
		if(d & 1){
			CCB_DATA_ON();
		}else{
			CCB_DATA_OFF();
		}
		CCB_WAIT();
		CCB_CLOCK_ON();
		CCB_WAIT();
		CCB_CLOCK_OFF();
		d >>= 1;
	}
	CCB_DATA_OFF();
}
