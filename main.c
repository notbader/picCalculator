/** C O N F I G U R A T I O N   B I T S ******************************/
#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF                       // CONFIG1H
#pragma config PWRT = OFF, BOREN = SBORDIS, BORV = 30                        // CONFIG2L
#pragma config WDTEN = OFF, WDTPS = 32768                     // CONFIG2H
#pragma config MCLRE = ON, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC       // CONFIG3H
#pragma config STVREN = ON, LVP = OFF, XINST = OFF                          // CONFIG4L
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF                   // CONFIG5L
#pragma config CPB = OFF, CPD = OFF                                         // CONFIG5H
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF               // CONFIG6L
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF                           // CONFIG6H
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF           // CONFIG7L
#pragma config EBTRB = OFF

/** I N C L U D E S **************************************************/
#include "p18f46k20.h"
#include "delays.h"
#include "lcd.h"

/** F U N C T I O N S**************************************************/
void Chk_INT (void);
void Read_Decode_Keypad (void);
int conv_to_number(char ch);

/** V A R I A B L E S *************************************************/
unsigned int num1 =0;
unsigned int num2 =0;
unsigned char keypad[4][4] = {'1','2','3','A',
                              '4','5','6','B',
			      '7','8','9','C',
							  '*','0','#','D'};	//KEYPAD PATTERN
unsigned char keyg = 0;
unsigned int count =0;

/** I N T E R R U P T S ***********************************************/
#pragma code My_HiPrio_Int = 0x0008	//HIGH PRIORITY INTERRUPT FOR PORTB CHANGE STATE INTERRUPT
void My_HiPrio_Int ()
{
	_asm
	GOTO Chk_INT
	_endasm
}
#pragma code

#pragma interrupt Chk_INT
void Chk_INT (void)
{
	if (INTCONbits.RBIF == 1)	// CHECK FOR INTERRUPT FLAG
	Read_Decode_Keypad();
}
/** D E C L A R A T I O N S *******************************************/
#pragma code


