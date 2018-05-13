
/** I N C L U D E S **************************************************/
#include "p18f46k20.h"
#include "delays.h"
#include "LCD.h"

void write_command(unsigned char cmd)
{
	en=0;
	rs=0;
	Delay10TCYx(4);			// 40 us DELAY
	db &= 0xf0;
	db = (cmd>>4)&0x0f;		// SEND UPPER NIBBLE
    en = 1;
	Delay1TCY();
    en = 0;
    Delay1KTCYx(5);			// DELAY OF 5 msec
    db &= 0xf0;
    db = cmd&0x0f;			//SEND LOWER NIBBLE
    en = 1;
    Delay1TCY();
    en = 0;
    Delay1KTCYx(5);			//DELAY OF 5 msec
}

void write_data(unsigned char data)
{
	en=0;
	rs=1;					//RS = 1 FOR DATA WRITING
	Delay10TCYx(4);
	db &= 0xf0;
	db |= (data>>4)&0x0f;	// SEND UPPER NIBBLE
    en = 1;
	Delay1TCY();
    en = 0;
    Delay1KTCYx(5);
    db &= 0xf0;
    db |= data&0x0f;		//SEND LOWER NIBBLE
    en = 1;
    Delay1TCY();
    en = 0;
    Delay1KTCYx(5);
}

void initialize_lcd(void)
{
	Delay1KTCYx(15);		// Wait for 15 msec
	write_command(3);
	Delay100TCYx(45);		// Wait for 4.5 msec
	write_command(3);
	Delay100TCYx(2);		// Wait for 200 usec
	write_command(3);
	Delay100TCYx(2);		// Wait for 200 usec
	write_command(2);
	Delay1KTCYx(5);			// Wait for 5 msec
	write_command(40);		// For 4 bit 2 lines 5x7 font
	write_command(0x0F);	// Display on, cursor on, blink on
	write_command(6);		// Cursor increment,Display not shift
}
void disp_num(int number)            //displays number on LCD
{
	unsigned char Unit_Digit  = 0;  //It will contain unit digit of numb
	unsigned char Tenth_Digit = 0;  //It will contain 10th position digit of numb

	if(number<0)					// IF NUMBER IS NEGATIVE
	{
		number = -1*number;          // Make number positive
		write_data('-');	 	// Display a negative sign on LCD
	}

	Tenth_Digit = (number/10);

	if( Tenth_Digit != 0)				// If it is zero, then don't display
		write_data(Tenth_Digit+0x30);	// CONVERT NUMBER TO CHARACTER

	Unit_Digit = number - Tenth_Digit*10;

	write_data(Unit_Digit+0x30);	  // CONVERT NUMBER TO CHARACTER
}

void lcdGoto(unsigned char address)
{
	write_command (address);
}

void clear_screen(void)
{
	write_command (1);		// CLEAR SCREEN
	write_command(0x0F);	// Display on, cursor on, blink on
}