#include <avr/io.h>
#include <util/delay.h>
#include "tun.h"
///////////////////////////////////////////
#define TUNE_SUCCESS 1
#define TUNE_FAIL -1

#define FM_IF_min  686900    // 10.7MHz-50KHz = 10,650,000 * 0.064 = 681,600
#define	FM_IF_max  687200 

#define CCB1_CE_ON()       PORTA|=1<<0 
#define CCB1_CE_OFF()      PORTA&=~(1<<0) 


#define CCB_WAIT()        _delay_us(5)

void CCBDataOut(uint8_t d);
void pllIN1Out(void);
void pllIN2Out(void);

/*-----------------------------------------------------------
	global variable
-----------------------------------------------------------*/
// IN1
static uint16_t P_CTR;
static uint8_t SNS, DVS, CTE, XS, R_CTR;

// IN2
static uint8_t IO_C, IO, BO, DO_C, UL, DZ_C, GT;
static uint8_t TBC, DLC, IFS, TEST;

// OUT
static uint32_t IF_CTR;
// BYTE IO, UL share IN2 
int Tuned(uint32_t freq )
{
	uint32_t IF_min, IF_max;

	// valid frequency ?
	if(freq < 87500 || freq > 108000){
		return TUNE_FAIL;
	}

	DVS = 1, SNS = 0;
	// IF pll divider
	R_CTR = 3;	// 25KHz
	P_CTR = (freq + 10700)/(25*2); //raczej +
	IF_min = FM_IF_min;
	IF_max = FM_IF_max;

	// select Xtal. frequency
	XS = 0;		// 7.2MHz	0 4.5	

	// set IO pin direction
	IO_C = 3;	// IO1, IO2 are output.
	GT = 3;     // IF count time 64msec
	DO_C = 0;	// DO pin are open.
	UL = 0;		// open
	DZ_C = 0;	// select DZA
	TBC = 0;	// do not use clock time base
	DLC = 0;	// normal charge pomp out
	IFS = 1;	// normal set 1
	TEST = 0;	// default value 


	CTE = 0, IO = 0, BO = 0b1001; // b3-cisza na 0, b2-nic, b1-cisza, b0-st
	UL = 0;
	pllIN1Out();
	_delay_us(50);
	pllIN2Out();
	_delay_us(50);

	
	if(IF_CTR > IF_min && IF_CTR < IF_max) return TUNE_SUCCESS;
	return TUNE_FAIL;
}


/*-----------------------------------------------------------
	IN1 DataOut
-----------------------------------------------------------*/

void pllIN1Out(void)
{
	uint8_t Data11, Data12, Data13;

	Data11 = P_CTR & 0xff;
	Data12 = (P_CTR >> 8) & 0xff;
	Data13 = (SNS & 1);
	Data13 |= ((DVS & 1) << 1);
	Data13 |= ((CTE & 1) << 2);
	Data13 |= ((XS & 1) << 3);
	Data13 |= ((R_CTR & 0x0f) << 4);

	CCBDataOut(0x28);
	CCB_WAIT();
	CCB1_CE_ON();
	CCBDataOut(Data11);
	CCBDataOut(Data12);
	CCBDataOut(Data13);
	CCB1_CE_OFF();

}


/*-----------------------------------------------------------
	IN2 DataOut
-----------------------------------------------------------*/

void pllIN2Out(void)
{
	uint8_t Data21, Data22, Data23;

	Data21 = IO_C & 3;
	Data21 |= ((IO & 3) << 2);
	Data21 |= ((BO & 0x0f) << 4);

	Data22 = ((DO_C & 0x07) << 1);
	Data22 |= ((UL & 3) << 4);
	Data22 |= ((DZ_C & 3) << 6);

	Data23 = GT & 3;
	Data23 |= ((TBC & 1) << 2);
	Data23 |= ((DLC & 1) << 3);
	Data23 |= ((IFS & 1) << 4);
	Data23 |= ((TEST & 0x07) << 5);

	CCBDataOut(0x29);
	CCB_WAIT();
	CCB1_CE_ON();
	CCBDataOut(Data21);
	CCBDataOut(Data22);
	CCBDataOut(Data23);
	CCB1_CE_OFF();

}
///////////////////////////////////////////
