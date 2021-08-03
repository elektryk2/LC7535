#include <avr/io.h>
#include "enco.h"

int val=0;
int val_tmp;

uint8_t read_gray_code_from_encoder(void ) 
{ 
 uint8_t val=0; 

  if(!bit_is_clear(PIND, PD2)) 
	val |= (1<<1); 

  if(!bit_is_clear(PIND, PD3)) 
	val |= (1<<0); 

  return val; 
}

int8_t readEnc(void)
{
	uint8_t retVal=0;

	val_tmp = read_gray_code_from_encoder(); 

	   if(val != val_tmp) 
	   { 
		   if( /*(val==2 && val_tmp==3) ||*/ 
			   (val==3 && val_tmp==1) || 
			   /*(val==1 && val_tmp==0) ||*/ 
			   (val==0 && val_tmp==2) 
			 ) 
		   { 
				retVal=1;; 
		   } 
		   else if( /*(val==3 && val_tmp==2) ||*/ 
			   (val==2 && val_tmp==0) || 
			   /*(val==0 && val_tmp==1) ||*/ 
			   (val==1 && val_tmp==3) 
			 ) 
		   { 
				retVal=-1;
		   } 

		   val = val_tmp; 
	   } 
	return retVal;
}
