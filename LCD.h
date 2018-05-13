#ifndef __lcd
#define __lcd

/** D E F I N I T I O N S ****************************************************/
#define rs PORTCbits.RC3	// RS
#define en PORTCbits.RC2	// EN
#define db PORTD			// DB4 - RD0 , DB5 - RD1 , DB6 - RD2 , DB7 -RD3


/** P R O T O T Y P E S ******************************************************/
/* To initialize LCD for 4 bit mode*/
void initialize_lcd(void);
/*	To write command to the screen
()Parameter takes values for different operation
*/
void write_command(unsigned char);
/*	To write command to the screen*/
void write_data(unsigned char);
/*	To display number to the screen
*	It takes number(int), convert it into char
*	and display on screen
*/
void disp_num(int);
/*	To clear the screen*/
void clear_screen (void);

void lcdGoto(unsigned char);
#endif