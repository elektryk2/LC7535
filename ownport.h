#include <avr/io.h>

struct bit {
    unsigned char B0:1;
    unsigned char B1:1;
    unsigned char B2:1;
    unsigned char B3:1;
    unsigned char B4:1;
    unsigned char B5:1;
    unsigned char B6:1;
    unsigned char B7:1;
};   

typedef union {
	volatile unsigned char BYTE;                  
	struct {                            
		unsigned char B0:1;
		unsigned char B1:1;
		unsigned char B2:1;
		unsigned char B3:1;
		unsigned char B4:1;
		unsigned char B5:1;
		unsigned char B6:1;
		unsigned char B7:1;
	  }      BIT;                   
}divideToBits; 
        

//#define pinD	(*(volatile struct bit    *)&PIND)


struct binport{
	struct bit PIN;
	struct bit DDR;
	struct bit PORT;
};


#define PA	(*(volatile struct binport    *)&PINA)
#define PB	(*(volatile struct binport    *)&PINB)
#define PC	(*(volatile struct binport    *)&PINC)
#define PD	(*(volatile struct binport    *)&PIND)