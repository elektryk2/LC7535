//-------------------------------------------------------------------------------------------------
// Wy�wietlacz alfanumeryczny ze sterownikiem HD44780
// Sterowanie w trybie 4-bitowym bez odczytu flagi zaj�to�ci
// z dowolnym przypisaniem sygna��w steruj�cych
// Plik : HD44780.c	
// Mikrokontroler : Atmel AVR
// Kompilator : avr-gcc
// Autor : Rados�aw Kwiecie�
// �r�d�o : http://radzio.dxp.pl/hd44780/
// Data : 24.03.2007
//-------------------------------------------------------------------------------------------------

#include "HD44780.h"
#include "ownport.h"
#define CS 
#define RCK


#define RS	data.BIT.B2
#define E	data.BIT.B3
#define D4 	data.BIT.B4 ///to ju� nie ma sensu, pojdzie po calosci 
divideToBits data;

void writeSpi(void)
{
SPDR = data.BYTE;					//Wysy�amy zawarto�� zmiennej bajt	
while( ! bit_is_set( SPSR, SPIF ) );		//Oczekujemy na zako�czenie transmisji ( do ustawienia SPIF ) przez sprz�t
PB.PORT.B2=1;
_delay_us(5);
PB.PORT.B2=0;
}

//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiaj�ca p�bajt na magistral� danych
//
//-------------------------------------------------------------------------------------------------
void _LCD_OutNibble(unsigned char nibbleToWrite)
{
E=1;
writeSpi();
data.BYTE &= 0b00001111;
data.BYTE |= (0b11110000 & (nibbleToWrite <<4));
writeSpi();
E=0;
writeSpi();		
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wy�wietacza (bez rozr�nienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------


void _LCD_WriteSPI(unsigned char dataToWrite)
{

_LCD_OutNibble(dataToWrite >> 4);

_LCD_OutNibble(dataToWrite);

_delay_us(50);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wy�wietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteCommand(unsigned char commandToWrite)
{
RS=0; writeSpi();
_LCD_WriteSPI(commandToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pami�ci wy�wietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteData(unsigned char dataToWrite)
{
RS=1; writeSpi();
_LCD_WriteSPI(dataToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wy�wietlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteText(char * text)
{
while(*text)
  LCD_WriteData(*text++);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia wsp�rz�dnych ekranowych
//
//-------------------------------------------------------------------------------------------------
void LCD_GoTo(unsigned char x, unsigned char y)
{
LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja czyszczenia ekranu wy�wietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Clear(void)
{
LCD_WriteCommand(HD44780_CLEAR);
_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywr�cenia pocz�tkowych wsp�rz�dnych wy�wietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Home(void)
{
LCD_WriteCommand(HD44780_HOME);
_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
void LCD_Initalize(void)
{
unsigned char i;

_delay_ms(15); // oczekiwanie na ustalibizowanie si� napiecia zasilajacego

data.BYTE=0;


writeSpi();


for(i = 0; i < 3; i++) // trzykrotne powt�rzenie bloku instrukcji
  {
  _LCD_OutNibble(0x03); // tryb 8-bitowy
  _delay_ms(5); // czekaj 5ms
  }


_LCD_OutNibble(0x02); // tryb 4-bitowy


_delay_ms(1); // czekaj 1ms 
LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wy��czenie wyswietlacza
LCD_WriteCommand(HD44780_CLEAR); // czyszczenie zawartos�i pamieci DDRAM
_delay_ms(2);
LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);// inkrementaja adresu i przesuwanie kursora
LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // w��cz LCD, bez kursora i mrugania


}

//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.c
//
//-------------------------------------------------------------------------------------------------
