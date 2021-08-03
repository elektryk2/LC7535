//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "iodef.h"
#include <avr/eeprom.h>
#include <util/delay.h>
#include "vol.h"
#include "tun.h"
#include "HD44780.h"
#include <stdlib.h>
#include <avr/interrupt.h> 
#include "nec.h"

#include "enco.h"


//static EEMEM uint8_t remDataEep[4];

void setChannel(uint8_t);
char * uichr(uint16_t x);
char * dBchr(int8_t x);
char * hzChr(uint32_t x);
 //void bzero(void *s, size_t n);

void freqLCD(void);

void volLCD(void);

void relays(void);
volatile uint8_t voll;
divideToBits relOut;




uint8_t retryCounter (void);

uint8_t readChannel(void)
{
	if(!iCD) return 0;
	if(!iAUX) return 1;
	if(!iTUNER) return 2;
	return 10;
}

char buff[10];

	uint8_t vol;
	uint32_t freq;
	
int main (void)
{


	_delay_ms(300);

	uint8_t remData[4];
	
	uint8_t retryCount;
	
	freq=106400L;
	DDRA=255;
	DDRB=0b10111111;
	
	PA.DDR.B5=0;
	PA.PORT.B5=1;
	
	DDRC=0;
	PORTC=255;

	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR1 ) | ( 1 << SPR0 );   //W³¹czamy SPI, 
	_delay_ms(10);
	LCD_Initalize();
	_delay_ms(10);
    LCD_Clear();	
	_delay_ms(10);
	LCD_GoTo(0,0);
	LCD_WriteText("RTC");
	
	relOut.BIT.B1=1;
	relays();

	vol=60;
	
	//uint32_t cnt=0;
	//voll=25;
	
	Tuned(freq);
	int16_t rem=0;
	
	LCD_WriteText("           ");
	LCD_GoTo(0,0);
	LCD_WriteText("Vol: ");
	LCD_WriteText(dBchr(vol));
	LCD_WriteText(" SP:AB");

	LCD_GoTo(0,1);
	LCD_WriteText("Src: ");
	LCD_WriteText("Aux");
	///LCD_WriteText(hzChr(freq));
	
	LCD_GoTo(15,1);
	LCD_WriteText("1");
	setVol(vol);
	
	Adafruit_NECremote(5);
	//while(1){}
	while(1)
	{
		if((voll=readEnc())!=0)
		{
			//LCD_GoTo(0,0);
			//LCD_WriteText("Vol: ");
			//LCD_WriteText(dBchr(vol));
			vol+=voll;
			//vol=setVol(vol);
			
			
			volLCD();
		}
		
//		if(cnt>16000) //opoznienie buttonow
//		{
//			cnt=0;
			
		if(!PC.PIN.B6 || !PC.PIN.B4) 
		{
			if(!PC.PIN.B6) 
				freq+=100;
				
				
			if(!PC.PIN.B4) 		
				freq-=100;
				
			freqLCD();
			_delay_ms(150);
		}
//		}
		
//		cnt++;

		//	_delay_ms(15);
		if(!PA.PIN.B5) 
		{
			rem=listen(1, remData);
			

				
				
				//LCD_GoTo(12,0);
			//	if(remData[0]) LCD_WriteText(uichr(remData[2]));
			//	if(remData[0]) LCD_WriteText(uichr(remData[3]));
			if(/*rem==-3 || */rem==1)
			{	
				retryCount=0;
				do
				{
					if(remData[2] == 0x59 &&  remData[3]==0xa6) 
					{
						vol++;
						volLCD();
					}
					
					if(remData[2] == 0x51 &&  remData[3]==0xae) 
					{
						vol--;
						volLCD();
					}
						
												
					if(remData[2] == 29 &&  remData[3]==226 && retryCount) 
					{	
						freq+=100;
						freqLCD();
						
					}	
					if(remData[2] == 0 &&  remData[3]==255 && retryCount) 		
					{	
						freq-=100;
						freqLCD();
					}	
					
					retryCount++;
					
				}while(retryCounter()>0);
				
				

				//bzero(remData, 4);
				
				//LCD_GoTo(16,0);
				//if(remData[0] && rem>=0) LCD_WriteText(uichr( retryCounter()));
			}
		}

			//if(remData[0]) eeprom_update_block(remData, remDataEep, 4);

			//LCD_WriteText(remData);
			
			///voldn 02 FD 59 A6
			///volup 02 FD 51 AE
			
			//tun up 29 226
			//tun dn 0 255

	}

}


char * dBchr(int8_t x)
{
	buff[0]='-';
	buff[1]='0' + x/10;
	buff[2]='0' + x%10;
	buff[3]='d';
	buff[4]='B';
	buff[5]=0;
	
	
	if(x==0) buff[0]=' ';
	if(buff[1] == '0') buff[1]=' ';
	return buff;
}

char * uichr(uint16_t x)
{

	buff[0]='0' + x/100;
	buff[1]='0' + x%100/10;
	buff[2]='0' + x%100%10;
	buff[3]=0;

	
	
	if(x==0) buff[0]=' ';
	//if(buff[1] == '0') buff[1]=' ';
	return buff;
}

char * hzChr(uint32_t x)
{
	buff[0]='0' + x/100000L;
	buff[1]='0' + x%100000L/10000L;
	buff[2]='0' + x%100000L%10000L/1000L;
	buff[3]='.';
	buff[4]='0' + x%100000L%10000L%1000L/100L;
	buff[5]=0;
	
	
	if(x<100000L) buff[0]=' ';
	//if(buff[1] == '0') buff[1]=' ';
	return buff;
}

void relays(void)
{
SPDR = relOut.BYTE;					//Wysy³amy zawartoœæ zmiennej bajt	
while( ! bit_is_set( SPSR, SPIF ) );		//Oczekujemy na zakoñczenie transmisji ( do ustawienia SPIF ) przez sprzêt
PB.PORT.B3=1;
PB.PORT.B3=0;

}

void freqLCD(void)
{
	if(freq>108000) freq=88000;
	if(freq<88000) freq=108000;
	Tuned(freq);
	LCD_GoTo(5,1);
	LCD_WriteText(hzChr(freq));
	LCD_WriteText(" MHz");
	//_delay_ms(70);
}

void volLCD(void)
{
	vol=setVol(vol);
	LCD_GoTo(5,0);
	//LCD_WriteText("Vol: ");
	LCD_WriteText(dBchr(vol));
}