void main()
{
	unsigned char message[7] ={'W','E','L','C','O','M','E'};
	char i=0;

	OSCCON = 82;
	OSCTUNEbits.PLLEN = 0;

	TRISCbits.TRISC3 = 0;	// RS AS OUTPUT
	TRISCbits.TRISC2 = 0;	// EN AS OUTPUT
	INTCON2bits.RBPU = 0;	// INTERNAL PULL-UP ENABLE

	TRISB = 0xF0;		// MAKE COLUMNS AS INPUT AND ROWS AS OUTPUT
	PORTB = 0xF0;
	TRISD = 0x00;
	PORTD = 0xF0;
	initialize_lcd();		// INITIALIZE LCD

	for (i;i<7;i++)
	write_data(message[i]);	// DISPLAY WELCOME MESSAGE
	write_data(33); // include exclamation mark
	Delay10KTCYx(200);
	clear_screen();

	while (PORTB!=0xF0);
	INTCONbits.RBIE = 1;	// ENABLE PORTB CHANGE STATE INTERRUPT
	IOCB = 0xFF;
	INTCONbits.GIE = 1;		// ENABLE INTERRUPT GLOBALLY
	while (1);

}
/*	This is the PORTB CHANGE STATE ISR
	for detection of pressed key
*/
void Read_Decode_Keypad (void)
{
	unsigned char temp, col = 0, row = 4;
	unsigned char operation;

	Delay1KTCYx(15);
	temp = PORTB;				// GET COLUMN
	temp^=0xF0;					// INVERT HIGH NIBBLE
	if (!temp) return;
	while (temp<<=1) col++;		// FIND COLUMN
	PORTB = 0xF7;				// GROUND ROW 0
	if (PORTB!=0xF7)
	row = 0;					// IF HIGH NIBBLE CHANGE IT'S ROW 0
	else {						//ELSE
		PORTB = 0xFB;
	if (PORTB!=0xFB)			// IS IT ROW 1?
	row = 1;
	else {
		PORTB = 0xFD;			// IS IT ROW 2?
	if (PORTB!=0xFD)
	row = 2;
	else {
		PORTB = 0xFE;			// IS IT ROW 3?
	if (PORTB!=0xFE)
	row = 3;
	}
	}
	}


	if (row<4){
		count++;
	keyg = keypad[row][col]; // SAVE THE CORRESPONDING NUMBER
	if (keyg == '*'){
		count =0;
		clear_screen();		//USE "*" FOR CLEARING SCREEN ANY TIME
	}
	else
	{
	switch(count)         //switch on function
	{
	 case (1):	// IF FIRST TIME KEY DETECTED ITS MUST BE NUMBER OTHERWISE SHOW ERROR AND CLEAR SCREEN
             if (keyg =='A' | keyg =='B' | keyg =='C' | keyg =='D' | keyg =='*' |keyg =='#')
		 {
                	 lcdGoto(0x85);
			 write_data ('E');write_data ('r');write_data ('r');write_data ('o');write_data ('r');
			 Delay10KTCYx(100);
			 clear_screen();
			 count =0;
				 }
                             else
				 {
				 write_data(keyg);				// SHOW NUMBER ON LCD
				 num1 = conv_to_number(keyg);	// CONVERT CHARACTER TO NUMBER
					 }
         break;

	 case (2):// IF SECOND TIME KEY DETECTED ITS MUST BE OPERATION OTHERWISE SHOW ERROR AND CLEAR SCREEN
            if (keyg =='1' | keyg =='2' | keyg =='3' | keyg =='4' | keyg =='5' |keyg =='6'| keyg =='7' | keyg =='8' | keyg =='9' | keyg =='0' |keyg =='*'|keyg =='#')
		 {
			 lcdGoto(0x85);
			 write_data ('E');write_data ('r');write_data ('r');write_data ('o');write_data ('r');
			 Delay10KTCYx(100);
			 clear_screen();
			 count =0;
					 }
			 else
                             {
	 switch (keyg)
			{
         case 'A' : write_data('+');break;		// A = "+"
         case 'B' : write_data('-');break;		// B = "-"
	 case 'C' : write_data('*');break;		// C = "*"
	 case 'D' : write_data('/');break;		// D = "/"
					}

		 operation = keyg;			// SAVE OPERATION
					 }
	break;

        case (3): //THIRD VALUE MUST BE THE SECOND NUMBER
             if (keyg =='A' | keyg =='B' | keyg =='C' | keyg =='D' | keyg =='*' |keyg =='#')
		 {
			 lcdGoto(0x85);
			 write_data ('E');write_data ('r');write_data ('r');write_data ('o');write_data ('r');
			 Delay10KTCYx(100);
			 clear_screen();
			 count =0;
				 }
			 else
                             {
				 write_data(keyg);				// SHOW NUMBER ON SCREEN
				 num2 = conv_to_number(keyg);	// CONVERT CHARACTER TO NUMBER
					 }
     break;

     case (4):// "#" IS USED FOR "="
		 if (keyg !='#')
                     {
			 lcdGoto(0x85);
			 write_data ('E');write_data ('r');write_data ('r');write_data ('o');write_data ('r');
			 Delay10KTCYx(100);
			 clear_screen();
			 count =0;
					 }
			 else
				 {
				 write_data ('=');	// WRITE "=" ON SCREEN
				 switch (operation)	// PERFORM OPERATION
					{
				case 'A' : disp_num (num1+num2);break;
				case 'B' : disp_num (num1-num2);break;
				case 'C' : disp_num (num1*num2);break;
				case 'D' : disp_num (num1/num2);break;
							}
				count =0;			//USE "*" TO RESET CALCULATOR FOR NEXT CALCULATION
					 }

	break;
				}
	}		}

	while (PORTB != 0xF0)
	PORTB = 0xF0;
	INTCONbits.RBIF = 0;
}
int conv_to_number(char ch)         //CONVERT CHARACTER INTO INTEGER
{
	int num = 0;

	switch(ch)
	{
		case '0': num = 0; break;
		case '1': num = 1; break;
		case '2': num = 2; break;
		case '3': num = 3; break;
		case '4': num = 4; break;
		case '5': num = 5; break;
		case '6': num = 6; break;
		case '7': num = 7; break;
		case '8': num = 8; break;
		case '9': num = 9; break;
		default: write_data ('E'); break;       //it means wrong input
	}

	return num;
}